// Copyright 2022 Team Havtorn. All Rights Reserved.

static float PI = 3.14159265;

float3 Diffuse(float3 pAlbedo)
{
    return pAlbedo / PI;
}

float NormalDistribution_GGX(float a, float NdH)
{
    // Isotropic ggx
    float a2 = a * a;
    float NdH2 = NdH * NdH;
    
    float denominator = NdH2 * (a2 - 1.0f) + 1.0f;
    denominator *= denominator;
    denominator *= PI;
    
    return a2 / denominator;
}

float Specular_D(float a, float NdH)
{
    return NormalDistribution_GGX(a, NdH);
}

float Geometric_Smith_Schlick_GGX(float a, float NdV, float NdL)
{
    // Smith schlick-GGX
    float k = a * 0.5f;
    float GV = NdV / (NdV * (1 - k) + k);
    float GL = NdL / (NdL * (1 - k) + k);
    
    return GV * GL;
}

float Specular_G(float a, float NdV, float NdL, float NdH, float VdH, float LdV)
{
    return Geometric_Smith_Schlick_GGX(a, NdV, NdL);
}

float3 Fresnel_Schlick(float3 specularColor, float3 h, float3 v)
{
    return (specularColor + (1.0f - specularColor) * pow((1.0f - saturate(dot(v, h))), 5));
}

float3 Specular_F(float3 specularColor, float3 h, float3 v)
{
    return Fresnel_Schlick(specularColor, h, v);
}

float3 Specular(float3 specularColor, float3 h, float3 v, float3 l, float a, float NdL, float NdV, float NdH, float VdH, float LdV)
{
    return ((Specular_D(a, NdH) * Specular_G(a, NdV, NdL, NdH, VdH, LdV)) * Specular_F(specularColor, h, v)) / (4.0f * NdL * NdV + 0.0001f);
}

float3 EvaluateDirectionalLight(float3 albedoColor, float3 specularColor, float3 normal, float roughness, float3 lightColor, float3 lightDir, float3 viewDir)
{
    // Compute some useful values
    float NdL = saturate(dot(normal, lightDir));
    float lambert = NdL; // Angle attenuation
    float NdV = saturate(dot(normal, viewDir));
    float3 h = normalize(lightDir + viewDir);
    float NdH = saturate(dot(normal, h));
    float VdH = saturate(dot(viewDir, h));
    float LdV = saturate(dot(lightDir, viewDir));
    float a = max(0.001f, roughness * roughness);
    
    float3 cDiff = Diffuse(albedoColor);
    float3 cSpec = Specular(specularColor, h, viewDir, lightDir, a, NdL, NdV, NdH, VdH, LdV);
    
    return saturate(lightColor * lambert * (cDiff * (1.0f - cSpec) + cSpec) * PI);
}

float3 EvaluatePointLight(float3 albedoColor, float3 specularColor, float3 normal, float roughness, float3 lightColor, float lightIntensity, float lightRange, float3 lightPos, float3 toEye, float3 pixelPos)
{
    float3 toLight = lightPos.xyz - pixelPos.xyz;
    float lightDistance = length(toLight);
    toLight = normalize(toLight);
    float NdL = saturate(dot(normal, toLight));
    float lambert = NdL; //angle attenuation
    float NdV = saturate(dot(normal, toEye));
    float3 h = normalize(toLight + toEye);
    float NdH = saturate(dot(normal, h));
    float VdH = saturate(dot(toEye, h));
    float LdV = saturate(dot(toLight, toEye));
    float a = max(0.001f, roughness * roughness);
    
    float3 cDiff = Diffuse(albedoColor);
    float3 cSpec = Specular(specularColor, h, toEye, toLight, a, NdL, NdV, NdH, VdH, LdV);
    
    float linearAttenuation = lightDistance / lightRange;
    linearAttenuation = 1.0f - linearAttenuation;
    linearAttenuation = saturate(linearAttenuation);
    float physicalAttenuation = saturate(1.0f / (lightDistance * lightDistance));
    float attenuation = lambert * linearAttenuation * physicalAttenuation;
    
    return saturate(lightColor * lightIntensity * lambert * linearAttenuation * physicalAttenuation * ((cDiff * (1.0f - cSpec) + cSpec) * PI));
}

