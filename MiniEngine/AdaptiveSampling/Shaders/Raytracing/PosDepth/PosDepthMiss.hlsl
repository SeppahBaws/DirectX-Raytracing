#define HLSL
#include "PosDepthCommon.h"

[shader("miss")]
void PosDepthMiss(inout HitInfo payload : SV_RayPayload)
{
	g_Normals[DispatchRaysIndex().xy] = float4(0.0f, 0.0f, 0.0f, -1.0f);
	g_BaseColors[DispatchRaysIndex().xy] = float4(0.0f, 0.0f, 0.0f, 0.0f);
}
