// Copyright 2022 Team Havtorn. All Rights Reserved.

#include "Includes/DeferredSamplingFunctions.hlsli"
#include "Includes/DeferredPBRFunctions.hlsli"
#include "Includes/MathHelpers.hlsli"
#include "Includes/PointLightShaderStructs.hlsli"
#include "Includes/ShadowSampling.hlsli"

PixelOutput main(PointLightVertexToPixel input)
{
    PixelOutput output;

    const float2 screenUV = (input.myUV.xy / input.myUV.z) * 0.5f + 0.5f;

    const float depth = PixelShader_Exists(screenUV).r;
    if (depth == 1)
    {
       discard;
    }
    
    float3 worldPosition = PixelShader_WorldPosition(screenUV).rgb;
    float3 toEye = normalize(CameraPosition.xyz - worldPosition.xyz);
    float3 albedo = GBuffer_Albedo(screenUV).rgb;
    albedo = GammaToLinear(albedo);
    const float3 normal = GBuffer_Normal(screenUV).xyz;
    const float metalness = GBuffer_Metalness(screenUV);
    const float perceptualRoughness = GBuffer_PerceptualRoughness(screenUV);

    const float3 specularColor = lerp((float3) 0.04, albedo, metalness);
    const float3 diffuseColor = lerp((float3) 0.00, albedo, 1 - metalness);
    
    float3 toLight = pointLightPositionAndRange.xyz - worldPosition.xyz;
    const float lightDistance = length(toLight);
    toLight = normalize(toLight);

	int shadowmapViewIndex = GetShadowmapViewIndex(worldPosition, pointLightPositionAndRange.xyz);
    SShadowmapViewData shadowData = ShadowmapViewData[shadowmapViewIndex];
    const float3 shadowFactor = ShadowFactor(worldPosition, shadowData.ShadowmapPosition.xyz, shadowData.ToShadowMapView, shadowData.ToShadowMapProjection, shadowDepthTexture, shadowSampler, shadowData.ShadowmapResolution, shadowData.ShadowAtlasResolution, shadowData.ShadowmapStartingUV);
    const float3 pointLight = EvaluatePointLight(diffuseColor, specularColor, normal, perceptualRoughness, pointLightColorAndIntensity.rgb * pointLightColorAndIntensity.a, pointLightPositionAndRange.w, toLight.xyz, lightDistance, toEye.xyz);
    const float3 radiance = pointLight * (1.0f - shadowFactor);
    
    output.Color.rgb = radiance;
    output.Color.a = 1.0f;
    return output;
}