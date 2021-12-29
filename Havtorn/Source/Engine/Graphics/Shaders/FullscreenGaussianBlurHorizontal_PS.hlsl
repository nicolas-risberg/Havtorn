#include "Includes/FullscreenShaderStructs.hlsli"

PixelOutput main(VertexToPixel input) {
	PixelOutput returnValue;
	
    float texelSize = 1.0f / /*resolution*/(myResolution.x * 0.125f);
	float3 blurColor = float3(0.0f, 0.0f, 0.0f);
	
	unsigned int kernelSize = 5;
	float start = (((float) (kernelSize) - 1.0f) / 2.0f) * -1.0f;
	for (unsigned int i = 0; i < kernelSize; i++) 
	{
		float2 uv = input.myUV.xy + float2(texelSize * (start + (float) i), 0.0f);
		float3 resource = fullscreenTexture1.Sample(defaultSampler, uv).rgb;
		blurColor += resource * gaussianKernel5[i];
	}
	
	returnValue.myColor.rgb = blurColor;
	returnValue.myColor.a = 1.0f;
	return returnValue;
};