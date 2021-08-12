#define HLSL
#include "RaytraceCommon.h"
#include "RayBinningCommon.h"

[shader("raygeneration")]
void DiffuseRayGenBinned()
{
	const float3 skyColor = float3(0.0f, 0.0f, 0.0f);

	// Current dispatch index of current raytrace shader
	const uint3 launchIndex = DispatchRaysIndex();

	// launchIndex is 1-dimensional, because we loop over the g_RayBinResult array

	// Pixel coordinate of current pixel
	const uint2 currentBinIndex = uint2(launchIndex.x % input.screenSize.x, launchIndex.x / input.screenSize.x);

	const uint2 localTileCoordinate = uint2(currentBinIndex.x % RAY_BINNING_TILE_SIZE, currentBinIndex.y % RAY_BINNING_TILE_SIZE);
	const uint localTileIndex = localTileCoordinate.y * RAY_BINNING_TILE_SIZE + localTileCoordinate.x;
	const uint2 tileCoordinate = uint2(currentBinIndex.x / RAY_BINNING_TILE_SIZE, currentBinIndex.y / RAY_BINNING_TILE_SIZE);
	const uint tileIndex = tileCoordinate.y * input.rayBinTileCountX + tileCoordinate.x;

	// Read and unpack coordinate to screen space coordinates
	const uint globalBinIndex = tileIndex * RAY_BINNING_TILE_SIZE * RAY_BINNING_TILE_SIZE + localTileIndex;
	const uint packedPixelCoordinate = g_RayBinResult[globalBinIndex];
	const uint2 currentPixelCoord = uint2((packedPixelCoordinate & 0xffff0000) >> 16, packedPixelCoordinate & 0xffff);

	// Clear the render target
	g_Output[currentBinIndex] = float4(skyColor, 1.0f);

	// If local index of this pixel is beyond the valid count, we don't need to calculate it
	if (localTileIndex > g_RayBinSizeResult[tileIndex])
	{
		// g_Output[currentPixelCoord] = float4(1.0f, 0.0f, 1.0f, 1.0f);
		g_Output[currentBinIndex] = float4(1.0f, 0.0f, 1.0f, 1.0f);
		return;
	}

	// Launch a ray into the scene
	float2 dims = float2(input.screenSize.xy);
	float2 d = (((currentBinIndex.xy + 0.5f) / dims.xy) * 2.f - 1.f);
	// float2 d = (((currentPixelCoord.xy + 0.5f) / dims.xy) * 2.f - 1.f);

	const float4 target = mul(g_Dynamic.projectionI, float4(d.x, -d.y, 1, 1));

	// Current pixel world position and direction
	const float3 pixelWorldPos = mul(g_Dynamic.viewI, float4(0, 0, 0, 1)).xyz;
	const float3 pixelWorldDir = mul(g_Dynamic.viewI, float4(target.xyz, 0)).xyz;

	// AO ray world position and direction
	const float3 aoWorldPos = pixelWorldPos + pixelWorldDir * g_AODirections[currentBinIndex].w;
	const float3 aoWorldDir = g_AODirections[currentBinIndex].xyz;

	// Check if this pixel hit anything. AOMiss will write a negative value to the alpha channel,
	// indicating that we can skip this pixel.
	if (g_AODirections[currentBinIndex].w < 0.0f)
	{
		g_Output[currentBinIndex] = float4(skyColor, 1.0f);
		return;
	}

	// Trace ambient occlusion ray.
	const float aoTMax = 100;
	RayDesc ray;
	ray.TMin = 0.1f;
	ray.TMax = aoTMax;
	ray.Origin = aoWorldPos;
	ray.Direction = aoWorldDir;
	HitInfo payload;
	payload.rayHitT = aoTMax;
	payload.skipShading = true;
	TraceRay(sceneBVH, RAY_FLAG_ACCEPT_FIRST_HIT_AND_END_SEARCH, ~0, 0, 1, 0, ray, payload);

	const float ao = payload.rayHitT / aoTMax;

	g_Output[currentBinIndex] = float4(ao, ao, ao, 1.0f);
}
