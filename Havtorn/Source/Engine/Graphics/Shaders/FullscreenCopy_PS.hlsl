// Copyright 2022 Team Havtorn. All Rights Reserved.

#include "Includes/FullscreenShaderStructs.hlsli"

PixelOutput main(VertexToPixel input) 
{
	PixelOutput returnValue;
	const float3 resource = fullscreenTexture1.Sample(defaultSampler, input.myUV.xy).rgb;
	returnValue.myColor.rgb = resource;
	returnValue.myColor.a = 1.0f;
	return returnValue;
};