// Copyright 2022 Team Havtorn. All Rights Reserved.

#include "Includes/DeferredSamplingFunctions.hlsli"
#include "Includes/DeferredPBRFunctions.hlsli"
#include "Includes/BoxLightShaderStructs.hlsli"
#include "Includes/MathHelpers.hlsli"
#include "Includes/ShadowSampling.hlsli"

PixelOutput main(BoxLightVertexToPixel input)
{
    PixelOutput output;

    float2 screenUV = (input.myUV.xy / input.myUV.z) * 0.5f + 0.5f;

    const float depth = PixelShader_Exists(screenUV).r;
    if (depth == 1)
    {
        output.myColor = GBuffer_Albedo(screenUV);
        return output;
    }
    
    float3 worldPosition = /*input.myWorldPosition*/PixelShader_WorldPosition(screenUV).rgb;
    
    
    //float3 clipSpace = input.myClipSpacePosition;
    //clipSpace.y *= -1.0f;
    //float2 screenSpaceUV = (clipSpace.xy / clipSpace.z) * 0.5f + 0.5f;
    
    float z = depthTexture.Sample(defaultSampler, screenUV).r;
    float x = screenUV.x * 2.0f - 1;
    float y = (1 - screenUV.y) * 2.0f - 1;
    const float4 projectedPos = float4(x, y, z, 1.0f);
    float4 viewSpacePos = mul(toCameraFromProjection, projectedPos);
    viewSpacePos /= viewSpacePos.w;
    const float4 worldPosFromDepth = mul(toWorldFromCamera, viewSpacePos);
    
    float4 objectPosition = mul(toBoxLightObject, worldPosFromDepth);
    
    clip(0.5f - abs(objectPosition.xy));
    clip(1.0f - /*abs*/(objectPosition.z));

    const float3 toEye = normalize(cameraPosition.xyz - worldPosition.xyz);
    float3 albedo = GBuffer_Albedo(screenUV).rgb;
    albedo = GammaToLinear(albedo);
    const float3 normal = GBuffer_Normal(screenUV).xyz;
    //float3 vertexNormal = GBuffer_VertexNormal(screenUV).xyz;
    //float ambientOcclusion = GBuffer_AmbientOcclusion(screenUV);
    const float metalness = GBuffer_Metalness(screenUV);
    const float perceptualRoughness = GBuffer_PerceptualRoughness(screenUV);
    //float emissiveData = GBuffer_Emissive(screenUV);

    const float3 specularColor = lerp((float3) 0.04, albedo, metalness);
    const float3 diffuseColor = lerp((float3) 0.00, albedo, 1 - metalness);
    
    float3 toLight = boxLightPositionAndRange.xyz - worldPosition.xyz;
    const float lightDistance = length(toLight);
    toLight = normalize(toLight);
    const float3 directionalLight = EvaluateBoxLight(diffuseColor, specularColor, normal, perceptualRoughness, boxLightColorAndIntensity.rgb * boxLightColorAndIntensity.w, boxLightPositionAndRange.w, toLight, lightDistance, toEye, boxLightDirection.xyz);
    const float3 radiance = directionalLight * (1.0f - ShadowFactor(worldPosition, boxLightPositionAndRange.xyz, toBoxLightView, toBoxLightProjection, shadowDepthTexture, shadowSampler, boxLightShadowmapResolution));

    output.myColor.rgb = radiance;
    output.myColor.a = 1.0f;
    return output;
}
