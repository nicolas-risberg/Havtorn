// Copyright 2022 Team Havtorn. All Rights Reserved.

#include "SamplingFunctions.hlsli"

float RoughnessFromPerceptualRoughness(float fPerceptualRoughness)
{
    return fPerceptualRoughness * fPerceptualRoughness;
}

float PerceptualRoughnessFromRoughness(float fRoughness)
{
    return sqrt(max(0.0, fRoughness));
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

int GetNumMips(TextureCube cubeTex)
{
    int iWidth = 0;
    int iHeight = 0;
    int numMips = 0;
    cubeTex.GetDimensions(0, iWidth, iHeight, numMips);
    return numMips;
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
    float rimmask = clamp(1 + fFadeParam * dot(vR, vertNormalNormalized), 0.0, 1.0);
    rimmask *= rimmask;
    return rimmask;
}

// FROM TGA MODELVIEWER used for rotating cubema, code snippet below EvaluateAmbience
float2 polarToCartesian(float polar, float2 vec)
{
    float2 vecOut;
    vecOut.x = length(vec) * sin(polar);
    vecOut.y = length(vec) * cos(polar);
    return vecOut;
}

float cartesianToPolar(float2 vec)
{
    return atan2(vec.x, vec.y);
}
// ! FROM TGA MODELVIEWER

float3 EvaluateAmbience(TextureCube lysBurleyCube, float3 vN, float3 org_normal, float3 to_cam, float perceptualRoughness, float ao, float3 dfcol, float3 spccol)
{      
    vN = normalize(vN);
    org_normal = normalize(org_normal);

    const int numMips = GetNumMips(lysBurleyCube);
    const int nrBrdfMips = numMips - nMipOffset;
    const float VdotN = clamp(dot(to_cam, vN), 0.0, 1.0f);
    const float3 vRorg = 2 * vN * VdotN - to_cam;

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
    fFade *= ApproximateSpecularSelfOcclusion(vR, org_normal);

    const float3 ambientDiffuse = ao * dfcol * diffRad;
    const float3 ambientSpecular = fFade * spccol * specRad;
    return ambientDiffuse + ambientSpecular;
}

/*
// With Code Tga Modelviewer to enable cubemap rotation.
float3 EvaluateAmbience(
    //float3 nvN = vN;
    //normalize(vN);
    //float3 norg_normal = org_normal;
    //normalize(org_normal);

    //int numMips = GetNumMips(lysBurleyCube);
    //const int nrBrdfMips = numMips - nMipOffset;
    //float VdotN = clamp(dot(to_cam, nvN), 0.0, 1.0f);
    //const float3 vRorg = 2 * nvN * VdotN - to_cam;

    //float3 vR = GetSpecularDominantDir(nvN, vRorg, RoughnessFromPerceptualRoughness(perceptualRoughness));
    //float RdotNsat = saturate(dot(nvN, vR));

    //float l = BurleyToMip(perceptualRoughness, numMips, RdotNsat);

    //// TGA modelviewer
    //float myCubeMapYRotation = 0.0f;
    //float2 rotatedVN = polarToCartesian(cartesianToPolar(vN.xz) + myCubeMapYRotation, vN.xz);
    //float3 reflectVector = reflect(-float3(to_cam.x, to_cam.y, to_cam.z), vN);
    //float2 rotatedReflectVector = polarToCartesian(cartesianToPolar(reflectVector.xz) + myCubeMapYRotation, reflectVector.xz);
    //float3 specRad = lysBurleyCube.SampleLevel(defaultSampler, float3(rotatedReflectVector.x, reflectVector.y, rotatedReflectVector.y), l).xyz;
    //float3 diffRad = lysBurleyCube.SampleLevel(defaultSampler, float3(rotatedVN.x, vN.y, rotatedVN.y), (float) (nrBrdfMips - 1)).xyz;
    //// ! TGA modelviewer
    
    //float3 specRad = lysBurleyCube.SampleLevel(defaultSampler, vR, l).xyz;
    //float3 diffRad = lysBurleyCube.SampleLevel(defaultSampler, nvN, (float) (nrBrdfMips - 1)).xyz;

    //float fT = 1.0 - RdotNsat;
    //float fT5 = fT * fT;
    //fT5 = fT5 * fT5 * fT;
    //spccol = lerp(spccol, (float3) 1.0, fT5);

    //float fFade = GetReductionInMicrofacets(perceptualRoughness);
    //fFade *= EmpiricalSpecularAO(ao, perceptualRoughness);
    //fFade *= ApproximateSpecularSelfOcclusion(vR, norg_normal);

    //float3 ambientDiffuse = ao * dfcol * diffRad;
    //float3 ambientSpecular = fFade * spccol * specRad;
    //return ambientDiffuse + ambientSpecular;
*/