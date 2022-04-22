// Copyright 2022 Team Havtorn. All Rights Reserved.

#include "Includes/DeferredSamplingFunctions.hlsli"

struct GBufferOutput
{
    float4 Albedo             : SV_TARGET0;
    float4 Normal             : SV_TARGET1;
    float4 VertexNormal       : SV_TARGET2;
    float4 MetalRoughEmAO     : SV_TARGET3;
};

GBufferOutput main(VertexModelToPixel input)
{
    VertexToPixel vertToPixel;
    vertToPixel.Position  = input.Position;
    vertToPixel.UV        = input.UV;
    
    // Alpha clip for foliage
    clip(PixelShader_Albedo(vertToPixel.UV).a - 0.5f);
    
    float3 albedo = PixelShader_Albedo(vertToPixel.UV).rgb;
    float3 normal = PixelShader_Normal(vertToPixel.UV).xyz;
    
    // === Detail Normals ===
    //if (NumberOfDetailNormals > 0)
    //{
    //    float detailNormalStrength = PixelShader_DetailNormalStrength(vertToPixel.UV);
    //    float strengthMultiplier = DetailStrengthDistanceMultiplier(cameraPosition.xyz, input.WorldPosition.xyz);
    //    float3 detailNormal;

    //    // Blend based on detail normal strength
    //    // X3512 Sampler array index must be literal expression => DETAILNORMAL_#
    //    // Sampled detail normal strength value: 
    //    //      0.2f - 0.24f    == DETAILNORMAL_1
    //    //      0.26f - 0.49f   == DETAILNORMAL_2
    //    //      0.51f - 0.74f   == DETAILNORMAL_3
    //    //      0.76f - 1.0f    == DETAILNORMAL_4
    //    // Note! This if-chain exists in 3 shaders: PBRPixelShader, GBufferPixelShader and DeferredRenderPassGBufferPixelShader
    //    // Make this better please
    //    if (detailNormalStrength > DETAILNORMAL_4_STR_RANGE_MIN)
    //    {
    //        detailNormal = PixelShader_DetailNormal(vertToPixel.UV, DETAILNORMAL_4).xyz;
    //        detailNormalStrength = (detailNormalStrength - DETAILNORMAL_4_STR_RANGE_MIN + 0.01f) / DETAILNORMAL_STR_RANGE_DIFF;
    //    }
    //    else if (detailNormalStrength > DETAILNORMAL_3_STR_RANGE_MIN)
    //    {
    //        detailNormal = PixelShader_DetailNormal(vertToPixel.UV, DETAILNORMAL_3).xyz;
    //        detailNormalStrength = (detailNormalStrength - DETAILNORMAL_3_STR_RANGE_MIN + 0.01f) / DETAILNORMAL_STR_RANGE_DIFF;
    //    }
    //    else if (detailNormalStrength > DETAILNORMAL_2_STR_RANGE_MIN)
    //    {
    //        detailNormal = PixelShader_DetailNormal(vertToPixel.UV, DETAILNORMAL_2).xyz;
    //        detailNormalStrength = (detailNormalStrength - DETAILNORMAL_2_STR_RANGE_MIN + 0.01f) / DETAILNORMAL_STR_RANGE_DIFF;
    //    }
    //    else
    //    {
    //        detailNormal = PixelShader_DetailNormal(vertToPixel.UV, DETAILNORMAL_1).xyz;
    //        detailNormalStrength = (detailNormalStrength - DETAILNORMAL_1_STR_RANGE_MIN + 0.01f) / DETAILNORMAL_STR_RANGE_DIFF;
    //    }
        
    //    detailNormal = SetDetailNormalStrength(detailNormal, detailNormalStrength, strengthMultiplier);
    //    normal = normal * 0.5 + 0.5;
    //    detailNormal = detailNormal * 0.5 + 0.5;
    //    normal = BlendRNM(normal, detailNormal);
        
    //    //---
        
    //    // Blend all 4 ontop of eachother
    //    //for (int i = 0; i < myNumberOfDetailNormals; ++i)
    //    //{
    //    //    detailNormal = PixelShader_DetailNormal(vertToPixel, i).myColor.xyz;
    //    //    detailNormal = SetDetailNormalStrength(detailNormal, detailNormalStrength, strengthMultiplier);
    //    //    normal = normal * 0.5 + 0.5;
    //    //    detailNormal = detailNormal * 0.5 + 0.5;
    //    //    normal = BlendRNM(normal, detailNormal);
    //    //}
    //} // End of if
    
    float3x3 tangentSpaceMatrix = float3x3(normalize(input.Tangent.xyz), normalize(input.Bitangent.xyz), normalize(input.Normal.xyz));
    normal = mul(normal.xyz, tangentSpaceMatrix);
    normal = normalize(normal);
    
    float ambientOcclusion      = PixelShader_AmbientOcclusion(vertToPixel.UV);
    float metalness             = PixelShader_Metalness(vertToPixel.UV);
    float perceptualRoughness   = PixelShader_PerceptualRoughness(vertToPixel.UV);
    float emissive              = PixelShader_Emissive(vertToPixel.UV);
    
    GBufferOutput output;    
    output.Albedo = float4(albedo.xyz, 1.0f);
    output.Normal = float4(normal.xyz, 1.0f);
    output.VertexNormal = float4(input.Normal.xyz, 1.0f);
    output.MetalRoughEmAO = float4(metalness, perceptualRoughness, emissive, ambientOcclusion);
    
    return output;
}