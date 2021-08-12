#pragma once

#ifndef HLSL
#include "HlslCompat.h"
#endif

#include "RaytracingHlslCompat.h"

// Ray payload
struct HitInfo
{
	bool skipShading;
	float rayHitT;
};

// Matrices for our ray generation
struct DynamicCB
{
	float4x4 view;
	float4x4 projection;
	float4x4 viewI;
	float4x4 projectionI;

	// Used to seed the pseudo-random number generator
	uint frameCount;
};

#ifdef HLSL

#ifndef SINGLE
static const float FLT_MAX = asfloat(0x7F7FFFFF);
#endif

RWTexture2D<float4> g_Output : register(u0);

RaytracingAccelerationStructure sceneBVH : register(t0);

StructuredBuffer<RayTraceMeshInfo> g_MeshInfo : register(t1);
ByteAddressBuffer g_Indices : register(t2);
ByteAddressBuffer g_Attributes : register(t3);

cbuffer b0 : register(b0)
{
	DynamicCB g_Dynamic;
};

void GenerateCameraRay(uint2 index, out float3 origin, out float3 direction)
{
	float2 dims = float2(DispatchRaysDimensions().xy);
	float2 d = (((index.xy + 0.5f) / dims.xy) * 2.f - 1.f);

	origin = mul(g_Dynamic.viewI, float4(0, 0, 0, 1)).xyz;
	float4 target = mul(g_Dynamic.projectionI, float4(d.x, -d.y, 1, 1));
	direction = mul(g_Dynamic.viewI, float4(target.xyz, 0)).xyz;
}

uint3 Load3x16BitIndices(uint offsetBytes)
{
	const uint dwordAlignedOffset = offsetBytes & ~3;

	const uint2 four16BitIndices = g_Indices.Load2(dwordAlignedOffset);

	uint3 indices;

	if (dwordAlignedOffset == offsetBytes)
	{
		indices.x = four16BitIndices.x & 0xffff;
		indices.y = (four16BitIndices.x >> 16) & 0xffff;
		indices.z = four16BitIndices.y & 0xffff;
	}
	else
	{
		indices.x = (four16BitIndices.x >> 16) & 0xffff;
		indices.y = four16BitIndices.y & 0xffff;
		indices.z = (four16BitIndices.y >> 16) & 0xffff;
	}

	return indices;
}

float2 GetUVAttribute(uint byteOffset)
{
	return asfloat(g_Attributes.Load2(byteOffset));
}

/*
	REF: https://gamedev.stackexchange.com/questions/23743/whats-the-most-efficient-way-to-find-barycentric-coordinates
	From "Real-Time Collision Detection" by Christer Ericson
*/
float3 BarycentricCoordinates(float3 pt, float3 v0, float3 v1, float3 v2)
{
	float3 e0 = v1 - v0;
	float3 e1 = v2 - v0;
	float3 e2 = pt - v0;
	float d00 = dot(e0, e0);
	float d01 = dot(e0, e1);
	float d11 = dot(e1, e1);
	float d20 = dot(e2, e0);
	float d21 = dot(e2, e1);
	float denom = 1.0 / (d00 * d11 - d01 * d01);
	float v = (d11 * d20 - d01 * d21) * denom;
	float w = (d00 * d21 - d01 * d20) * denom;
	float u = 1.0 - v - w;
	return float3(u, v, w);
}

float3 RayPlaneIntersection(float3 planeOrigin, float3 planeNormal, float3 rayOrigin, float3 rayDirection)
{
	float t = dot(-planeNormal, rayOrigin - planeOrigin) / dot(planeNormal, rayDirection);
	return rayOrigin + rayDirection * t;
}

