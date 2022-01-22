// Copyright 2022 Team Havtorn. All Rights Reserved.

#include "Includes/PBRAmbience.hlsli"
#include "Includes/PBRDirectionalLight.hlsli"
#include "Includes/ShadowSampling.hlsli"

float3 EvaluatePointLight(float3 albedoColor, float3 specularColor, float3 normal, float roughness, float3 lightColor, float lightIntensity, float lightRange, float3 lightPos, float3 toEye, float3 pixelPos)
{
    float3 toLight = lightPos.xyz - pixelPos.xyz;
    const float lightDistance = length(toLight);
    toLight = normalize(toLight);
    const float NdL = saturate(dot(normal, toLight));

    const float lambert = NdL;
    const float NdV = saturate(dot(normal, toEye));
    const float3 h = normalize(toLight + toEye);
    const float NdH = saturate(dot(normal, h));
    const float VdH = saturate(dot(toEye, h));
    const float LdV = saturate(dot(toLight, toEye));
    const float a = max(0.001f, roughness * roughness);

    const float3 cDiff = Diffuse(albedoColor);
    const float3 cSPec = Specular(specularColor, h, toEye, toLight, a, NdL, NdV, NdH, VdH, LdV);
    
    float linearAttenuation = lightDistance / lightRange;
    linearAttenuation = 1.0f - linearAttenuation;
    linearAttenuation = saturate(linearAttenuation);
    const float physicalAttenuation = saturate(1.0f / (lightDistance * lightDistance));
    //float attenuation = lambert * linearAttenuation * physicalAttenuation;
    
    return saturate(lightColor * lightIntensity * lambert * linearAttenuation * physicalAttenuation * ((cDiff * (1.0f - cSPec) + cSPec) * PI));
}

