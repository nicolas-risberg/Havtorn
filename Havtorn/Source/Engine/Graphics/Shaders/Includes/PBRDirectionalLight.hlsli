// Copyright 2022 Team Havtorn. All Rights Reserved.

static float PI = 3.14;
float3 Diffuse(float3 pAlbedo)
{
    return pAlbedo / PI;
}

float NormalDistribution_GGX(float a, float NdH)
{
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
    return ((Specular_D(a, NdH) * Specular_G(a, NdV, NdL, NdH, VdH, LdV)) * Specular_F(specularColor, v, h)) / (4.0f * NdL * NdV + 0.0001f);
}

float3 EvaluateDirectionalLight(float3 albedoColor, float3 specularColor, float3 normal, float roughness, float3 lightColor, float3 lightDir, float3 viewDir)
{
	const float NdL = saturate(dot(normal, lightDir));
	const float lambert = NdL;
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