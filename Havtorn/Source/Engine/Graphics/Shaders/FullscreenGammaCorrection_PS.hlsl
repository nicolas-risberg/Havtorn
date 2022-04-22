// Copyright 2022 Team Havtorn. All Rights Reserved.

#include "Includes/FullscreenShaderStructs.hlsli"

float3 LinearToGamma(float3 color)
{
    return pow(abs(color), 1.0 / 2.2);
}

float3 GammaToLinear(float3 color)
{
    return pow(abs(color), 2.2);
}

PixelOutput main(VertexToPixel input)
{
    PixelOutput returnValue;

    const float3 resource = fullscreenTexture1.Sample(defaultSampler, input.myUV.xy).rgb;
    const float3 gammaCorrected = LinearToGamma(resource);
    //returnValue.myColor.rgb = LinearToGamma(resource);
    returnValue.myColor.rgb = gammaCorrected;
    returnValue.myColor.a = 1.0f;
    return returnValue;
}