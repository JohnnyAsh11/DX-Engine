#include "VertexToPixel.hlsli"

struct VertexShaderInput
{
    float3 position : POSITION; 
    float3 normal: NORMAL; 
    float2 uv : UV;
    float3 tangent : TANGENT;
};

VertexToPixel main( VertexShaderInput input )
{
    VertexToPixel output;
    
    output.worldPos = float3(0.0f, 0.0f, 0.0f);
    output.normal = input.normal;
    output.uv = input.uv;
    output.tangent = input.tangent;
    output.screenPosition = float4(input.position, 0.0f);
    
	return output;
}