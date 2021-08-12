#define HLSL
#include "NoiseMapCommon.h"
#include "../../Util/ShaderStructs.h"

RWTexture2D<float> g_Output : register(u0);

cbuffer b0 : register(b0)
{
	NoiseMapGenParams g_Params;
}

[numthreads(BLUR_DISPATCH_SIZE, BLUR_DISPATCH_SIZE, 1)]
void main(uint3 threadId : SV_DispatchThreadID )
{
	const int size = g_Params.averageSize;

	// do blur on output, sampling the output :)
	float blurSum = 0.0f;
	for (int y = -size * 2; y < size * 2; y++)
	{
		for (int x = -size * 2; x < size * 2; x++)
		{
			const float2 newUv = threadId.xy + float2(x, y);

			// We have to check if we're not off-screen
			if (newUv.x < g_Params.screenSize.x && newUv.y < g_Params.screenSize.y &&
				newUv.x > 0.0f && newUv.y > 0.0f)
			{
				blurSum += g_Output[newUv];
			}
		}
	}

	g_Output[threadId.xy] = blurSum > 0.1 ? 1 : 0;
}
