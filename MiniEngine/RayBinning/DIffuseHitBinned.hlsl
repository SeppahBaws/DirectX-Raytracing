#define HLSL
#include "RaytraceCommon.h"
#include "RayBinningCommon.h"
#include "RaytracingHlslCompat.h"

// Local root signature
cbuffer Material : register(b2)
{
	uint g_MaterialID;
}

// Local root signature
SamplerState g_Sampler : register(s0);

// Local root signature
Texture2D<float4> g_TexDiffuse : register(t4);
Texture2D<float4> g_TexNormal : register(t5);

[shader("closesthit")]
void DiffuseClosestHitBinned(inout HitInfo payload, in BuiltInTriangleIntersectionAttributes attrib)
{
	payload.rayHitT = RayTCurrent();
}
