// Copyright 2022 Team Havtorn. All Rights Reserved.

#include "Includes/DeferredSamplingFunctions.hlsli"
#include "Includes/DeferredPBRFunctions.hlsli"
#include "Includes/MathHelpers.hlsli"
#include "Includes/SpotLightShaderStructs.hlsli"

PixelOutput main(SpotLightVertexToPixel input)
{
    PixelOutput output;

    const float2 screenUV = (input.myUV.xy / input.myUV.z) * 0.5f + 0.5f;

    const float depth = PixelShader_Exists(screenUV).r;
    if (depth == 1)
    {
        discard;
    }
    
    float3 worldPosition = PixelShader_WorldPosition(screenUV).rgb;
    const float3 lightDirection = normalize(-spotLightDirectionAndAngleExponent.xyz);
    float3 toLight = spotLightPositionAndRange.xyz - worldPosition.xyz;
    const float lightDistance = length(toLight);

    const float theta = dot(normalize(toLight), lightDirection);
    const float cutOff = cos(radians(myInnerOuterAngle.x));
    const float outerCutOff = cos(radians(myInnerOuterAngle.y));
    const float epsilon = cutOff - outerCutOff;
    const float intensity = clamp(((theta - outerCutOff) / epsilon), 0.0f, 1.0f);
    
    float3 toEye = normalize(spotLightPositionAndRange.xyz - worldPosition.xyz);
    float3 albedo = GBuffer_Albedo(screenUV).rgb;
    albedo = GammaToLinear(albedo);
    const float3 normal = GBuffer_Normal(screenUV).xyz;
    const float metalness = GBuffer_Metalness(screenUV);
    const float perceptualRoughness = GBuffer_PerceptualRoughness(screenUV);

    const float3 specularColor = lerp((float3) 0.04, albedo, metalness);
    const float3 diffuseColor = lerp((float3) 0.00, albedo, 1 - metalness);
    
    toLight = normalize(toLight);
    const float3 radiance = EvaluatePointLight(diffuseColor * intensity, specularColor * intensity, normal, perceptualRoughness, spotLightColorAndIntensity.rgb * spotLightColorAndIntensity.a, spotLightPositionAndRange.w, toLight.xyz, lightDistance, toEye.xyz);
    //float3 radiance = EvaluateSpotLight(diffuseColor * intensity, specularColor * intensity, normal, perceptualRoughness, spotLightColorAndIntensity.rgb * spotLightColorAndIntensity.a, spotLightPositionAndRange.w, toLight.xyz, lightDistance, toEye.xyz, spotLightDirectionAndAngleExponent.xyz, spotLightDirectionAndAngleExponent.w, myInnerOuterAngle);
    
    output.myColor.rgb = radiance;
    output.myColor.a = 1.0f;
    return output;
}