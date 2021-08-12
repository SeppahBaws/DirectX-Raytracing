#include "Structs.hlsli"

cbuffer VSConstants : register(b0)
{
	float4x4 modelToProjection;
};

VSOutput main(VSInput input)
{
	VSOutput output = (VSOutput)0;
	output.position = mul(modelToProjection, float4(input.position, 1.0f));
	output.uv = input.uv;
	return output;
}
