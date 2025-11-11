#include "VertexToPixel.hlsli"
#include "Lighting.hlsli"
#include "PhysicsTextureRender.hlsli"

Texture2D Albedo : register(t0);
Texture2D Normal : register(t1);
Texture2D Roughness : register(t2);
Texture2D Metal : register(t3);

SamplerState Sampler : register(s0);

cbuffer ExternalData : register(b4)
{
    // 16 byte memory padding rules applied:
    // - -
    float2 Offset;
    float2 Scale;
    // - -
    float3 CameraPosition;
    float padding;
    // - -
    Light Lights[MAX_LIGHT_COUNT];
    // - -
    float4 Color;
}

float4 main(VertexToPixel input) : SV_TARGET
{    
    return Color;
}