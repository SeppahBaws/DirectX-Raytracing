#define HLSL
#include "RaytraceCommon.h"

[shader("miss")]
void AOMiss(inout HitInfo payload)
{
	// Passing -1 to the alpha channel will signal the ray binning that it didn't hit anything (aka sky),
	// thus we don't need to try and shoot rays to this pixel
	g_Output[DispatchRaysIndex().xy] = float4(0.0f, 0.0f, 0.0f, -1.0f);
}
