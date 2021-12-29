#include "Includes/FullscreenShaderStructs.hlsli"

GBufferOutput main(VertexToPixel input)
{
    GBufferOutput gbuffer;
    float3 albedo       = fullscreenTexture2.Sample(defaultSampler, input.myUV.xy).rgb;
    float3 normal       = fullscreenTexture3.Sample(defaultSampler, input.myUV.xy).rgb;
    float3 vertexNormal = fullscreenTexture4.Sample(defaultSampler, input.myUV.xy).rgb;
    float4 material     = fullscreenTexture5.Sample(defaultSampler, input.myUV.xy).rgba;
    gbuffer.myAlbedo = albedo;
    gbuffer.myNormal = normal;
    gbuffer.myVertexNormal = vertexNormal;
    gbuffer.myMetalRoughAOEm = material;
    return gbuffer;
}