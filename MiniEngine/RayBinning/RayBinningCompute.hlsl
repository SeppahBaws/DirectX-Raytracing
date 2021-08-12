#define HLSL
#include "Packing.hlsli"
#define RAY_BINNING_COMMON_NO_ROOT_PARAMS
#include "RayBinningCommon.h"

RWTexture2D<float4> g_AODirections : register(u0);

RWStructuredBuffer<uint> g_RayBinResult : register(u1);
RWStructuredBuffer<uint> g_RayBinSizeResult : register(u2);

cbuffer Constants : register(b0)
{
    RayBinningInfo g_Info;
}

groupshared uint gs_binSize[RAY_BINNING_TILE_SIZE * RAY_BINNING_TILE_SIZE + 1];
groupshared uint gs_binOffset[RAY_BINNING_TILE_SIZE * RAY_BINNING_TILE_SIZE];

[numthreads(RAY_BINNING_TILE_SIZE, RAY_BINNING_TILE_SIZE, 1)]
void RayBinning(uint2 groupThreadId : SV_GroupThreadID, uint2 groupId : SV_GroupID)
{
    const uint2 currentCoord = groupId * RAY_BINNING_TILE_SIZE + groupThreadId;

    // Initialize the invalid counter to 0
    if (groupThreadId.x == 0 && groupThreadId.y == 0)
    {
        gs_binSize[256] = 0;
    }
    
    // Every tile initializes a counter to 0
    gs_binSize[groupThreadId.y * RAY_BINNING_TILE_SIZE + groupThreadId.x] = 0;
    
    GroupMemoryBarrierWithGroupSync();
    
    // is this sample valid
    uint binIndex = 256;
    
    // Is this sample valid? otherwise its bin index is 256
    const float4 sampled = g_AODirections[currentCoord];
    if (sampled.w > 0.0 && (float)currentCoord.x < g_Info.screenSize.x && (float)currentCoord.y < g_Info.screenSize.y)
    {
        const float3 currentDirection = sampled.xyz;
    
        const float2 octahedralDirection = PackOctahedral(currentDirection);
        const uint2 binCoordinates = (uint2)((octahedralDirection * 0.5 + 0.5) * RAY_BINNING_TILE_SIZE);
        binIndex = binCoordinates.y * RAY_BINNING_TILE_SIZE + binCoordinates.x;
    }
    
    // Increment the bin size of the bin where this sample goes
    int rayBinIndex = 0;
    if (binIndex != 256)
    {
        InterlockedAdd(gs_binSize[binIndex], 1, rayBinIndex);
    }
    
    GroupMemoryBarrierWithGroupSync();
    
    if (groupThreadId.x == 0 && groupThreadId.y == 0)
    {
        gs_binOffset[0] = 0;
        for (int i = 1; i < 256; ++i)
        {
            gs_binOffset[i] = gs_binOffset[i - 1] + gs_binSize[i - 1];
        }
    }
    
    GroupMemoryBarrierWithGroupSync();
    
    // Store the binning results
    if (binIndex < 256)
    {
        const uint groupIndex = groupId.y * g_Info.rayBinTileCountX + groupId.x;
        const uint globalOffset = groupIndex * RAY_BINNING_TILE_SIZE * RAY_BINNING_TILE_SIZE + gs_binOffset[binIndex] + rayBinIndex;
        g_RayBinResult[globalOffset] = ((currentCoord.x & 0xffff) << 16) + (currentCoord.y & 0xffff);
    }
    
    // Output the size of every bin
    if (groupThreadId.x == 0 && groupThreadId.y == 0)
    {
        const uint groupIndex = groupId.x * g_Info.rayBinTileCountX + groupId.x;
        g_RayBinSizeResult[groupIndex] = gs_binOffset[255] + gs_binSize[255];
    }
}
