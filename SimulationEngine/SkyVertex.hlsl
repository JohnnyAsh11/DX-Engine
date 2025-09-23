#include "VertexToPixel.hlsli"

struct VertexShaderInput
{
    float3 position : POSITION;
    float3 normal : NORMAL;
    float2 uv : UV;
    float3 tangent : TANGENT;
    float4 color : COLOR;
};

struct VertexToPixel_Sky
{
    float4 position : SV_POSITION;
    float3 sampleDir : DIRECTION;
};

cbuffer ExternalData : register(b0)
{
    matrix view;
    matrix projection;
}

VertexToPixel_Sky main(VertexShaderInput input)
{
    VertexToPixel_Sky output;
    
    matrix viewNoTranslation = view;
    viewNoTranslation._14 = 0;
    viewNoTranslation._24 = 0;
    viewNoTranslation._34 = 0;
    matrix viewProj = mul(projection, viewNoTranslation);
    
    output.position = mul(viewProj, float4(input.position, 1.0f));
    output.position.z = output.position.w;
    output.sampleDir = input.position;
    
    return output;
}