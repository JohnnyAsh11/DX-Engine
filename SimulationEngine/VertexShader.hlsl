#include "VertexToPixel.hlsli"
#include "VertexInput.hlsli"

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

VertexToPixel main( VertexShaderInput input )
{
    VertexToPixel output;
    
    matrix wvp = mul(projection, mul(view, world));
    //matrix wvp = mul(world, mul(view, projection));
    //matrix wvp = mul(view, mul(world, projection));
    
    output.uv = input.uv;
    output.color = input.color;
    output.screenPosition = mul(wvp, float4(input.position, 1.0f));
    output.worldPos = mul(world, float4(input.position, 1.0f)).xyz;
    output.tangent = normalize(mul((float3x3) world, input.tangent));
    output.normal = normalize(mul((float3x3) worldInvTranspose, input.normal));
    
	return output;
}