// Copyright 2022 Team Havtorn. All Rights Reserved.

#include "Includes/PBRDeferredAmbiance.hlsli"
//#include "Includes/DeferredPBRFunctions.hlsli"

PixelOutput main(VertexToPixel input)
{
    PixelOutput output;

    float3 albedo = GBuffer_Albedo(input.UV).rgb;
    albedo = GammaToLinear(albedo);
    output.Color.rgb = albedo;
    output.Color.a = 1.0f;
    return output;
}