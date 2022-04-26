// Copyright 2022 Team Havtorn. All Rights Reserved.

#include "Includes/PBRDeferredAmbiance.hlsli"
#include "Includes/DeferredPBRFunctions.hlsli"
#include "Includes/ShadowSampling.hlsli"

PixelOutput main(VertexToPixel input)
{
    PixelOutput output;

    const float depth = PixelShader_Exists(input.UV).r;
    if (depth == 1)
    {
        output.Color = GBuffer_Albedo(input.UV);
        return output;
    }
    
    float3 worldPosition = PixelShader_WorldPosition(input.UV).rgb;
    float3 toEye = normalize( cameraPosition.xyz - worldPosition.xyz);
    float3 albedo = GBuffer_Albedo(input.UV).rgb;
    albedo = GammaToLinear(albedo);
    const float3 normal = GBuffer_Normal(input.UV).xyz;
    const float3 vertexNormal = GBuffer_VertexNormal(input.UV).xyz;
    const float ambientOcclusion = GBuffer_AmbientOcclusion(input.UV);
    const float metalness = GBuffer_Metalness(input.UV);
    const float perceptualRoughness = GBuffer_PerceptualRoughness(input.UV);
    const float emissiveData = GBuffer_Emissive(input.UV);
    const float ssao = PixelShader_SSAO(input.UV);

    const float3 specularColor = lerp((float3) 0.04, albedo, metalness);
    const float3 diffuseColor = lerp((float3) 0.00, albedo, 1 - metalness);

    const float3 ambiance = EvaluateAmbiance(environmentTexture, normal, vertexNormal, toEye, perceptualRoughness, metalness, albedo, ambientOcclusion, diffuseColor, specularColor);
    const float3 directionalLight = EvaluateDirectionalLight(diffuseColor, specularColor, normal, perceptualRoughness, directionalLightColor.rgb * directionalLightColor.a, toDirectionalLight.xyz, toEye.xyz);
    const float3 emissive = albedo * emissiveData;
    const float3 radiance = (ambiance * ssao) + directionalLight * (1.0f - ShadowFactor(worldPosition, directionalLightPosition.xyz, toDirectionalLightView, toDirectionalLightProjection, shadowDepthTexture, shadowSampler, directionalLightShadowMapResolution)) + emissive;

    output.Color.rgb = radiance;
    output.Color.a = 1.0f;
    return output;
}
