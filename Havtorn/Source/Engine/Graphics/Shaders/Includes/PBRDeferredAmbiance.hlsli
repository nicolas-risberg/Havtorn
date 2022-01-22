// Copyright 2022 Team Havtorn. All Rights Reserved.

#include "DeferredSamplingFunctions.hlsli"
#include "MathHelpers.hlsli"

float RoughnessFromPerceptualRoughness(float fPerceptualRoughness)
{
    return fPerceptualRoughness * fPerceptualRoughness;
}

float PerceptualRoughnessFromRoughness(float fRoughness)
{
    return sqrt(max(0.0, fRoughness));
}

#define FLT_EPSILON 1.192092896e-07f
#define nMipOffset 3

int GetNumMips(TextureCube cubeTex)
{
    int iWidth = 0;
    int iHeight = 0;
    int numMips = 0;
    cubeTex.GetDimensions(0, iWidth, iHeight, numMips);
    return numMips;
}

float SpecularPowerFromPerceptualRoughness(float fPerceptualRoughness)
{
	const float fRoughness = RoughnessFromPerceptualRoughness(fPerceptualRoughness);
    return (2.0 / max(FLT_EPSILON, fRoughness * fRoughness)) - 2.0;
}

float PerceptualRoughnessFromSpecularPower(float fSpecPower)
{
	const float fRoughness = sqrt(2.0 / (fSpecPower + 2.0));
    return PerceptualRoughnessFromRoughness(fRoughness);
}

float BurleyToMip(float fPerceptualRoughness, int nMips, float NdotR)
{
    float fSpecPower = SpecularPowerFromPerceptualRoughness(fPerceptualRoughness);
    fSpecPower /= (4 * max(NdotR, FLT_EPSILON));
    const float fScale = PerceptualRoughnessFromSpecularPower(fSpecPower);
    return fScale * (nMips - 1 - nMipOffset);
}

float3 GetSpecularDominantDir(float3 vN, float3 vR, float fRealRoughness)
{
	const float fInvRealRough = saturate(1 - fRealRoughness);
	const float lerpFactor = fInvRealRough * (sqrt(fInvRealRough) + fRealRoughness);
    
    return lerp(vN, vR, lerpFactor);
}

float GetReductionInMicrofacets(float perceptualRoughness)
{
	const float roughness = RoughnessFromPerceptualRoughness(perceptualRoughness);
    return 1.0 / (roughness * roughness + 1.0);
}

float EmpiricalSpecularAO(float ao, float perceptualRoughness)
{
	const float fSmooth = 1 - perceptualRoughness;
	const float fSpecAo = gain(ao, 0.5 + max(0.0, fSmooth * 0.4));
    
    return min(1.0, fSpecAo + lerp(0.0, 0.5, fSmooth * fSmooth * fSmooth * fSmooth));

}

float ApproximateSpecularSelfOcclusion(float3 vR, float3 vertNormalNormalized)
{
    const float fFadeParam = 1.3;
    float rimMask = clamp(1 + fFadeParam * dot(vR, vertNormalNormalized), 0.0, 1.0);
    rimMask *= rimMask;
    
    return rimMask;
}

float3 EvaluateAmbiance(TextureCube lysBurleyCube, float3 vN, float3 orgNormal, float3 toCam, float perceptualRoughness, float metalness, float3 albedo, float ao, float3 dfcol, float3 spccol)
{
	const int numMips = GetNumMips(lysBurleyCube);
    const int nrBrdfMips = numMips - nMipOffset;
	const float VdotN = clamp(dot(toCam, vN), 0.0, 1.0f);
    const float3 vRorg = 2 * vN * VdotN - toCam;

	const float3 vR = GetSpecularDominantDir(vN, vRorg, RoughnessFromPerceptualRoughness(perceptualRoughness));
	const float RdotNsat = saturate(dot(vN, vR));

	const float l = BurleyToMip(perceptualRoughness, numMips, RdotNsat);

	const float3 specRad = lysBurleyCube.SampleLevel(defaultSampler, vR, l).xyz;
	const float3 diffRad = lysBurleyCube.SampleLevel(defaultSampler, vN, (float) (nrBrdfMips - 1)).xyz;

	const float fT = 1.0 - RdotNsat;
    float fT5 = fT * fT;
    fT5 = fT5 * fT5 * fT;
    spccol = lerp(spccol, (float3) 1.0, fT5);
    
    float fFade = GetReductionInMicrofacets(perceptualRoughness);
    fFade *= EmpiricalSpecularAO(ao, perceptualRoughness);
    fFade *= ApproximateSpecularSelfOcclusion(vR, orgNormal);

	const float3 ambientDiffuse = ao * dfcol * diffRad;
	const float3 ambientSpecular = fFade * spccol * specRad;
    return ambientDiffuse + ambientSpecular;
}