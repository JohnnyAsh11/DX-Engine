#include "VertexToPixel.hlsli"

cbuffer ExternalData : register(b0)
{
	// - 16 byte spacing rules -
    matrix world;
	// - -
    matrix worldInvTranspose;
	// - -
    matrix view;
	// - -
    matrix projection;
}

struct VertexShaderInput
{
    float3 position : POSITION; 
    float3 normal : NORMAL;
    float2 uv : UV;
    float3 tangent : TANGENT;
    float4 color : COLOR;
};

VertexToPixel main( VertexShaderInput input )
{
    VertexToPixel output;
    
    matrix wvp = mul(world, mul(view, projection));
    
    output.uv = input.uv;
    output.color = input.color;
    output.screenPosition = mul(wvp, float4(input.position, 1.0f));
    output.worldPos = mul(world, float4(input.position, 1.0f)).xyz;
    output.tangent = normalize(mul((float3x3) world, input.tangent));
    output.normal = normalize(mul((float3x3) worldInvTranspose, input.normal));
    
	return output;
}