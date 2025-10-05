#include "VertexToPixel.hlsli"

Texture2D Texture : register(t0);
SamplerState Sampler : register(s0);

float4 main( VertexToPixel input ) : SV_TARGET
{
    return float4(input.uv, 0.5f, 1.0f);
    //return Texture.Sample(Sampler, input.uv);
}