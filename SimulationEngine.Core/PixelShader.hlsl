#include "VertexToPixel.hlsli"

Texture2D Texture : register(t0);
SamplerState Sampler : register(s0);

float4 main( VertexToPixel input ) : SV_TARGET
{
    return Texture.Sample(Sampler, input.uv);
}