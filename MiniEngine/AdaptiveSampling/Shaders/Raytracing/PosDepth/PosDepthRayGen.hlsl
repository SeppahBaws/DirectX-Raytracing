#define HLSL
#include "PosDepthCommon.h"
#include "../../Util/RaytraceCommon.h"

[shader("raygeneration")]
void PosDepthRayGen()
{
	HitInfo payload;
	payload.rayHitT = 1.0f;
	payload.skipShading = false;

	const uint2 launchIndex = DispatchRaysIndex().xy;
	const float2 dims = float2(DispatchRaysDimensions().xy);
	float2 d = (((launchIndex.xy + 0.5f) / dims.xy) * 2.0f - 1.0f);

	RayDesc ray;
	ray.Origin = mul(g_Dynamic.viewI, float4(0.0f, 0.0f, 0.0f, 1.0f)).xyz;
	float4 target = mul(g_Dynamic.projectionI, float4(d.x, -d.y, 1.0f, 1.0f));
	ray.Direction = mul(g_Dynamic.viewI, float4(target.xyz, 0.0f)).xyz;
	ray.TMin = 0;
	ray.TMax = FLT_MAX;

	TraceRay(g_BVH, RAY_FLAG_CULL_BACK_FACING_TRIANGLES, 0xFF, 0, 1, 0, ray, payload);
}