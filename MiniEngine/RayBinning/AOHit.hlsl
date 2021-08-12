#define HLSL
#include "RaytraceCommon.h"

cbuffer Material : register(b1)
{
	uint g_MaterialID;
}

[shader("closesthit")]
void AOClosestHit(inout HitInfo payload, in BuiltInTriangleIntersectionAttributes attrib)
{
	payload.rayHitT = RayTCurrent();
	if (payload.skipShading)
	{
		return;
	}

	const uint2 launchIdx = DispatchRaysIndex().xy;
	
	RayTraceMeshInfo meshInfo = g_MeshInfo[g_MaterialID];
	
	float2 uv, ddxUV, ddyUV;
	float3 vertexNormal;

	GetMeshData(meshInfo, PrimitiveIndex(), attrib.barycentrics, DispatchRaysIndex().xy,
		uv, ddxUV, ddyUV, vertexNormal);
	
	const float3 worldPosition = WorldRayOrigin() + WorldRayDirection() * RayTCurrent();
	
	uint randSeed = InitRand(
		DispatchRaysIndex().x + DispatchRaysIndex().y * DispatchRaysDimensions().x,
		g_Dynamic.frameCount, 16);

	const float3 aoDir = GetCosHemisphereSample(randSeed, vertexNormal);


	// Output directions to the directions buffer
	// RGB: direction
	// A: distance
	g_Output[launchIdx] = float4(aoDir, RayTCurrent());
}