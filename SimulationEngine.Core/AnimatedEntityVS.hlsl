#include "VertexToPixel.hlsli"
#include "VertexInput.hlsli"
#include "AnimJoint.hlsli"

cbuffer ExternalData : register(b4)
{
	// - 16 byte spacing rules -
    matrix world;
	// - -
    matrix worldInvTranspose;
	// - -
    matrix view;
	// - -
    matrix projection;
    // - -
    Joint joints[MAX_JOINT_COUNT];
}

VertexToPixel main(VertexShaderInput input)
{
    VertexToPixel output;
    
    matrix wvp = mul(projection, mul(view, world));
    
    output.screenPosition = mul(wvp, float4(input.position, 1.0f));
    output.normal = normalize(mul((float3x3) worldInvTranspose, input.normal));
    output.uv = input.uv;
    output.worldPos = mul(world, float4(input.position, 1.0f)).xyz;
    output.tangent = normalize(mul((float3x3) world, input.tangent));
    
    float4x4 test = joints[30].InvBindPose;
    output.test = test._11_11_11_11;
    
    return output;
}