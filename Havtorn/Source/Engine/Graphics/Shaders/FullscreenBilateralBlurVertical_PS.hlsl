// Copyright 2022 Team Havtorn. All Rights Reserved.

#include "Includes/FullscreenShaderStructs.hlsli"

PixelOutput main(VertexToPixel input)
{
    PixelOutput returnValue;
	
    float texelSize = 1.0f / /*900.0f*/resolution.y;
    float3 blurColor = float3(0.0f, 0.0f, 0.0f);
    float normalizationFactor = 0.0f;
    const float bZ = 1.0 / normpdf(0.0, BSIGMA);
    const float3 originalPixelValue = fullscreenTexture1.Sample(defaultSampler, input.myUV.xy).rgb;
    //float3 originalDepth = fullscreenTexture2.Sample(defaultSampler, input.myUV.xy).rgb;

    const unsigned int kernelSize = 5;
    const float start = (((float) (kernelSize) - 1.0f) / 2.0f) * -1.0f;
    for (unsigned int i = 0; i < kernelSize; i++)
    {
	    const float2 uv = input.myUV.xy + float2(0.0f, texelSize * (start + (float) i));
	    const float3 resource = fullscreenTexture1.Sample(defaultSampler, uv).rgb;
        //float3 resourceDepth = fullscreenTexture2.Sample(defaultSampler, uv).rgb;
        //colorFactor = normpdf3(resourceDepth - originalDepth, BSIGMA) * bZ * gaussianKernel5[i];
        const float colorFactor = normpdf3(resource - originalPixelValue, BSIGMA) * bZ * gaussianKernel5[i];
        normalizationFactor += colorFactor;
        blurColor += resource * colorFactor;
    }
	
    returnValue.myColor.rgb = blurColor / normalizationFactor;
    returnValue.myColor.a = 1.0f;
    return returnValue;
};