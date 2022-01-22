// Copyright 2022 Team Havtorn. All Rights Reserved.

#include "Includes/FullscreenShaderStructs.hlsli"

PixelOutput main(VertexToPixel input) {
	PixelOutput returnValue;
	
    float texelSize = 1.0f / /*resolution*/(myResolution.y * 0.125f);
	float3 blurColor = float3(0.0f, 0.0f, 0.0f);

	const unsigned int kernelSize = 5;
	const float start = (((float) (kernelSize) - 1.0f) / 2.0f) * -1.0f;
	for (unsigned int i = 0; i < kernelSize; i++)
	{
		const float2 uv = input.myUV.xy + float2(0.0f, texelSize * (start + (float) i));
		const float3 resource = fullscreenTexture1.Sample(defaultSampler, uv).rgb;
		blurColor += resource * gaussianKernel5[i];
	}
	
	returnValue.myColor.rgb = blurColor;
	returnValue.myColor.a = 1.0f;
	return returnValue;
};