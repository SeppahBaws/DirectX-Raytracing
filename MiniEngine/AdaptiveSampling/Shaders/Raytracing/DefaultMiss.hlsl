#define HLSL
#include "DefaultPassCommon.h"

[shader("miss")]
void DefaultMiss(inout HitInfo payload : SV_RayPayload)
{
	g_Output[DispatchRaysIndex().xy] = float4(0.0f, 0.0f, 0.0f, 1.0f);
}
