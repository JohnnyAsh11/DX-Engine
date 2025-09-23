#ifndef __VERTEXINPUT_H_
#define __VERTEXINPUT_H_

struct VertexShaderInput
{
    float3 position : POSITION;
    float3 normal : NORMAL;
    float2 uv : UV;
    float3 tangent : TANGENT;
    float4 color : COLOR;
};

#endif //__VERTEXINPUT_H_