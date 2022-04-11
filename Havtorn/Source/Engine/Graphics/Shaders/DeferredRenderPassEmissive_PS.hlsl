// Copyright 2022 Team Havtorn. All Rights Reserved.

#include "Includes/PBRDeferredAmbiance.hlsli"
//#include "Includes/DeferredPBRFunctions.hlsli"

PixelOutput main(VertexToPixel input)
{
    PixelOutput output;

    const float emissiveData = GBuffer_Emissive(input.UV);
    output.Color.rgb = emissiveData;
    output.Color.a = 1.0f;
    return output;
}