// Copyright 2022 Team Havtorn. All Rights Reserved.

#include "Includes/PBRDeferredAmbiance.hlsli"
//#include "Includes/DeferredPBRFunctions.hlsli"

PixelOutput main(VertexToPixel input)
{
    PixelOutput output;

    float ambientOcclusion = GBuffer_AmbientOcclusion(input.myUV);
    output.myColor.rgb = ambientOcclusion.xxx;
    output.myColor.a = 1.0f;
    return output;
}