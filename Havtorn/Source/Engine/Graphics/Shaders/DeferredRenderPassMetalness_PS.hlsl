// Copyright 2022 Team Havtorn. All Rights Reserved.

#include "Includes/PBRDeferredAmbiance.hlsli"
//#include "Includes/DeferredPBRFunctions.hlsli"

PixelOutput main(VertexToPixel input)
{
    PixelOutput output;

    const float metalness = GBuffer_Metalness(input.myUV);
    output.myColor.rgb = metalness;
    output.myColor.a = 1.0f;
    return output;
}