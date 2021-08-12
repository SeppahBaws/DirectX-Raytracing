#define HLSL
#include "NoiseMapCommon.h"
#include "../../Util/ShaderStructs.h"

RWTexture2D<float> g_Output : register(u0);
RWTexture2D<float> g_RayTraceEffects : register(u1);
RWTexture2D<float4> g_NormalsDepthBuffer : register(u2);

cbuffer b0 : register(b0)
{
	NoiseMapGenParams g_Params;
}

[numthreads(GEN_DISPATCH_SIZE, GEN_DISPATCH_SIZE, 1)]
void main(uint3 threadId : SV_DispatchThreadID)
{
	float avg = 0;
	int amount = 0;

	const int size = g_Params.averageSize;

	const float4 pixelNormDepth = g_NormalsDepthBuffer[threadId.xy];

	// Get the local average
	for (int y = -size; y < size; y++)
	{
		for (int x = -size; x < size; x++)
		{
			const float2 newUv = threadId.xy + float2(x, y);

			// We have to check if we're not off-screen
			if (newUv.x < g_Params.screenSize.x && newUv.y < g_Params.screenSize.y &&
				newUv.x > 0.0f && newUv.y > 0.0f)
			{
				const float4 newNormDepth = g_NormalsDepthBuffer[newUv];

				// if start pixel normal dot local pixel normal is smaller than 0.8 (e.g.) then we disregard this pixel?
				if (abs(pixelNormDepth.w - newNormDepth.w) < g_Params.depthTolerance &&
					dot(pixelNormDepth.xyz, newNormDepth.xyz) > g_Params.normalTolerance)
				{
					avg += g_RayTraceEffects[newUv];
					amount++;
				}
			}
		}
	}

	if (amount > 0.0f)
		avg /= amount;

	const float pixel = g_RayTraceEffects[threadId.xy];
	// The difference between the pixel's value and the local average.
	const float diff = pixel - avg;

	g_Output[threadId.xy] = diff;
}
