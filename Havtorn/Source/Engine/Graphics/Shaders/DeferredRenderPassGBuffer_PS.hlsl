// Copyright 2022 Team Havtorn. All Rights Reserved.

#include "Includes/DeferredSamplingFunctions.hlsli"

struct GBufferOutput
{
    float4 WorldPosition      : SV_TARGET0;
    float4 Albedo             : SV_TARGET1;
    float4 Normal             : SV_TARGET2;
    float4 VertexNormal       : SV_TARGET3;
    //float myMetalness           : SV_TARGET4;
    //float myRoughness           : SV_TARGET5;
    //float myAmbientOcclusion    : SV_TARGET6;
    //float myEmissive            : SV_TARGET7;
};

GBufferOutput main(VertexModelToPixel input)
{
    VertexToPixel vertToPixel;
    //vertToPixel.myPosition  = input.myPosition;
    vertToPixel.myUV        = input.myUV;
    
    float3 albedo = PixelShader_Albedo(vertToPixel.myUV).rgb;
    float3 normal = PixelShader_NormalForIsolatedRendering(vertToPixel.myUV).xyz;
    
    if (myNumberOfDetailNormals > 0)
    {
        float detailNormalStrength = PixelShader_DetailNormalStrength(vertToPixel.myUV);
        const float strengthMultiplier = DetailStrengthDistanceMultiplier(cameraPosition.xyz, input.myWorldPosition.xyz);
        float3 detailNormal;

        // Blend based on detail normal strength
        // X3512 Sampler array index must be literal expression => DETAILNORMAL_#
        // Sampled detail normal strength value: 
        //      0.1f - 0.24f    == DETAILNORMAL_1
        //      0.26f - 0.49f   == DETAILNORMAL_2
        //      0.51f - 0.74f   == DETAILNORMAL_3
        //      0.76f - 1.0f    == DETAILNORMAL_4
        // Note! This if-chain exists in 3 shaders: PBRPixelShader, GBufferPixelShader and DeferredRenderPassGBufferPixelShader
        // Make this better please
        if (detailNormalStrength > DETAILNORMAL_4_STR_RANGE_MIN)
        {
            detailNormal = PixelShader_DetailNormal(vertToPixel.myUV, DETAILNORMAL_4).xyz;
            detailNormalStrength = (detailNormalStrength - DETAILNORMAL_4_STR_RANGE_MIN + 0.01f) / DETAILNORMAL_STR_RANGE_DIFF;
        }
        else if (detailNormalStrength > DETAILNORMAL_3_STR_RANGE_MIN)
        {
            detailNormal = PixelShader_DetailNormal(vertToPixel.myUV, DETAILNORMAL_3).xyz;
            detailNormalStrength = (detailNormalStrength - DETAILNORMAL_3_STR_RANGE_MIN + 0.01f) / DETAILNORMAL_STR_RANGE_DIFF;
        }
        else if (detailNormalStrength > DETAILNORMAL_2_STR_RANGE_MIN)
        {
            detailNormal = PixelShader_DetailNormal(vertToPixel.myUV, DETAILNORMAL_2).xyz;
            detailNormalStrength = (detailNormalStrength - DETAILNORMAL_2_STR_RANGE_MIN + 0.01f) / DETAILNORMAL_STR_RANGE_DIFF;
        }
        else
        {
            detailNormal = PixelShader_DetailNormal(vertToPixel.myUV, DETAILNORMAL_1).xyz;
            detailNormalStrength = (detailNormalStrength - DETAILNORMAL_1_STR_RANGE_MIN + 0.01f) / DETAILNORMAL_STR_RANGE_DIFF;
        }
        
        detailNormal = SetDetailNormalStrength(detailNormal, detailNormalStrength, strengthMultiplier);
        normal = normal * 0.5 + 0.5;
        detailNormal = detailNormal * 0.5 + 0.5;
        normal = BlendRNM(normal, detailNormal);
        
        //---
        
        // Blend all 4 ontop of eachother
        //for (int i = 0; i < myNumberOfDetailNormals; ++i)
        //{
        //    detailNormal = PixelShader_DetailNormal(vertToPixel, i).myColor.xyz;
        //    detailNormal = SetDetailNormalStrength(detailNormal, detailNormalStrength, strengthMultiplier);
        //    normal = normal * 0.5 + 0.5;
        //    detailNormal = detailNormal * 0.5 + 0.5;
        //    normal = BlendRNM(normal, detailNormal);
        //}
    } // End of if
    
    float ambientOcclusion      = PixelShader_AmbientOcclusion(vertToPixel.myUV);
    float metalness             = PixelShader_Metalness(vertToPixel.myUV);
    float perceptualRoughness   = PixelShader_PerceptualRoughness(vertToPixel.myUV);
    float emissive              = PixelShader_Emissive(vertToPixel.myUV);
    
    // Original, using 8 textures
    //GBufferOutput output;
    //output.myWorldPosition = input.myWorldPosition;
    //output.myAlbedo = float4(albedo, 1.0f);
    //output.myNormal = float4(normal, 1.0f);
    //output.myVertexNormal = float4(input.myNormal.xyz, 1.0f);
    //output.myMetalness = metalness;
    //output.myRoughness = perceptualRoughness;
    //output.myAmbientOcclusion = ambientOcclusion;
    //output.myEmissive = emissive;
    //return output;
    
    // Using 4 textures
    GBufferOutput output;
    output.WorldPosition  = input.myWorldPosition;
    output.Albedo         = float4(albedo, 1.0f);
    output.Normal         = float4(normal, 1.0f);
    output.VertexNormal   = float4(input.myNormal.xyz, 1.0f);
    
    output.WorldPosition.w = metalness;
    output.Albedo.w        = perceptualRoughness;
    output.Normal.w        = ambientOcclusion;
    output.VertexNormal.w    = emissive;
    return output;
}