#ifndef __LIGHTING_H_
#define __LIGHTING_H_

#define MAX_LIGHT_COUNT 5
#define LIGHT_TYPE_DIRECTIONAL 0
#define LIGHT_TYPE_POINT 1
#define LIGHT_TYPE_SPOT 2
#define MAX_SPECULAR_EXPONENT 256.0f

struct Light
{
    // 16 byte memory padding rules applied:
    // - -
    int Type;
    float3 Direction;
    // - -
    float Range;
    float3 Position;
    // - - 
    float Intensity;
    float3 Color;
    // - -
    float SpotInnerAngle;
    float SpotOuterAngle;
    float2 Padding;
    // - -
};

#endif //__LIGHTING_H_