// Copyright 2022 Team Havtorn. All Rights Reserved.

struct VertexInput {
    unsigned int myIndex : SV_VERTEXID;
};

struct VertexToPixel {
	float4 myPosition : SV_POSITION;
	float2 myUV : TEXCOORD;
};

struct PixelOutput {
	float4 myColor : SV_TARGET;
};

struct GBufferOutput {
    float3 myAlbedo         : SV_TARGET0;
    float3 myNormal         : SV_TARGET1;
    float3 myVertexNormal   : SV_TARGET2;
    float4 myMetalRoughAOEm : SV_TARGET3;
};

cbuffer FullscreenBuffer : register(b0)
{
    float2 myResolution;
    float2 myNoiseScale;
    float4 mySampleKernel[16];
}

cbuffer PostProcessingBuffer : register(b2)
{
    float4 myWhitePointColor;
    float myWhitePointIntensity;
    float myExposure;
    float mySSAORadius;
    float mySSAOSampleBias;
    float mySSAOMagnitude;
    float mySSAOContrast;
		
    int myIsReinhard;
    int myIsUncharted;
    int myIsACES;

    float myEmissiveStrength;
    
    float myVignetteStrength;    
    float myPadding;
    
    float4 myVignetteColor;
}

Texture2D fullscreenTexture1 : register(t0);
Texture2D fullscreenTexture2 : register(t1);
Texture2D fullscreenTexture3 : register(t2);
Texture2D fullscreenTexture4 : register(t3);
Texture2D fullscreenTexture5 : register(t4);
SamplerState defaultSampler : register(s0); // Clamp Sampler
SamplerState wrapSampler : register(s1);

static const float2 resolution = float2(1600.0f, 900.0f) / 8.0f;
static const float2 brokenScreenData = float2(-0.82f, -0.29f);
static const float gaussianKernel5[5] = { 0.06136f, 0.24477f, 0.38774f, 0.24477f, 0.06136f };

#define BSIGMA 0.1

float normpdf(in float x, in float sigma)
{
    return 0.39894 * exp(-0.5 * x * x / (sigma * sigma)) / sigma;
}

float normpdf3(in float3 v, in float sigma)
{
    return 0.39894 * exp(-0.5 * dot(v, v) / (sigma * sigma)) / sigma;
}