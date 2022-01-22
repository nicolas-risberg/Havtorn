// Copyright 2022 Team Havtorn. All Rights Reserved.

#include "Includes/DeferredSamplingFunctions.hlsli"

struct GBufferOutput
{    
    float4 myAlbedo             : SV_TARGET0;
    float4 myNormal             : SV_TARGET1;
    float4 myVertexNormal       : SV_TARGET2;
    float4 myMetalRoughEmAO     : SV_TARGET3;
};

GBufferOutput main(VertexPaintModelToPixel input)
{
    VertexToPixel vertToPixel;
    vertToPixel.myPosition = input.myPosition;
    vertToPixel.myUV = input.myUV;
    
    float3 vertexColors = input.myColor;
    
    float3 albedo               = PixelShader_Albedo(vertToPixel.myUV).rgb;
    float3 normal               = PixelShader_Normal(vertToPixel.myUV).xyz;
    float3x3 tangentSpaceMatrix = float3x3(normalize(input.myTangent.xyz), normalize(input.myBinormal.xyz), normalize(input.myNormal.xyz));
    normal = mul(normal.xyz, tangentSpaceMatrix);
    normal = normalize(normal);
    float ambientOcclusion      = PixelShader_AmbientOcclusion(vertToPixel.myUV);
    float metalness             = PixelShader_Metalness(vertToPixel.myUV);
    float perceptualRoughness   = PixelShader_PerceptualRoughness(vertToPixel.myUV);
    float emissive              = PixelShader_Emissive(vertToPixel.myUV);
   
    // Extra materials
    float3 albedo2              = PixelShader_Albedo(vertexPaintTextures[RED_ALBEDO], vertToPixel.myUV).rgb;
    float3 normal2              = PixelShader_Normal(vertexPaintTextures[RED_NORMAL], vertToPixel.myUV).xyz;
    normal2 = mul(normal2.xyz, tangentSpaceMatrix);
    normal2 = normalize(normal2);
    float ambientOcclusion2     = PixelShader_AmbientOcclusion(vertexPaintTextures[RED_NORMAL], vertToPixel.myUV);
    float metalness2            = PixelShader_Metalness(vertexPaintTextures[RED_MATERIAL], vertToPixel.myUV);
    float perceptualRoughness2  = PixelShader_PerceptualRoughness(vertexPaintTextures[RED_MATERIAL], vertToPixel.myUV);
    float emissive2             = PixelShader_Emissive(vertexPaintTextures[RED_MATERIAL], vertToPixel.myUV);
    
    float3 albedo3              = PixelShader_Albedo(vertexPaintTextures[GREEN_ALBEDO], vertToPixel.myUV).rgb;
    float3 normal3              = PixelShader_Normal(vertexPaintTextures[GREEN_NORMAL], vertToPixel.myUV).xyz;
    normal3 = mul(normal3.xyz, tangentSpaceMatrix);
    normal3 = normalize(normal3);
    float ambientOcclusion3     = PixelShader_AmbientOcclusion(vertexPaintTextures[GREEN_NORMAL], vertToPixel.myUV);
    float metalness3            = PixelShader_Metalness(vertexPaintTextures[GREEN_MATERIAL], vertToPixel.myUV);
    float perceptualRoughness3  = PixelShader_PerceptualRoughness(vertexPaintTextures[GREEN_MATERIAL], vertToPixel.myUV);
    float emissive3             = PixelShader_Emissive(vertexPaintTextures[GREEN_MATERIAL], vertToPixel.myUV);
    
    float3 albedo4              = PixelShader_Albedo(vertexPaintTextures[BLUE_ALBEDO], vertToPixel.myUV).rgb;
    float3 normal4              = PixelShader_Normal(vertexPaintTextures[BLUE_NORMAL], vertToPixel.myUV).xyz;
    normal4 = mul(normal4.xyz, tangentSpaceMatrix);
    normal4 = normalize(normal4);
    float ambientOcclusion4     = PixelShader_AmbientOcclusion(vertexPaintTextures[BLUE_NORMAL], vertToPixel.myUV);
    float metalness4            = PixelShader_Metalness(vertexPaintTextures[BLUE_MATERIAL], vertToPixel.myUV);
    float perceptualRoughness4  = PixelShader_PerceptualRoughness(vertexPaintTextures[BLUE_MATERIAL], vertToPixel.myUV);
    float emissive4             = PixelShader_Emissive(vertexPaintTextures[BLUE_MATERIAL], vertToPixel.myUV);
    
    // Using 4 textures
    GBufferOutput output;
    
    float3 albedo_Base = albedo;
    float3 albedo_R = albedo2;
    float3 albedo_G = albedo3;
    float3 albedo_B = albedo4;
    
    float3 combined_RGB = float3(albedo_R * vertexColors.r) + (albedo_G * vertexColors.g) + (albedo_B * vertexColors.b); //+ (albedo_Base * (1.0f - vertexColors.r - vertexColors.g - vertexColors.b));
    output.myAlbedo = float4(combined_RGB, 1.0f);
    
    
    
    
  
    //output.myAlbedo = float4(lerp(lerp(lerp(albedo, albedo2, vertexColors.r), albedo3, vertexColors.g), albedo4, vertexColors.b), 1.0f);
    output.myNormal = float4(lerp(lerp(lerp(normal, normal2, vertexColors.r), normal3, vertexColors.g), normal4, vertexColors.b), 1.0f);
    output.myVertexNormal = float4(input.myNormal.xyz, 1.0f);
    
    output.myMetalRoughEmAO.r = lerp(lerp(lerp(metalness, metalness2, vertexColors.r), metalness3, vertexColors.g), metalness4, vertexColors.b);
    output.myMetalRoughEmAO.g = lerp(lerp(lerp(perceptualRoughness, perceptualRoughness2, vertexColors.r), perceptualRoughness3, vertexColors.g), perceptualRoughness4, vertexColors.b);
    output.myMetalRoughEmAO.b = lerp(lerp(lerp(emissive, emissive2, vertexColors.r), emissive3, vertexColors.g), emissive4, vertexColors.b);
    output.myMetalRoughEmAO.a = lerp(lerp(lerp(ambientOcclusion, ambientOcclusion2, vertexColors.r), ambientOcclusion3, vertexColors.g), ambientOcclusion4, vertexColors.b);
    return output;
}