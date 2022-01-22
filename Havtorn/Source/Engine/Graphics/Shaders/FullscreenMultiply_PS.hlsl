// Copyright 2022 Team Havtorn. All Rights Reserved.

#include "Includes/FullscreenShaderStructs.hlsli"

PixelOutput main(VertexToPixel input)
{
    PixelOutput returnValue;
    const float3 resource = fullscreenTexture1.Sample(defaultSampler, input.myUV.xy).rgb;
    const float3 resource2 = fullscreenTexture2.Sample(defaultSampler, input.myUV.xy).rgb;
	
	returnValue.myColor.rgb = resource * resource2;
	
    returnValue.myColor.a = 0.0f;
    return returnValue;
};