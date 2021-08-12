#define HLSL
#include "FinalCombineCommon.h"
#include "../../Util/ShaderStructs.h"

RWTexture2D<float3> g_Output : register(u0);
RWTexture2D<float3> g_Diffuse : register(u1);
RWTexture2D<float4> g_ShadowMap : register(u2);

RWTexture2D<float> g_NoiseMap : register(u3);

cbuffer Params : register(b0)
{
	FinalCombineParams g_Params;
}

[numthreads(COMBINE_DISPATCH_SIZE, COMBINE_DISPATCH_SIZE, 1)]
void main(uint3 DTid : SV_DispatchThreadID)
{
	if (g_Params.debugNoiseMap)
	{
		g_Output[DTid.xy] = float3(g_NoiseMap[DTid.xy], 0, 0);
	}
	else if (g_Params.debugRTEffects)
	{
		g_Output[DTid.xy] = g_ShadowMap[DTid.xy].r;
	}
	else
	{
		g_Output[DTid.xy] = g_Diffuse[DTid.xy] * g_ShadowMap[DTid.xy].r;
	}
}
