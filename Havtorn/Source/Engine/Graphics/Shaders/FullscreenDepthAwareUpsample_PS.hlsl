// Copyright 2022 Team Havtorn. All Rights Reserved.

#include "Includes/FullscreenShaderStructs.hlsli"

PixelOutput main(VertexToPixel input)
{
    PixelOutput returnValue;
    //float4 downSampledColorRed = fullscreenTexture1.GatherRed(defaultSampler, input.myUV.xy);
    //float4 downSampledColorGreen = fullscreenTexture1.GatherGreen(defaultSampler, input.myUV.xy);
    //float4 downSampledColorBlue = fullscreenTexture1.GatherBlue(defaultSampler, input.myUV.xy);
    //float4 downSampledColorAlpha = fullscreenTexture1.GatherAlpha(defaultSampler, input.myUV.xy);
    //float4 nearestDepth = fullscreenTexture2.GatherRed(defaultSampler, input.myUV.xy);
    //float actualDepth = fullscreenTexture3.Sample(defaultSampler, input.myUV.xy);
    
    //float smallestDifference = 10.0f;
    //if (abs(nearestDepth.x - actualDepth) < smallestDifference)
    //{
    //    smallestDifference = abs(nearestDepth.x - actualDepth);
    //    returnValue.myColor.rgb = float3(downSampledColorRed.x, downSampledColorGreen.x, downSampledColorBlue.x);
    //    //returnValue.myColor.a = downSampledColorAlpha.x;
    //}
    //if (abs(nearestDepth.y - actualDepth) < smallestDifference)
    //{
    //    smallestDifference = abs(nearestDepth.y - actualDepth);
    //    returnValue.myColor.rgb = float3(downSampledColorRed.y, downSampledColorGreen.y, downSampledColorBlue.y);
    //    //returnValue.myColor.a = downSampledColorAlpha.y;
    //}
    //if (abs(nearestDepth.z - actualDepth) < smallestDifference)
    //{
    //    smallestDifference = abs(nearestDepth.z - actualDepth);
    //    returnValue.myColor.rgb = float3(downSampledColorRed.z, downSampledColorGreen.z, downSampledColorBlue.z);
    //    //returnValue.myColor.a = downSampledColorAlpha.z;
    //}
    //if (abs(nearestDepth.w - actualDepth) < smallestDifference)
    //{
    //    smallestDifference = abs(nearestDepth.w - actualDepth);
    //    returnValue.myColor.rgb = float3(downSampledColorRed.w, downSampledColorGreen.w, downSampledColorBlue.w);
    //    //returnValue.myColor.a = downSampledColorAlpha.w;
    //}

    const float3 resource = fullscreenTexture1.Sample(defaultSampler, input.myUV.xy).rgb;
    returnValue.myColor.rgb = resource;
    returnValue.myColor.a = 1.0f;
    return returnValue;
};