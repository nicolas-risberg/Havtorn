// Copyright 2022 Team Havtorn. All Rights Reserved.

#include "Includes/DeferredSamplingFunctions.hlsli"
#include "Includes/DeferredPBRFunctions.hlsli"
#include "Includes/MathHelpers.hlsli"
#include "Includes/PointLightShaderStructs.hlsli"

PixelOutput main(/*PointLightGeometryToPixel*/PointLightVertexToPixel input)
{
    PixelOutput output;

    const float2 screenUV = (input.myUV.xy / input.myUV.z) * 0.5f + 0.5f;
    //float2 screenUV = input.myUV;

    const float depth = PixelShader_Exists(screenUV).r;
    if (depth == 1)
    {
       discard;
    }
    
    float3 worldPosition = PixelShader_WorldPosition(screenUV).rgb;
    float3 toEye = normalize(cameraPosition.xyz - worldPosition.xyz);
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
    const float3 radiance = EvaluatePointLight(diffuseColor, specularColor, normal, perceptualRoughness, pointLightColorAndIntensity.rgb * pointLightColorAndIntensity.a, pointLightPositionAndRange.w, toLight.xyz, lightDistance, toEye.xyz);
    
    output.myColor.rgb = radiance;
    output.myColor.a = 1.0f;
    return output;
}