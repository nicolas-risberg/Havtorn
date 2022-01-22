// Copyright 2022 Team Havtorn. All Rights Reserved.

#include "Includes/FullscreenShaderStructs.hlsli"

GBufferOutput main(VertexToPixel input)
{
    GBufferOutput gbuffer;
    const float3 albedo       = fullscreenTexture2.Sample(defaultSampler, input.myUV.xy).rgb;
    const float3 normal       = fullscreenTexture3.Sample(defaultSampler, input.myUV.xy).rgb;
    const float3 vertexNormal = fullscreenTexture4.Sample(defaultSampler, input.myUV.xy).rgb;
    const float4 material     = fullscreenTexture5.Sample(defaultSampler, input.myUV.xy).rgba;
    gbuffer.myAlbedo = albedo;
    gbuffer.myNormal = normal;
    gbuffer.myVertexNormal = vertexNormal;
    gbuffer.myMetalRoughAOEm = material;
    return gbuffer;
}