PixelOutPut main(VertexToPixel input)
{
    PixelOutPut output;
    
    float3 toEye = normalize(cameraPosition.xyz - input.myWorldPosition.xyz);
    float4 albedo = PixelShader_Color(input).myColor.rgba;
    
    clip(albedo.a - 0.5f);
    
    //albedo.rgb = GammaToLinear(albedo.rgb);
    float3 normal = PixelShader_Normal(input).myColor.xyz;
    
    if (myNumberOfDetailNormals > 0)
    { // get from ModelData when rendering
        float detailNormalStrength = PixelShader_DetailNormalStrength(input);
        const float strengthMultiplier = DetailStrengthDistanceMultiplier(cameraPosition.xyz, input.myWorldPosition.xyz); // should change based on distance to camera
        float3 detailNormal;
        
        // Blend based on detail normal strength
        // X3512 Sampler array index must be literal expression => DETAILNORMAL_#
        // Sampled detail normal strength value: 
        //      0.1f - 0.24f    == DETAILNORMAL_1
        //      0.26f - 0.49f   == DETAILNORMAL_2
        //      0.51f - 0.74f   == DETAILNORMAL_3
        //      0.76f - 1.0f    == DETAILNORMAL_4
        // Note! This if-chain exists in 3 shaders: PBRPixelShader, GBufferPixelShader and DeferredRenderPassGBufferPixelShader
        // Make this better please
        if (detailNormalStrength > DETAILNORMAL_4_STR_RANGE_MIN)
        {
            detailNormal = PixelShader_DetailNormal(input, DETAILNORMAL_4).myColor.xyz;
            detailNormalStrength = (detailNormalStrength - DETAILNORMAL_4_STR_RANGE_MIN + 0.01f) / DETAILNORMAL_STR_RANGE_DIFF;
        }
        else if (detailNormalStrength > DETAILNORMAL_3_STR_RANGE_MIN)
        {
            detailNormal = PixelShader_DetailNormal(input, DETAILNORMAL_3).myColor.xyz;
            detailNormalStrength = (detailNormalStrength - DETAILNORMAL_3_STR_RANGE_MIN + 0.01f) / DETAILNORMAL_STR_RANGE_DIFF;
        }
        else if (detailNormalStrength > DETAILNORMAL_2_STR_RANGE_MIN)
        {
            detailNormal = PixelShader_DetailNormal(input, DETAILNORMAL_2).myColor.xyz;
            detailNormalStrength = (detailNormalStrength - DETAILNORMAL_2_STR_RANGE_MIN + 0.01f) / DETAILNORMAL_STR_RANGE_DIFF;
        }
        else
        {
            detailNormal = PixelShader_DetailNormal(input, DETAILNORMAL_1).myColor.xyz;
            detailNormalStrength = (detailNormalStrength - DETAILNORMAL_1_STR_RANGE_MIN + 0.01f) / DETAILNORMAL_STR_RANGE_DIFF;
        }
        
        detailNormal = SetDetailNormalStrength(detailNormal, detailNormalStrength, strengthMultiplier);
        normal = normal * 0.5 + 0.5;
        detailNormal = detailNormal * 0.5 + 0.5;
        normal = BlendRNM(normal, detailNormal);
        
        // Blend all 4
        //for (int i = 0; i < myNumberOfDetailNormals; ++i)
        //{
        //    detailNormal = PixelShader_DetailNormal(input, i).myColor.xyz;
        //    detailNormal = SetDetailNormalStrength(detailNormal, detailNormalStrength, strengthMultiplier);
        //    normal = normal * 0.5 + 0.5;
        //    detailNormal = detailNormal * 0.5 + 0.5;
        //    normal = BlendRNM(normal, detailNormal);
        //}
    } // End of if

    const float3x3 tangentSpaceMatrix = float3x3(normalize(input.myTangent.xyz), normalize(input.myBiNormal.xyz), normalize(input.myNormal.xyz));
    normal = mul(normal.xyz, tangentSpaceMatrix);
    normal = normalize(normal);

    const float ambientocclusion = PixelShader_AmbientOcclusion(input).myColor.b;
    const float metalness = PixelShader_Metalness(input).myColor.r;
    float perceptualroughness = PixelShader_PerceptualRoughness(input).myColor.g;
    perceptualroughness = pow(abs(perceptualroughness), 1.0f / 1.1f); // from tga modelviewer // To mimic substance painters more blury roughness
    const float emissivedata = PixelShader_Emissive(input).myColor.b;

    const float3 specularcolor = lerp((float3) 0.04, albedo.rgb, metalness);
    const float3 color = lerp((float3) 0.00, albedo.rgb, 1 - metalness);

    const float3 ambience = EvaluateAmbience(environmentTexture, normal, normalize(input.myNormal.xyz), toEye, perceptualroughness, ambientocclusion, color, specularcolor);
    float3 directionallight = EvaluateDirectionalLight(color, specularcolor, normal, perceptualroughness, directionalLightColor.xyz, toDirectionalLight.xyz, toEye.xyz);
    directionallight *= directionalLightColor.w;
 
    float3 pointLights = 0;
    for (unsigned int index = 0; index < myNumberOfUsedPointLights; index++)
    {
        PointLight currentLight = myPointLights[index];
        pointLights += EvaluatePointLight(color, specularcolor, normal, perceptualroughness, currentLight.myColorAndRange.rgb, currentLight.myPositionAndIntensity.w, currentLight.myColorAndRange.a, currentLight.myPositionAndIntensity.xyz, toEye, input.myWorldPosition.xyz);
    }

    const float3 emissive = albedo.rgb * emissivedata; // Maybe add cool multiplier?? // Aki 2021
    const float3 radiance = ambience + directionallight * (1.0f - ShadowFactor(input.myWorldPosition.xyz, directionalLightPosition.xyz, toDirectionalLightView, toDirectionalLightProjection, shadowDepthTexture, shadowSampler, directionalLightShadowMapResolution)) + pointLights + emissive;
   
    output.myColor.rgb = /*LinearToGamma(*/radiance/*)*/;
    output.myColor.a = albedo.w;
      
    return output;
}