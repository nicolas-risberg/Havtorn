// Copyright 2022 Team Havtorn. All Rights Reserved.

#define PI_RCP 0.31830988618379067153776752674503

#define INTERLEAVED_GRID_SIZE 8
#define INTERLEAVED_GRID_SIZE_SQR 64
#define INTERLEAVED_GRID_SIZE_SQR_RCP 0.015625
#define USE_RANDOM_RAY_SAMPLES

cbuffer FrameBuffer : register(b0)
{
    float4x4 toCameraSpace;
    float4x4 toWorldFromCamera;
    float4x4 toProjectionSpace;
    float4x4 toCameraFromProjection;
    float4 cameraPosition;
}

cbuffer VolumetricLightBuffer : register(b4)
{
    float numberOfSamplesReciprocal;
    float lightPower;
    float scatteringProbability;
    float henyeyGreensteinGValue;
}

sampler shadowSampler : register(s1);
Texture2D depthTexture : register(t21);
Texture2D shadowDepthTexture : register(t22);

static const float lightVolumetricRandomRaySamples[64] = { 34.0f, 6, 22, 62, 25, 15, 10, 56, 29, 18, 46, 35, 5, 9, 37, 24, 60, 13, 19, 28, 17, 41, 11, 31, 58, 51, 40, 59, 7, 27, 50, 21, 4, 48, 53, 45, 43, 39, 20, 42, 44, 49, 38, 54, 23, 2, 33, 47, 1, 26, 61, 3, 12, 57, 36, 30, 55, 16, 0, 52, 14, 32, 8, 63 };

float PhaseFunctionHenyeyGreenstein(float projection, float g)
{
    return ((0.25f * PI_RCP) * (1 - g * g)) / pow((1 + g * g - 2.0f * g * projection), 1.5f);
}

float PhaseFunctionMieHazy(float projection)
{
    return ((0.25f * PI_RCP)) * (0.5f + 4.5f * pow(0.5f * (1 + projection), 8.0f));
}

float PhaseFunctionMieMurky(float projection)
{
    return ((0.25f * PI_RCP)) * (0.5f + 16.5f * pow(0.5f * (1 + projection), 32.0f));
}