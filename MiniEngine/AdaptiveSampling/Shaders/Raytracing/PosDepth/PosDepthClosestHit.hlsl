#define HLSL
#include "PosDepthCommon.h"
#include "../../Util/RaytraceCommon.h"

cbuffer Material : register(b2)
{
	uint g_MaterialID;
}

SamplerState g_Sampler : register(s0);
Texture2D<float4> g_Diffuse : register(t4);
Texture2D<float4> g_Normal : register(t5);

[shader("closesthit")]
void PosDepthClosestHit(inout HitInfo payload, in BuiltInTriangleIntersectionAttributes attributes)
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
	const float4 diffuseColor = g_Diffuse.SampleGrad(g_Sampler, uv, ddxUV, ddyUV).rgba;
	
	g_Normals[DispatchRaysIndex().xy] = float4(vsNormal, RayTCurrent());
	g_BaseColors[DispatchRaysIndex().xy] = diffuseColor;
}
