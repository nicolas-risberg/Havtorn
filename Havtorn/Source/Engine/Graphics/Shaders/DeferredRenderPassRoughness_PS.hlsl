// Copyright 2022 Team Havtorn. All Rights Reserved.

#include "Includes/PBRDeferredAmbiance.hlsli"
//#include "Includes/DeferredPBRFunctions.hlsli"

PixelOutput main(VertexToPixel input)
{
    PixelOutput output;

    const float perceptualRoughness = GBuffer_PerceptualRoughness(input.UV);
    output.Color.rgb = perceptualRoughness;
    output.Color.a = 1.0f;
    return output;
}