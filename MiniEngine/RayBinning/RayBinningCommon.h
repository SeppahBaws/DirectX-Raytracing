#pragma once

#ifndef HLSL
#include "HlslCompat.h"
#endif

struct RayBinningInfo
{
	float2 screenSize;
	uint rayBinTileCountX;

	// Padding for alignment.
	float4x4 padding;
};

#ifdef HLSL
#define RAY_BINNING_TILE_SIZE 16

#ifndef RAY_BINNING_COMMON_NO_ROOT_PARAMS
// These root parameters are part of the global root signature,
// however due to my shitty code organization you have to both include RaytraceCommon.h
// and RayBinningCommon.h :)
// Might change in the future, but it's just test for now so who cares

Texture2D<float4> g_AODirections : register(t6);

RWStructuredBuffer<uint> g_RayBinResult : register(u10);
RWStructuredBuffer<uint> g_RayBinSizeResult : register(u11);

cbuffer Constants : register(b1)
{
	RayBinningInfo input;
}
#endif

#endif
