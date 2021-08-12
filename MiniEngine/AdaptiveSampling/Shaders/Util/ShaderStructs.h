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

// Matrices for the ray generation
struct DynamicCB
{
	float4x4 viewI;
	float4x4 projectionI;

	float4x4 previousViewProj;

	float4 previousCamPos;
	float4 currentCamPos;

	// used to see the pseudo-random number generator
	uint frameCount;

	bool isFirstPass;
	uint minimumPassCount;
	uint maximumPassCount;
	float varianceToleration;
	uint currentPassIdx;
	float2 screenSize;
};

struct ShadowSettings
{
	float sphereOffset;
	float sphereSize;
	float shadowDirX;
	float shadowDirY;
	float shadowDirZ;

	// MiniEngine being annoying :)
	float4x4 padding;
};

struct NoiseMapGenParams
{
	int averageSize;
	float rTolerance;
	float gTolerance;
	float edgeTolerance;
	float depthTolerance;
	float normalTolerance;
	float2 screenSize;

	// MiniEngine being annoying :)
	float4x4 padding;
};

struct FinalCombineParams
{
	bool debugNoiseMap;
	uint _p1; // padding
	bool debugRTEffects;
	uint _p2; // padding
	bool debugRTOffsets;

	// MiniEngine being annoying :)
	float4x4 padding;
};
