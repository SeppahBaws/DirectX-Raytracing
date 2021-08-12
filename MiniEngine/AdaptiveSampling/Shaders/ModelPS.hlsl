#include "Structs.hlsli"

Texture2D<float4> texDiffuse : register(t0);
Texture2D<float3> texSpecular : register(t1);
Texture2D<float4> texEmissive : register(t2);
Texture2D<float3> texNormal : register(t3);
Texture2D<float4> texLightmap : register(t4);
Texture2D<float4> texReflection : register(t5);

SamplerState textureSampler : register(s0);

float4 main(VSOutput input) : SV_TARGET
{
	float4 diffuse = texDiffuse.Sample(textureSampler, input.uv);

	float3 finalColor = diffuse.rgb;

	return float4(finalColor, 1.0f);
}