float3 EvaluatePointLight(float3 diffuseColor, float3 specularColor, float3 normal, float roughness, float3 intensityScaledColor, float3 lightRange, float3 toLight, float3 lightDistance, float3 toEye)
{
    float NdL = saturate(dot(normal, toLight));
    float lambert = NdL;
    float NdV = saturate(dot(normal, toEye));
    float3 h = normalize(toLight + toEye);
    float NdH = saturate(dot(normal, h));
    float VdH = saturate(dot(toEye, h));
    float LdV = saturate(dot(toLight, toEye));
    float a = max(0.001f, roughness * roughness);
    
    float3 cDiff = Diffuse(diffuseColor);
    float3 cSpec = Specular(specularColor, h, toEye, toLight, a, NdL, NdV, NdH, VdH, LdV);
    
    float linearAttenuation = lightDistance / lightRange;
    linearAttenuation = 1.0f - linearAttenuation;
    linearAttenuation = saturate(linearAttenuation);
    float physicalAttenuation = saturate(1.0f / (lightDistance * lightDistance));
    float attenuation = lambert * linearAttenuation * physicalAttenuation;
    
    return saturate(intensityScaledColor * attenuation * ((cDiff * (1.0f - cSpec) + cSpec) * PI));
}

float3 EvaluateSpotLight(float3 diffuseColor, float3 specularColor, float3 normal, float roughness, float3 intensityScaledColor, float3 lightRange, float3 toLight, float3 lightDistance, float3 toEye, float3 lightDir, float angleExponent, float2 innerOuterAngle)
{
    float NdL = saturate(dot(normal, toLight));
    float lambert = NdL;
    float NdV = saturate(dot(normal, toEye));
    float3 h = normalize(toLight + toEye);
    float NdH = saturate(dot(normal, h));
    float VdH = saturate(dot(toEye, h));
    float LdV = saturate(dot(toLight, toEye));
    float a = max(0.001f, roughness * roughness);
    
    float3 cDiff = Diffuse(diffuseColor);
    float3 cSpec = Specular(specularColor, h, toEye, toLight, a, NdL, NdV, NdH, VdH, LdV);
    
    float linearAttenuation = lightDistance / lightRange;
    linearAttenuation = 1.0f - linearAttenuation;
    linearAttenuation = saturate(linearAttenuation);
    float physicalAttenuation = saturate(1.0f / (lightDistance * lightDistance));
    float attenuation = lambert * linearAttenuation * physicalAttenuation;
    
    float3 finalColor = 0.0f;
    finalColor += intensityScaledColor * ((cDiff * (1.0f - cSpec) + cSpec) * PI);
    
    finalColor *= linearAttenuation * physicalAttenuation;
    
    //finalColor *= pow(max(dot(lightDir, -toLight), 0.0f), angleExponent);
    float inner = sin(innerOuterAngle.x);
    float outer = sin(innerOuterAngle.y);
    
    finalColor *= saturate((dot(lightDir, -toLight) - outer / (inner - outer)));
    return finalColor;
}

float3 EvaluateBoxLight(float3 diffuseColor, float3 specularColor, float3 normal, float roughness, float3 intensityScaledColor, float3 lightRange, float3 toLight, float3 lightDistance, float3 toEye, float3 lightDir)
{
    float NdL = saturate(dot(normal, toLight));
    float lambert = NdL;
    float NdV = saturate(dot(normal, toEye));
    float3 h = normalize(toLight + toEye);
    float NdH = saturate(dot(normal, h));
    float VdH = saturate(dot(toEye, h));
    float LdV = saturate(dot(toLight, toEye));
    float a = max(0.001f, roughness * roughness);
    
    float3 cDiff = Diffuse(diffuseColor);
    float3 cSpec = Specular(specularColor, h, toEye, toLight, a, NdL, NdV, NdH, VdH, LdV);
    
    float linearAttenuation = lightDistance / lightRange;
    linearAttenuation = 1.0f - linearAttenuation;
    linearAttenuation = saturate(linearAttenuation);
    float physicalAttenuation = saturate(1.0f / (lightDistance * lightDistance));
    float attenuation = lambert * linearAttenuation * physicalAttenuation;
    
    float3 finalColor = 0.0f;
    finalColor += intensityScaledColor * ((cDiff * (1.0f - cSpec) + cSpec) * PI);
    
    finalColor *= linearAttenuation * physicalAttenuation;
    //float3 dirToLight = dot(lightDir, -toLight);
    //finalColor *= saturate(dirToLight);
    finalColor *= lambert;
    
    return saturate(finalColor);
}