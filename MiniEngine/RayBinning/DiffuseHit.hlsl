#define HLSL
#include "RaytraceCommon.h"
#include "RaytracingHlslCompat.h"

// Local root signature
cbuffer Material : register(b1)
{
	uint g_MaterialID;
}

// Local root signature
SamplerState g_Sampler : register(s0);

// Local root signature
Texture2D<float4> g_TexDiffuse : register(t4);
Texture2D<float4> g_TexNormal : register(t5);

[shader("closesthit")]
void DiffuseClosestHit(inout HitInfo payload, in BuiltInTriangleIntersectionAttributes attrib)
{
	payload.rayHitT = RayTCurrent();
	if (payload.skipShading)
	{
		return;
	}

	float3 outputColor = float3(0.0f, 0.0f, 0.0f);

	RayTraceMeshInfo meshInfo = g_MeshInfo[g_MaterialID];

	float2 uv, ddxUV, ddyUV;
	float3 vsNormal;

	GetMeshData(meshInfo, PrimitiveIndex(), attrib.barycentrics, DispatchRaysIndex().xy,
		uv, ddxUV, ddyUV, vsNormal);
	
	const float3 worldPosition = WorldRayOrigin() + WorldRayDirection() * RayTCurrent();
	
	const float3 diffuseColor = g_TexDiffuse.SampleGrad(g_Sampler, uv, ddxUV, ddyUV).rgb;
	// const float3 normalColor = g_TexNormal.SampleGrad(g_Sampler, uv, ddxUV, ddyUV).rgb * 2.0f - 1.0f;

	uint randSeed = InitRand(
		DispatchRaysIndex().x + DispatchRaysIndex().y * DispatchRaysDimensions().x,
		g_Dynamic.frameCount,
		16);

	float3 aoDir = GetCosHemisphereSample(randSeed, vsNormal);
	
	// Trace ambient occlusion ray.
	const float aoTMax = 100;
	RayDesc aoRay;
	aoRay.TMin = 0.1f;
	aoRay.TMax = aoTMax;
	aoRay.Origin = worldPosition;
	aoRay.Direction = aoDir;
	HitInfo aoPayload;
	aoPayload.rayHitT = aoTMax;
	aoPayload.skipShading = true;
	TraceRay(sceneBVH, RAY_FLAG_ACCEPT_FIRST_HIT_AND_END_SEARCH, 0xFF, 0, 1, 0, aoRay, aoPayload);

	const float ao = aoPayload.rayHitT / aoTMax;

	// outputColor = ao * diffuseColor;
	outputColor = ao;

	g_Output[DispatchRaysIndex().xy] = float4(outputColor, 1.0f);
}
