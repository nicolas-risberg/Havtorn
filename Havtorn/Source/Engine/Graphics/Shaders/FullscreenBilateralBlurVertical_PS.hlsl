#include "Includes/FullscreenShaderStructs.hlsli"

PixelOutput main(VertexToPixel input)
{
    PixelOutput returnValue;
	
    float texelSize = 1.0f / /*900.0f*/resolution.y;
    float3 blurColor = float3(0.0f, 0.0f, 0.0f);
    float normalizationFactor = 0.0f;
    float bZ = 1.0 / normpdf(0.0, BSIGMA);
    float colorFactor = 0.0f;
    float3 originalPixelValue = fullscreenTexture1.Sample(defaultSampler, input.myUV.xy).rgb;
    float3 originalDepth = fullscreenTexture2.Sample(defaultSampler, input.myUV.xy).rgb;
    
    unsigned int kernelSize = 5;
    float start = (((float) (kernelSize) - 1.0f) / 2.0f) * -1.0f;
    for (unsigned int i = 0; i < kernelSize; i++)
    {
        float2 uv = input.myUV.xy + float2(0.0f, texelSize * (start + (float) i));
        float3 resource = fullscreenTexture1.Sample(defaultSampler, uv).rgb;
        float3 resourceDepth = fullscreenTexture2.Sample(defaultSampler, uv).rgb;
        //colorFactor = normpdf3(resourceDepth - originalDepth, BSIGMA) * bZ * gaussianKernel5[i];
        colorFactor = normpdf3(resource - originalPixelValue, BSIGMA) * bZ * gaussianKernel5[i];
        normalizationFactor += colorFactor;
        blurColor += resource * colorFactor;
    }
	
    returnValue.myColor.rgb = blurColor / normalizationFactor;
    returnValue.myColor.a = 1.0f;
    return returnValue;
};