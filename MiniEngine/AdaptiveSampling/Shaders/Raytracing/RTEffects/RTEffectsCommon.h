#pragma once

#include "../../Util/ShaderStructs.h"

#ifdef HLSL

// G-buffer layout:
// R: averaged raytracing output
// G: counter for how many frames this pixel's average is
// B: depth

RWTexture2D<float4> g_RTEffects : register(u0);
RWTexture2D<float4> g_RTEffectsPrevious : register(u1);
RWTexture2D<float4> g_Normals : register(u2);
RWTexture2D<float> g_NoiseMap : register(u3);

RaytracingAccelerationStructure g_BVH : register(t0);

StructuredBuffer<RayTraceMeshInfo> g_MeshInfo : register(t1);
ByteAddressBuffer g_Indices : register(t2);
ByteAddressBuffer g_Attributes : register(t3);

cbuffer b0 : register(b0)
{
	DynamicCB g_Dynamic;
}

cbuffer LightSettings : register(b1)
{
	ShadowSettings shadowSettings;
}

#endif
