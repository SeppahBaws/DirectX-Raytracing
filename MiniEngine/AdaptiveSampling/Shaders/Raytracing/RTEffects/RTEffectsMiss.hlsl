#define HLSL
#include "RTEffectsCommon.h"

[shader("miss")]
void RTEffectsMiss(inout HitInfo payload : SV_RayPayload)
{
	// payload.rayHitT = -1.0f;
}
