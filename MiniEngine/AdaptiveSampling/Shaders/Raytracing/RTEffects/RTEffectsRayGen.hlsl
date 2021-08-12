#define HLSL
#include "RTEffectsCommon.h"
#include "../../Util/RaytraceCommon.h"

#define RT_AMBIENT_OCCLUSION
#define RT_SOFT_SHADOW

[shader("raygeneration")]
void RTEffectsRayGen()
{
	// Early-out test so we don't waste rays on pixels that don't hit any geometry.
	if (g_Normals[DispatchRaysIndex().xy].a < 0.0f)
	{
		g_RTEffects[DispatchRaysIndex().xy] = 0.0f;
		return;
	}

	// For 2nd pass: test for the noise map;
	if (g_NoiseMap[DispatchRaysIndex().xy] < 0.01f)
	{
		return;
	}

	// We need the world position in the variance check, so we calculate it here.

	const uint2 launchIndex = DispatchRaysIndex().xy; // screen pixel coordinate
	const float2 dims = float2(DispatchRaysDimensions().xy); // screen dimensions
	const float2 d = (((launchIndex.xy + 0.5f) / dims.xy) * 2.0f - 1.0f); // NDC

	uint randSeed = InitRand(DispatchRaysIndex().x + DispatchRaysIndex().y * DispatchRaysDimensions().x, g_Dynamic.frameCount);

	// Calculate the world position from the pixel index combined with the depth stored in 
	const float4 target = mul(g_Dynamic.projectionI, float4(d.x, -d.y, 1, 1));
	const float3 pixelWorldPos = mul(g_Dynamic.viewI, float4(0, 0, 0, 1)).xyz;
	const float3 pixelWoldDir = mul(g_Dynamic.viewI, float4(target.xyz, 0)).xyz;

	const float3 worldPos = pixelWorldPos + pixelWoldDir * g_Normals[DispatchRaysIndex().xy].a;

	// Temporal reprojection.
	float4 reprojected = mul(g_Dynamic.previousViewProj, float4(worldPos, 1)); // Previous world position.
	reprojected.xyz = reprojected.xyz / reprojected.w; // Perspective divide
	reprojected.x = (reprojected.x + 1.0f) / 2.0f * DispatchRaysDimensions().x; // map to viewport, with pixel indices.
	reprojected.y = (1.0f - reprojected.y) / 2.0f * DispatchRaysDimensions().y;

	// Variance check
	// Also check if we have reached our minimum amount of passes yet.
	if (g_Dynamic.frameCount != 0 && g_Dynamic.currentPassIdx > g_Dynamic.minimumPassCount)
	{
		const float output = g_RTEffects[DispatchRaysIndex().xy].r;
		const float previous = g_RTEffectsPrevious[reprojected.xy].r;

		float variance;
		if (g_Dynamic.frameCount > 2)
			variance = pow(output - previous, 2) / (g_Dynamic.frameCount - 1);
		else
			variance = 0;

		// todo: investigate why the variance is sometimes NaN - I suspect it is a precision error, could also be a parallelism issue according to Matthieu.
		if (variance == 0x7FF) // NaN
			variance = 0;

		// If the variance is low enough, we don't need to trace more rays
		if (variance < g_Dynamic.varianceToleration) // noiseAcceptance defaulted to 0.001f
			return;
	}

	//
	// Ambient Occlusion
	//
	float ao = 1.0f;
#ifdef RT_AMBIENT_OCCLUSION
	const float3 normal = g_Normals[DispatchRaysIndex().xy].xyz;
	const float3 aoDir = GetCosHemisphereSample(randSeed, normal);

	const float aoTMax = 100;
	RayDesc aoRay;
	aoRay.Origin = worldPos;
	aoRay.Direction = aoDir;
	aoRay.TMin = 0.01f;
	aoRay.TMax = aoTMax;
	HitInfo aoPayload;
	aoPayload.rayHitT = aoTMax;
	aoPayload.skipShading = true;
	TraceRay(g_BVH, RAY_FLAG_ACCEPT_FIRST_HIT_AND_END_SEARCH, 0xFF, 0, 1, 0, aoRay, aoPayload);

	ao = aoPayload.rayHitT / aoTMax;
#endif

	//
	// Soft Shadows
	//
	float shadow = 1.0f;
#ifdef RT_SOFT_SHADOW
	const float shadowValue = 0.3f;
	const float3 shadowDir = float3(shadowSettings.shadowDirX, shadowSettings.shadowDirY, shadowSettings.shadowDirZ);
	const float3 randomOffset = GetRandomOnUnitSphere(randSeed) * shadowSettings.sphereSize;

	const float3 shadowRayDir = normalize(normalize(shadowDir) * shadowSettings.sphereOffset + randomOffset);
	RayDesc shadowRay;
	shadowRay.TMin = 0.1f;
	shadowRay.TMax = FLT_MAX;
	shadowRay.Origin = worldPos;
	shadowRay.Direction = shadowRayDir;
	HitInfo shadowPayload;
	shadowPayload.rayHitT = FLT_MAX;
	shadowPayload.skipShading = true;
	TraceRay(g_BVH, RAY_FLAG_ACCEPT_FIRST_HIT_AND_END_SEARCH, 0xFF, 0, 1, 0, shadowRay, shadowPayload);

	shadow = 1.0f;
	if (shadowPayload.rayHitT != FLT_MAX)
		shadow = shadowValue;
#endif


	const float output = ao * shadow;

	const bool isOffscreen = reprojected.x < 0 || reprojected.x > g_Dynamic.screenSize.x ||
		reprojected.y < 0 || reprojected.y > g_Dynamic.screenSize.y;

	// Depth of current world position to previous frame's camera position
	const float depthToPrevCam = length(worldPos - g_Dynamic.previousCamPos.xyz);

	// Depth of current world position to current frame's camera position
	const float depthToCurrCam = length(worldPos - g_Dynamic.currentCamPos.xyz);

	// If this is the first frame or we're offscreen, we don't have any other data to work with
	// so we just spit out the raw output, in order to have *something* to work off next frame.
	if (g_Dynamic.frameCount == 0 || isOffscreen)
	{
		g_RTEffects[DispatchRaysIndex().xy] = float4(output, depthToCurrCam, 0, 1);
	}
	else
	{
		// Depth-based

		// Retrieve the results from the previous pass.
		float previous = g_RTEffects[DispatchRaysIndex().xy].r;
		float passCount = g_RTEffects[DispatchRaysIndex().xy].a;

		// If this is the first pass, we don't have anything in g_RTEffects yet, so we have to take the value from previous frame.
		if (g_Dynamic.isFirstPass)
		{
			previous = g_RTEffectsPrevious[reprojected.xy].r;
			passCount = g_RTEffectsPrevious[reprojected.xy].a;
		}

		// Handle disocclusion
		// image of what is happening: (slide 24) https://www.gdcvault.com/play/1022970/Temporal-Reprojection-Anti-Aliasing-in

		// test with an epsilon, for rounding errors.
		// Only do this on the first pass. otherwise it will result in very noisy results.
		if (g_Dynamic.currentPassIdx == 0 && abs(depthToPrevCam - g_RTEffectsPrevious[reprojected.xy].g) > depthToCurrCam / 1000.0f)
		{
			g_RTEffects[DispatchRaysIndex().xy] = float4(output, depthToCurrCam, 0, 1);
			return;
		}


		// Increment frame count, as we are now adding one additional value to the average.
		passCount += 1;

		// Running average
		// https://www.avrfreaks.net/comment/215641#comment-215641
		float pixelAvg = (previous * (passCount - 1) + output) / passCount;

		g_RTEffects[DispatchRaysIndex().xy] = float4(pixelAvg, depthToCurrCam, 0, passCount);
	}
}
