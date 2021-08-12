#include "Structs.hlsli"

Texture2D<float4> texDiffuse : register(t0);
SamplerState sampler0 : register(s0);

void main(VSOutput input)
{
	if (texDiffuse.Sample(sampler0, input.uv).a < 0.5)
	    discard;
}
