// Copyright 2022 Team Havtorn. All Rights Reserved.

#include "Includes/PBRDeferredAmbiance.hlsli"
//#include "Includes/DeferredPBRFunctions.hlsli"

PixelOutput main(VertexToPixel input)
{
    PixelOutput output;

    const float3 normal = GBuffer_Normal(input.myUV).xyz;
    output.myColor.rgb = normal * 0.5f + 0.5f;
    output.myColor.a = 1.0f;
    return output;
}