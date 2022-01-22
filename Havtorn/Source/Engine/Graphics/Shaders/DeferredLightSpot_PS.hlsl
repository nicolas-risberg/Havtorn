// Copyright 2022 Team Havtorn. All Rights Reserved.

#include "Includes/DeferredSamplingFunctions.hlsli"
#include "Includes/DeferredPBRFunctions.hlsli"
#include "Includes/SpotLightShaderStructs.hlsli"

PixelOutput main(SpotLightVertexToPixel input)
{
    PixelOutput output;
    
    float2 screenUV = (input.myUV.xy / input.myUV.z) * 0.5f + 0.5f;
    
    float depth = PixelShader_Exists(screenUV).r;
    if (depth == 1)
    {
        discard;
    }
    
    float3 worldPosition = PixelShader_WorldPosition(screenUV).rgb;
    float3 lightDirection = normalize(-spotLightDirectionAndAngleExponent.xyz);
    float3 toLight = spotLightPositionAndRange.xyz - worldPosition.xyz;
    float lightDistance = length(toLight);
    
    float theta = dot(normalize(toLight), lightDirection);
    float cutOff = cos(radians(myInnerOuterAngle.x));
    float outerCutOff = cos(radians(myInnerOuterAngle.y));
    float epsilon = cutOff - outerCutOff;
    float intensity = clamp(((theta - outerCutOff) / epsilon), 0.0f, 1.0f);
    
    float3 toEye = normalize(spotLightPositionAndRange.xyz - worldPosition.xyz);
    float3 albedo = GBuffer_Albedo(screenUV).rgb;
    albedo = GammaToLinear(albedo);
    float3 normal = GBuffer_Normal(screenUV).xyz;
    float metalness = GBuffer_Metalness(screenUV);
    float perceptualRoughness = GBuffer_PerceptualRoughness(screenUV);
    
    float3 specularColor = lerp((float3) 0.04, albedo, metalness);
    float3 diffuseColor = lerp((float3) 0.00, albedo, 1 - metalness);
    
    toLight = normalize(toLight);
    float3 radiance = EvaluatePointLight(diffuseColor * intensity, specularColor * intensity, normal, perceptualRoughness, spotLightColorAndIntensity.rgb * spotLightColorAndIntensity.a, spotLightPositionAndRange.w, toLight.xyz, lightDistance, toEye.xyz);
    //float3 radiance = EvaluateSpotLight(diffuseColor * intensity, specularColor * intensity, normal, perceptualRoughness, spotLightColorAndIntensity.rgb * spotLightColorAndIntensity.a, spotLightPositionAndRange.w, toLight.xyz, lightDistance, toEye.xyz, spotLightDirectionAndAngleExponent.xyz, spotLightDirectionAndAngleExponent.w, myInnerOuterAngle);
    
    output.myColor.rgb = radiance;
    output.myColor.a = 1.0f;
    return output;
}