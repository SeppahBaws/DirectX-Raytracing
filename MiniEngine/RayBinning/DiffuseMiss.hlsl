#define HLSL
#include "RaytraceCommon.h"

[shader("miss")]
void DiffuseMiss(inout HitInfo payload : SV_RayPayload)
{
	g_Output[DispatchRaysIndex().xy] = float4(0.0f, 0.0f, 0.0f, 1.0f);
}
