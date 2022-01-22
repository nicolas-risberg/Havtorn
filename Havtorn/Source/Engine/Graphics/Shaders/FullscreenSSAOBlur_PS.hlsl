// Copyright 2022 Team Havtorn. All Rights Reserved.

#include "Includes/FullscreenShaderStructs.hlsli"

//static int maxSize = 4;
//static int maxKernelSize = ((maxSize * 2 + 1) * (maxSize * 2 + 1));

//static int i = 0;
//static int j = 0;
//static int count = 0;

//static float3 valueRatios = float3(0.3f, 0.59f, 0.11f);
    
//static float values[maxKernelSize];

//static float4 color = 0.0f;
//static float4 meanTemp = 0.0f;
//static float4 mean = 0.0f;
//static float valueMean = 0.0f;
//static float variance = 0.0f;
//static float minVariance = -1.0f;

//void FindMean(int i0, int i1, int j0, int j1, float2 uv)
//{
//    meanTemp = 0.0f;
//    count = 0.0f;
    
//    for (i = i0; i <= i1; ++i)
//    {
//        for (j = j0; j <= j1; ++j)
//        {
//            color = fullscreenTexture1.Sample(defaultSampler, (uv + float2(i, j)) * )

//        }

//    }
//}

PixelOutput main(VertexToPixel input)
{
    PixelOutput returnValue;

    const int blurSize = 4;
    const float2 texelSize = 1.0 / myResolution;
    float result = 0.0;
    const float2 hlim = float(-blurSize) * 0.5 + 0.5;
    
    for (int i = 0; i < blurSize; ++i)
    {
        for (int j = 0; j < blurSize; ++j)
        {
	        const float2 offset = (hlim + float2(float(i), float(j))) * texelSize;
            result += fullscreenTexture1.Sample(defaultSampler, input.myUV + offset).r;
        }
    }
 
    returnValue.myColor.rgb = result / float(blurSize * blurSize);
    returnValue.myColor.a = 1.0f;
    return returnValue;
};