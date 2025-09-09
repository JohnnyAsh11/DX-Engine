#ifndef __VERTEXTOPIXEL_H_
#define __VERTEXTOPIXEL_H_

struct VertexToPixel
{
    float4 screenPosition : SV_POSITION;
    float3 normal : NORMAL;
    float2 uv : TEXCOORD;
    float3 worldPos : POSITION;
    float3 tangent : TANGENT;
    float4 color : COLOR;
};

#endif
