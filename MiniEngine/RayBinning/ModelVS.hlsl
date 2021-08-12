#include "Structs.hlsli"

cbuffer VSConstants : register(b0)
{
    float4x4 mvpMatrix;
    float3 viewPos;
}

VSOutput main(VSInput input)
{
    VSOutput output;

    output.position = mul(mvpMatrix, float4(input.position, 1.0f));
    output.uv = input.uv;
    output.normal = input.normal;
    output.tangent = input.tangent;
    output.bitangent = input.bitangent;

    return output;
}
