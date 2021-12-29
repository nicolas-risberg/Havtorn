#include "Includes/FullscreenShaderStructs.hlsli"

PixelOutput main(VertexToPixel input)
{
    PixelOutput returnValue;
    float3 resource = fullscreenTexture1.Sample(defaultSampler, input.myUV.xy).rgb;
    float3 resource2 = fullscreenTexture2.Sample(defaultSampler, input.myUV.xy).rgb;
	
	returnValue.myColor.rgb = resource * resource2;
	
    returnValue.myColor.a = 0.0f;
    return returnValue;
};