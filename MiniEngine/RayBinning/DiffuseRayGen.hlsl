#define HLSL
#include "RaytraceCommon.h"

[shader("raygeneration")]
void DiffuseRayGen()
{
	HitInfo payload;
	payload.rayHitT = 1.0f;
	payload.skipShading = false;

	// Get the location within the dispatched 2D grid of work items
	// (often maps to pixels, so this could represent a pixel coordinate).
	uint2 launchIndex = DispatchRaysIndex().xy;
	float2 dims = float2(DispatchRaysDimensions().xy);
	float2 d = (((launchIndex.xy + 0.5f) / dims.xy) * 2.f - 1.f);
	// Define a ray, consisting of origin, direction, and the min-max distance
	// values
	// #DXR Extra: Perspective Camera
	float aspectRatio = dims.x / dims.y;
	// Perspective
	RayDesc ray;
	ray.Origin = mul(g_Dynamic.viewI, float4(0, 0, 0, 1)).xyz;
	float4 target = mul(g_Dynamic.projectionI, float4(d.x, -d.y, 1, 1));
	ray.Direction = mul(g_Dynamic.viewI, float4(target.xyz, 0)).xyz;
	ray.TMin = 0;
	ray.TMax = FLT_MAX;

	TraceRay(sceneBVH, RAY_FLAG_CULL_BACK_FACING_TRIANGLES, ~0, 0, 1, 0, ray, payload);
}
