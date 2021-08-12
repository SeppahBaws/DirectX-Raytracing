#define HLSL
#include "DefaultPassCommon.h"
#include "../Util/RaytraceCommon.h"

cbuffer Material : register(b1)
{
	uint g_MaterialID;
}

SamplerState g_Sampler : register(s0);
Texture2D<float4> g_Diffuse : register(t4);
Texture2D<float4> g_Normal : register(t5);

[shader("closesthit")]
void DefaultClosestHit(inout HitInfo payload, in BuiltInTriangleIntersectionAttributes attributes)
{
	payload.rayHitT = RayTCurrent();
	if (payload.skipShading)
	{
		return;
	}

	RayTraceMeshInfo meshInfo = g_MeshInfo[g_MaterialID];

	float2 uv, ddxUV, ddyUV;
	float3 vsNormal, vsTangent, vsBitangent;

	GetMeshData(meshInfo, PrimitiveIndex(), attributes.barycentrics, DispatchRaysIndex().xy,
		uv, ddxUV, ddyUV, vsNormal, vsTangent, vsBitangent);

	// Diffuse color from texture
	const float3 diffuseColor = g_Diffuse.SampleGrad(g_Sampler, uv, ddxUV, ddyUV).rgb;

	const float3 worldPosition = WorldRayOrigin() + WorldRayDirection() * RayTCurrent();

	uint randSeed = InitRand(DispatchRaysIndex().x + DispatchRaysIndex().y * DispatchRaysDimensions().x, g_Dynamic.frameCount);
	const float3 aoDir = GetCosHemisphereSample(randSeed, vsNormal);

	// Ambient Occlusion
	const float aoTMax = 100;
	RayDesc aoRay;
	aoRay.TMin = 0.1f;
	aoRay.TMax = aoTMax;
	aoRay.Origin = worldPosition;
	aoRay.Direction = aoDir;
	HitInfo aoPayload;
	aoPayload.rayHitT = aoTMax;
	aoPayload.skipShading = true;
	TraceRay(g_BVH, RAY_FLAG_ACCEPT_FIRST_HIT_AND_END_SEARCH, 0xFF, 0, 1, 0, aoRay, aoPayload);
	
	const float ao = aoPayload.rayHitT / aoTMax;

	// Soft shadows
	const float3 shadowColor = float3(0.1f, 0.1f, 0.1f);
	const float3 shadowDir = float3(shadowSettings.shadowDirX, shadowSettings.shadowDirY, shadowSettings.shadowDirZ);
	const float3 randomOffset = GetRandomOnUnitSphere(randSeed) * shadowSettings.sphereSize;

	const float3 rayDir = normalize(normalize(shadowDir) * shadowSettings.sphereOffset + randomOffset);
	RayDesc shadowRay;
	shadowRay.TMin = 0.1f;
	shadowRay.TMax = FLT_MAX;
	shadowRay.Origin = worldPosition;
	shadowRay.Direction = rayDir;
	HitInfo shadowPayload;
	shadowPayload.rayHitT = FLT_MAX;
	shadowPayload.skipShading = true;
	TraceRay(g_BVH, RAY_FLAG_ACCEPT_FIRST_HIT_AND_END_SEARCH, 0xFF, 0, 1, 0, shadowRay, shadowPayload);

	float3 outputColor = diffuseColor * ao;

	if (shadowPayload.rayHitT != FLT_MAX)
		outputColor *= shadowColor;

	g_Output[DispatchRaysIndex().xy] = float4(outputColor, 0.0f);
}
