// Copyright 2022 Team Havtorn. All Rights Reserved.

#include "Includes/PBRDeferredAmbiance.hlsli"
#include "Includes/DeferredPBRFunctions.hlsli"
#include "Includes/ShadowSampling.hlsli"

PixelOutput main(VertexToPixel input)
{
    PixelOutput output;

    const float depth = PixelShader_Exists(input.myUV).r;
    if (depth == 1)
    {
        output.myColor = GBuffer_Albedo(input.myUV);
        return output;
    }
    
    float3 worldPosition = PixelShader_WorldPosition(input.myUV).rgb;
    float3 toEye = normalize( cameraPosition.xyz - worldPosition.xyz);
    float3 albedo = GBuffer_Albedo(input.myUV).rgb;
    albedo = GammaToLinear(albedo);
    const float3 normal = GBuffer_Normal(input.myUV).xyz;
    const float3 vertexNormal = GBuffer_VertexNormal(input.myUV).xyz;
    const float ambientOcclusion = GBuffer_AmbientOcclusion(input.myUV);
    const float metalness = GBuffer_Metalness(input.myUV);
    const float perceptualRoughness = GBuffer_PerceptualRoughness(input.myUV);
    const float emissiveData = GBuffer_Emissive(input.myUV);
    const float ssao = PixelShader_SSAO(input.myUV);

    const float3 specularColor = lerp((float3) 0.04, albedo, metalness);
    const float3 diffuseColor = lerp((float3) 0.00, albedo, 1 - metalness);

    const float3 ambiance = EvaluateAmbiance(environmentTexture, normal, vertexNormal, toEye, perceptualRoughness, metalness, albedo, ambientOcclusion, diffuseColor, specularColor);
    const float3 directionalLight = EvaluateDirectionalLight(diffuseColor, specularColor, normal, perceptualRoughness, directionalLightColor.rgb * directionalLightColor.a, toDirectionalLight.xyz, toEye.xyz);
    const float3 emissive = albedo * emissiveData;
    const float3 radiance = (ambiance * ssao) + directionalLight * (1.0f - ShadowFactor(worldPosition, directionalLightPosition.xyz, toDirectionalLightView, toDirectionalLightProjection, shadowDepthTexture, shadowSampler, directionalLightShadowMapResolution)) + emissive;

    output.myColor.rgb = radiance;
    output.myColor.a = 1.0f;
    return output;
}
