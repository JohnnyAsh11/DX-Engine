#include "VertexToPixel.hlsli"

struct VertexShaderInput
{
    float3 position : POSITION; 
    float3 normal: NORMAL; 
    float2 uv : UV;
    float3 tangent : TANGENT;
};

VertexToPixel main( VertexShaderInput input ) : SV_POSITION
{
    VertexToPixel output;
    
    output.screenPosition = input.position;
    output.normal = input.normal;
    output.uv = input.uv;
    output.tangent = input.tangent;
    
	return output;
}