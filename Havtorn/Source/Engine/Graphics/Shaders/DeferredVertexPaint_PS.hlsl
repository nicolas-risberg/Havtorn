// Copyright 2022 Team Havtorn. All Rights Reserved.

#include "Includes/DeferredSamplingFunctions.hlsli"

struct GBufferOutput
{    
    float4 Albedo             : SV_TARGET0;
    float4 Normal             : SV_TARGET1;
    float4 VertexNormal       : SV_TARGET2;
    float4 MetalRoughEmAO     : SV_TARGET3;
};

GBufferOutput main(VertexPaintModelToPixel input)
{
    VertexToPixel vertToPixel;
    //vertToPixel.myPosition = input.myPosition;
    vertToPixel.myUV = input.myUV;
    
    float3 vertexColors = input.myColor;

    //const float3 albedo               = PixelShader_Albedo(vertToPixel.myUV).rgb;
    float3 normal               = PixelShader_Normal(vertToPixel.myUV).xyz;
    const float3x3 tangentSpaceMatrix = float3x3(normalize(input.myTangent.xyz), normalize(input.myBinormal.xyz), normalize(input.myNormal.xyz));
    normal = mul(normal.xyz, tangentSpaceMatrix);
    normal = normalize(normal);
    const float ambientOcclusion      = PixelShader_AmbientOcclusion(vertToPixel.myUV);
    const float metalness             = PixelShader_Metalness(vertToPixel.myUV);
    const float perceptualRoughness   = PixelShader_PerceptualRoughness(vertToPixel.myUV);
    const float emissive              = PixelShader_Emissive(vertToPixel.myUV);
   
    // Extra materials
    const float3 albedo2              = PixelShader_Albedo(vertexPaintTextures[RED_ALBEDO], vertToPixel.myUV).rgb;
    float3 normal2              = PixelShader_Normal(vertexPaintTextures[RED_NORMAL], vertToPixel.myUV).xyz;
    normal2 = mul(normal2.xyz, tangentSpaceMatrix);
    normal2 = normalize(normal2);
    const float ambientOcclusion2     = PixelShader_AmbientOcclusion(vertexPaintTextures[RED_NORMAL], vertToPixel.myUV);
    const float metalness2            = PixelShader_Metalness(vertexPaintTextures[RED_MATERIAL], vertToPixel.myUV);
    const float perceptualRoughness2  = PixelShader_PerceptualRoughness(vertexPaintTextures[RED_MATERIAL], vertToPixel.myUV);
    const float emissive2             = PixelShader_Emissive(vertexPaintTextures[RED_MATERIAL], vertToPixel.myUV);

    const float3 albedo3              = PixelShader_Albedo(vertexPaintTextures[GREEN_ALBEDO], vertToPixel.myUV).rgb;
    float3 normal3              = PixelShader_Normal(vertexPaintTextures[GREEN_NORMAL], vertToPixel.myUV).xyz;
    normal3 = mul(normal3.xyz, tangentSpaceMatrix);
    normal3 = normalize(normal3);
    const float ambientOcclusion3     = PixelShader_AmbientOcclusion(vertexPaintTextures[GREEN_NORMAL], vertToPixel.myUV);
    const float metalness3            = PixelShader_Metalness(vertexPaintTextures[GREEN_MATERIAL], vertToPixel.myUV);
    const float perceptualRoughness3  = PixelShader_PerceptualRoughness(vertexPaintTextures[GREEN_MATERIAL], vertToPixel.myUV);
    const float emissive3             = PixelShader_Emissive(vertexPaintTextures[GREEN_MATERIAL], vertToPixel.myUV);

    const float3 albedo4              = PixelShader_Albedo(vertexPaintTextures[BLUE_ALBEDO], vertToPixel.myUV).rgb;
    float3 normal4              = PixelShader_Normal(vertexPaintTextures[BLUE_NORMAL], vertToPixel.myUV).xyz;
    normal4 = mul(normal4.xyz, tangentSpaceMatrix);
    normal4 = normalize(normal4);
    const float ambientOcclusion4     = PixelShader_AmbientOcclusion(vertexPaintTextures[BLUE_NORMAL], vertToPixel.myUV);
    const float metalness4            = PixelShader_Metalness(vertexPaintTextures[BLUE_MATERIAL], vertToPixel.myUV);
    const float perceptualRoughness4  = PixelShader_PerceptualRoughness(vertexPaintTextures[BLUE_MATERIAL], vertToPixel.myUV);
    const float emissive4             = PixelShader_Emissive(vertexPaintTextures[BLUE_MATERIAL], vertToPixel.myUV);
    
    // Using 4 textures
    GBufferOutput output;
    
    //float3 albedoBase = albedo;
    const float3 albedoR = albedo2;
    const float3 albedoG = albedo3;
    const float3 albedoB = albedo4;
    
    float3 combinedRGB = float3(albedoR * vertexColors.r) + (albedoG * vertexColors.g) + (albedoB * vertexColors.b); //+ (albedo_Base * (1.0f - vertexColors.r - vertexColors.g - vertexColors.b));
    output.Albedo = float4(combinedRGB, 1.0f);

    //output.myAlbedo = float4(lerp(lerp(lerp(albedo, albedo2, vertexColors.r), albedo3, vertexColors.g), albedo4, vertexColors.b), 1.0f);
    output.Normal = float4(lerp(lerp(lerp(normal, normal2, vertexColors.r), normal3, vertexColors.g), normal4, vertexColors.b), 1.0f);
    output.VertexNormal = float4(input.myNormal.xyz, 1.0f);
    
    output.MetalRoughEmAO.r = lerp(lerp(lerp(metalness, metalness2, vertexColors.r), metalness3, vertexColors.g), metalness4, vertexColors.b);
    output.MetalRoughEmAO.g = lerp(lerp(lerp(perceptualRoughness, perceptualRoughness2, vertexColors.r), perceptualRoughness3, vertexColors.g), perceptualRoughness4, vertexColors.b);
    output.MetalRoughEmAO.b = lerp(lerp(lerp(emissive, emissive2, vertexColors.r), emissive3, vertexColors.g), emissive4, vertexColors.b);
    output.MetalRoughEmAO.a = lerp(lerp(lerp(ambientOcclusion, ambientOcclusion2, vertexColors.r), ambientOcclusion3, vertexColors.g), ambientOcclusion4, vertexColors.b);
    return output;
}