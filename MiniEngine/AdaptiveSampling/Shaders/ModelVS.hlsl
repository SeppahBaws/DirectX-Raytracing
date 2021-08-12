#include "Structs.hlsli"

cbuffer VSConstants : register(b0)
{
	float4x4 viewProjection;
	float4x4 model;
}

VSOutput main(VSInput input)
{
	VSOutput output = (VSOutput)0;
	float4x4 mvp = mul(viewProjection, model);
	output.position = mul(mvp, float4(input.position, 1.0f));
	output.uv = input.uv;
	output.normal = input.normal;
	output.tangent = input.tangent;
	output.bitangent = input.bitangent;

	return output;
}
