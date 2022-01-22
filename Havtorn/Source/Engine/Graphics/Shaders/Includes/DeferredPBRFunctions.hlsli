// Copyright 2022 Team Havtorn. All Rights Reserved.

static float PI = 3.14159265;

float3 Diffuse(float3 pAlbedo)
{
    return pAlbedo / PI;
}

float NormalDistribution_GGX(float a, float NdH)
{
    // Isotropic ggx
    const float a2 = a * a;
    const float NdH2 = NdH * NdH;
    
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
    const float k = a * 0.5f;
    const float GV = NdV / (NdV * (1 - k) + k);
    const float GL = NdL / (NdL * (1 - k) + k);
    
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
    const float NdL = saturate(dot(normal, lightDir));
    const float lambert = NdL; // Angle attenuation
    const float NdV = saturate(dot(normal, viewDir));
    const float3 h = normalize(lightDir + viewDir);
    const float NdH = saturate(dot(normal, h));
    const float VdH = saturate(dot(viewDir, h));
    const float LdV = saturate(dot(lightDir, viewDir));
    const float a = max(0.001f, roughness * roughness);

    const float3 cDiff = Diffuse(albedoColor);
    const float3 cSpec = Specular(specularColor, h, viewDir, lightDir, a, NdL, NdV, NdH, VdH, LdV);
    
    return saturate(lightColor * lambert * (cDiff * (1.0f - cSpec) + cSpec) * PI);
}

float3 EvaluatePointLight(float3 albedoColor, float3 specularColor, float3 normal, float roughness, float3 lightColor, float lightIntensity, float lightRange, float3 lightPos, float3 toEye, float3 pixelPos)
{
    float3 toLight = lightPos.xyz - pixelPos.xyz;
    const float lightDistance = length(toLight);
    toLight = normalize(toLight);
    const float NdL = saturate(dot(normal, toLight));
    const float lambert = NdL; //angle attenuation
    const float NdV = saturate(dot(normal, toEye));
    const float3 h = normalize(toLight + toEye);
    const float NdH = saturate(dot(normal, h));
    const float VdH = saturate(dot(toEye, h));
    const float LdV = saturate(dot(toLight, toEye));
    const float a = max(0.001f, roughness * roughness);

    const float3 cDiff = Diffuse(albedoColor);
    const float3 cSpec = Specular(specularColor, h, toEye, toLight, a, NdL, NdV, NdH, VdH, LdV);
    
    float linearAttenuation = lightDistance / lightRange;
    linearAttenuation = 1.0f - linearAttenuation;
    linearAttenuation = saturate(linearAttenuation);
    const float physicalAttenuation = saturate(1.0f / (lightDistance * lightDistance));
    //float attenuation = lambert * linearAttenuation * physicalAttenuation;
    
    return saturate(lightColor * lightIntensity * lambert * linearAttenuation * physicalAttenuation * ((cDiff * (1.0f - cSpec) + cSpec) * PI));
}

float3 EvaluatePointLight(float3 diffuseColor, float3 specularColor, float3 normal, float roughness, float3 intensityScaledColor, float lightRange, float3 toLight, float lightDistance, float3 toEye)
{
	const float NdL = saturate(dot(normal, toLight));
	const float lambert = NdL;
	const float NdV = saturate(dot(normal, toEye));
	const float3 h = normalize(toLight + toEye);
	const float NdH = saturate(dot(normal, h));
	const float VdH = saturate(dot(toEye, h));
	const float LdV = saturate(dot(toLight, toEye));
	const float a = max(0.001f, roughness * roughness);

	const float3 cDiff = Diffuse(diffuseColor);
	const float3 cSpec = Specular(specularColor, h, toEye, toLight, a, NdL, NdV, NdH, VdH, LdV);
    
    float linearAttenuation = lightDistance / lightRange;
    linearAttenuation = 1.0f - linearAttenuation;
    linearAttenuation = saturate(linearAttenuation);
	const float physicalAttenuation = saturate(1.0f / (lightDistance * lightDistance));
	const float attenuation = lambert * linearAttenuation * physicalAttenuation;
    
    return saturate(intensityScaledColor * attenuation * ((cDiff * (1.0f - cSpec) + cSpec) * PI));
}

float3 EvaluateSpotLight(float3 diffuseColor, float3 specularColor, float3 normal, float roughness, float3 intensityScaledColor, float lightRange, float3 toLight, float lightDistance, float3 toEye, float3 lightDir, float angleExponent, float2 innerOuterAngle)
{
	const float NdL = saturate(dot(normal, toLight));
	const float lambert = NdL;
	const float NdV = saturate(dot(normal, toEye));
	const float3 h = normalize(toLight + toEye);
	const float NdH = saturate(dot(normal, h));
	const float VdH = saturate(dot(toEye, h));
	const float LdV = saturate(dot(toLight, toEye));
	const float a = max(0.001f, roughness * roughness);

	const float3 cDiff = Diffuse(diffuseColor);
	const float3 cSpec = Specular(specularColor, h, toEye, toLight, a, NdL, NdV, NdH, VdH, LdV);
    
    float linearAttenuation = lightDistance / lightRange;
    linearAttenuation = 1.0f - linearAttenuation;
    linearAttenuation = saturate(linearAttenuation);
	const float physicalAttenuation = saturate(1.0f / (lightDistance * lightDistance));
    //float attenuation = lambert * linearAttenuation * physicalAttenuation;
    
    float3 finalColor = 0.0f;
    finalColor += intensityScaledColor * ((cDiff * (1.0f - cSpec) + cSpec) * PI);
    
    finalColor *= linearAttenuation * physicalAttenuation;
    
    //finalColor *= pow(max(dot(lightDir, -toLight), 0.0f), angleExponent);
	const float inner = sin(innerOuterAngle.x);
	const float outer = sin(innerOuterAngle.y);
    
    finalColor *= saturate((dot(lightDir, -toLight) - outer / (inner - outer)));
    return finalColor;
}

float3 EvaluateBoxLight(float3 diffuseColor, float3 specularColor, float3 normal, float roughness, float3 intensityScaledColor, float lightRange, float3 toLight, float lightDistance, float3 toEye, float3 lightDir)
{
	const float NdL = saturate(dot(normal, toLight));
	const float lambert = NdL;
	const float NdV = saturate(dot(normal, toEye));
	const float3 h = normalize(toLight + toEye);
	const float NdH = saturate(dot(normal, h));
	const float VdH = saturate(dot(toEye, h));
	const float LdV = saturate(dot(toLight, toEye));
	const float a = max(0.001f, roughness * roughness);

	const float3 cDiff = Diffuse(diffuseColor);
	const float3 cSpec = Specular(specularColor, h, toEye, toLight, a, NdL, NdV, NdH, VdH, LdV);
    
    float linearAttenuation = lightDistance / lightRange;
    linearAttenuation = 1.0f - linearAttenuation;
    linearAttenuation = saturate(linearAttenuation);
	const float physicalAttenuation = saturate(1.0f / (lightDistance * lightDistance));
    //float attenuation = lambert * linearAttenuation * physicalAttenuation;
    
    float3 finalColor = 0.0f;
    finalColor += intensityScaledColor * ((cDiff * (1.0f - cSpec) + cSpec) * PI);
    
    finalColor *= linearAttenuation * physicalAttenuation;
    //float3 dirToLight = dot(lightDir, -toLight);
    //finalColor *= saturate(dirToLight);
    finalColor *= lambert;
    
    return saturate(finalColor);
}