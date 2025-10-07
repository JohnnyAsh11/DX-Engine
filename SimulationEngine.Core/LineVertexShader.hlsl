
struct LineVertexInput
{
    float3 position : POSITION;
};

cbuffer ExternalData : register(b2)
{
    matrix world;
    matrix view;
    matrix projection;
    float4 color;
};

struct LineVertexToPixel
{
    float4 position : SV_POSITION;
    float4 color : COLOR;
};

LineVertexToPixel main(LineVertexInput input)
{
    LineVertexToPixel output;
    matrix wvp = mul(projection, mul(view, world));
    
    output.position = mul(wvp, float4(input.position, 1.0f));
    output.color = color;
    
    return output;
}