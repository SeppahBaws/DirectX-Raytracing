#define HLSL
#include "RTEffectsCommon.h"

[shader("closesthit")]
void RTEffectsClosestHit(inout HitInfo payload, in BuiltInTriangleIntersectionAttributes attributes)
{
	payload.rayHitT = RayTCurrent();
}