void GetMeshData(in RayTraceMeshInfo meshInfo, in uint primitiveIdx, in float2 barycentrics, in float2 threadID,
	out float2 uv, out float2 ddxUV, out float2 ddyUV, out float3 normal)
{
	const uint3 ii = Load3x16BitIndices(meshInfo.m_IndexOffsetBytes + primitiveIdx * 3 * 2);
	
	const float2 uv0 = GetUVAttribute(meshInfo.m_UVAttributeOffsetBytes + ii.x * meshInfo.m_AttributeStrideBytes);
	const float2 uv1 = GetUVAttribute(meshInfo.m_UVAttributeOffsetBytes + ii.y * meshInfo.m_AttributeStrideBytes);
	const float2 uv2 = GetUVAttribute(meshInfo.m_UVAttributeOffsetBytes + ii.z * meshInfo.m_AttributeStrideBytes);

	// Get UV coordinates from barycentric coordinates
	const float3 bary = float3(1.0 - barycentrics.x - barycentrics.y, barycentrics.x, barycentrics.y);
	uv = bary.x * uv0 + bary.y * uv1 + bary.z * uv2;

	// Get vertex normals
	const float3 normal0 = asfloat(g_Attributes.Load3(meshInfo.m_NormalAttributeOffsetBytes + ii.x * meshInfo.m_AttributeStrideBytes));
	const float3 normal1 = asfloat(g_Attributes.Load3(meshInfo.m_NormalAttributeOffsetBytes + ii.y * meshInfo.m_AttributeStrideBytes));
	const float3 normal2 = asfloat(g_Attributes.Load3(meshInfo.m_NormalAttributeOffsetBytes + ii.z * meshInfo.m_AttributeStrideBytes));
	normal = normalize(normal0 * bary.x + normal1 * bary.y + normal2 * bary.z);

	// Get the vertex positions
	const float3 p0 = asfloat(g_Attributes.Load3(meshInfo.m_PositionAttributeOffsetBytes + ii.x * meshInfo.m_AttributeStrideBytes));
	const float3 p1 = asfloat(g_Attributes.Load3(meshInfo.m_PositionAttributeOffsetBytes + ii.y * meshInfo.m_AttributeStrideBytes));
	const float3 p2 = asfloat(g_Attributes.Load3(meshInfo.m_PositionAttributeOffsetBytes + ii.z * meshInfo.m_AttributeStrideBytes));


	const float3 worldPosition = WorldRayOrigin() + WorldRayDirection() * RayTCurrent();

	// Compute partial derivatives of UV coodinates
	float3 ddxOrigin, ddxDir, ddyOrigin, ddyDir;
	GenerateCameraRay(uint2(threadID.x + 1, threadID.y), ddxOrigin, ddxDir);
	GenerateCameraRay(uint2(threadID.x, threadID.y + 1), ddyOrigin, ddyDir);

	// Plane normal
	const float3 triangleNormal = normalize(cross(p2 - p0, p1 - p2));

	// Intersect helper rays
	const float3 xOffsetPoint = RayPlaneIntersection(worldPosition, triangleNormal, ddxOrigin, ddxDir);
	const float3 yOffsetPoint = RayPlaneIntersection(worldPosition, triangleNormal, ddyOrigin, ddyDir);

	// Compute barycentrics
	const float3 baryX = BarycentricCoordinates(xOffsetPoint, p0, p1, p2);
	const float3 baryY = BarycentricCoordinates(yOffsetPoint, p0, p1, p2);

	// Compute UVs and take the difference
	const float3x2 uvMat = float3x2(uv0, uv1, uv2);
	ddxUV = mul(baryX, uvMat) - uv;
	ddyUV = mul(baryY, uvMat) - uv;
}

// Generates a seed for a random number generator from 2 inputs plus a backoff
// credit: Chris Wyman, from tutorial: http://cwyman.org/code/dxrTutors/tutors/Tutor5/tutorial05.md.html
uint InitRand(uint val0, uint val1, uint backoff = 16)
{
	uint v0 = val0, v1 = val1, s0 = 0;

	[unroll]
	for (uint n = 0; n < backoff; n++)
	{
		s0 += 0x9e3779b9;
		v0 += ((v1 << 4) + 0xa341316c) ^ (v1 + s0) ^ ((v1 >> 5) + 0xc8013ea4);
		v1 += ((v0 << 4) + 0xad90777d) ^ (v0 + s0) ^ ((v0 >> 5) + 0x7e95761e);
	}
	return v0;
}

// Takes our seed, updates it, and returns a pseudorandom float in [0..1]
// credit: Chris Wyman, from tutorial: http://cwyman.org/code/dxrTutors/tutors/Tutor5/tutorial05.md.html
float NextRand(inout uint s)
{
	s = (1664525u * s + 1013904223u);
	return float(s & 0x00FFFFFF) / float(0x01000000);
}

// Utility function to get a vector perpendicular to an input vector 
//    (from "Efficient Construction of Perpendicular Vectors Without Branching")
// credit: Chris Wyman, from tutorial: http://cwyman.org/code/dxrTutors/tutors/Tutor5/tutorial05.md.html
float3 GetPerpendicularVector(float3 u)
{
	float3 a = abs(u);
	uint xm = ((a.x - a.y) < 0 && (a.x - a.z) < 0) ? 1 : 0;
	uint ym = (a.y - a.z) < 0 ? (1 ^ xm) : 0;
	uint zm = 1 ^ (xm | ym);
	return cross(u, float3(xm, ym, zm));
}

// Get a cosine-weighted random vector centered around a specified normal direction.
// credit: Chris Wyman, from tutorial: http://cwyman.org/code/dxrTutors/tutors/Tutor5/tutorial05.md.html
float3 GetCosHemisphereSample(inout uint randSeed, float3 hitNorm)
{
	// Get 2 random numbers to select our sample with
	float2 randVal = float2(NextRand(randSeed), NextRand(randSeed));

	// Cosine weighted hemisphere sample from RNG
	float3 bitangent = GetPerpendicularVector(hitNorm);
	float3 tangent = cross(bitangent, hitNorm);
	float r = sqrt(randVal.x);
	float phi = 2.0f * 3.14159265f * randVal.y;

	// Get our cosine-weighted hemisphere lobe sample direction
	return tangent * (r * cos(phi).x) + bitangent * (r * sin(phi)) + hitNorm.xyz * sqrt(1 - randVal.x);
}

#endif
