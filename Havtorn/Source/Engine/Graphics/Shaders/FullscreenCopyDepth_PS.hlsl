// Copyright 2022 Team Havtorn. All Rights Reserved.

#include "Includes/FullscreenShaderStructs.hlsli"

float Lerp(float a, float b, float t)
{
    return (1.0f - t) * a + b * t;
}

float InvLerp(float a, float b, float v)
{
    return (v - a) / (b - a);
}

float Remap(float inMin, float inMax, float outMin, float outMax, float v)
{
    float t = InvLerp(inMin, inMax, v);
    return Lerp(outMin, outMax, t);
}

PixelOutput main(VertexToPixel input)
{
    PixelOutput returnValue;
    const float resource = fullscreenTexture1.Sample(defaultSampler, input.myUV.xy).r;

	returnValue.myColor.r = Remap(0.99f, 1.0f, 0.0f, 1.0f, resource);
    returnValue.myColor.gb = 0.0f;
    returnValue.myColor.a = 1.0f;
    return returnValue;
}