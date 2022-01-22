// Copyright 2022 Team Havtorn. All Rights Reserved.

#include "Includes/FullscreenShaderStructs.hlsli"

PixelOutput main(VertexToPixel input)
{
    PixelOutput returnValue;
    float resource = fullscreenTexture1.Sample(defaultSampler, input.myUV.xy).r;
    returnValue.myColor.r = resource;
    returnValue.myColor.a = 1.0f;
    return returnValue;
}