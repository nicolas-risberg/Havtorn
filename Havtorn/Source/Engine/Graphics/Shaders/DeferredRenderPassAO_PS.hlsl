// Copyright 2022 Team Havtorn. All Rights Reserved.

#include "Includes/PBRDeferredAmbiance.hlsli"
//#include "Includes/DeferredPBRFunctions.hlsli"

PixelOutput main(VertexToPixel input)
{
    PixelOutput output;

    float ambientOcclusion = GBuffer_AmbientOcclusion(input.UV);
    output.Color.rgb = ambientOcclusion.xxx;
    output.Color.a = 1.0f;
    return output;
}