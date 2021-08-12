#pragma once

#include "../../Util/ShaderStructs.h"

#ifdef HLSL

RWTexture2D<float4> g_Normals : register(u0);
RWTexture2D<float4> g_BaseColors : register(u1);
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
