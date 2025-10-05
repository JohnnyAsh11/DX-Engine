#include "VertexToPixel.hlsli"
#include "Lighting.hlsli"
#include "PhysicsTextureRender.hlsli"

Texture2D Albedo : register(t0);
Texture2D Normal : register(t1);
Texture2D Roughness : register(t2);
Texture2D Metal : register(t3);

SamplerState Sampler : register(s0);

cbuffer ExternalData : register(b0)
{
    // 16 byte memory padding rules applied:
    // - -
    float3 CameraPosition;
    float padding;
    // - -
    Light Lights[MAX_LIGHT_COUNT];
}

float4 main( VertexToPixel input ) : SV_TARGET
{
    // Making sure that the normals are normalized and that the ambient is black.
    input.normal = normalize(input.normal);
    
    // Getting the surface/albedo color from the Albedo texture.
    float3 albedoColor = pow(Albedo.Sample(Sampler, input.uv).xyz, 2.2f);
    
    // unpacking the normal map and setting its value.
    float3 unpackedNormal = Normal.Sample(Sampler, input.uv).rgb * 2 - 1;
    unpackedNormal = normalize(unpackedNormal);
    float3 N = normalize(input.normal);
    float3 T = normalize(input.tangent);
    T = normalize(T - N * dot(T, N));
    float3 B = cross(T, N);
    float3x3 TBN = float3x3(T, B, N);
    input.normal = mul(unpackedNormal, TBN);
    
    // Using only the red color channel from both textures.
    // Getting the roughness value from the roughness map.
    float roughness = Roughness.Sample(Sampler, input.uv).r;
    // Getting the metalness valuse from the metal map.
    float metalness = Metal.Sample(Sampler, input.uv).r;
    
    float3 specularColor = lerp(F0_NON_METAL, albedoColor.rgb, metalness);
    float3 total = 0.0f;
    float3 toCamera = normalize(CameraPosition - input.worldPos);
    for (int i = 0; i < MAX_LIGHT_COUNT; i++)
    {
        Light currentLight = Lights[i];
        
        // Calculating different light amounts.
        float3 fFresnel;
        float3 toLight = normalize(currentLight.Position - input.worldPos);
        float diff = DiffusePBR(input.normal, toLight);
        float3 PBR = MicrofacetBRDF(
                    input.normal,
                    toLight,
                    toCamera,
                    roughness,
                    specularColor,
                    fFresnel);
        
        // Calculate diffuse with energy conservation, including cutting diffuse for metals
        float3 balancedDiff = DiffuseEnergyConserve(diff, fFresnel, metalness);
        
        // Combing the total afflicted lighting.
        total += (balancedDiff * albedoColor + PBR) * currentLight.Intensity * currentLight.Color;
    }
    
    return pow(float4(total, 1.0f), 1 / 2.2f);
}