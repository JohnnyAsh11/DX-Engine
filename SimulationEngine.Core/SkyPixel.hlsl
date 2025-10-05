#include "VertexToPixel.hlsli"

TextureCube Texture : register(t0);
SamplerState Sampler : register(s0);

struct VertexToPixel_Sky
{
    float4 position : SV_POSITION;
    float3 sampleDir : DIRECTION;
};

float4 main(VertexToPixel_Sky input) : SV_TARGET
{
    return (Texture.Sample(Sampler, input.sampleDir) - float4(0.5f, 0.5f, 0.5f, 0.0f));
}