// Copyright 2022 Team Havtorn. All Rights Reserved.

//#include "EngineDefines.h"

TextureCube environmentTexture : register(t0);
Texture2D colorTexture : register(t1); //sRGB: RGB-A = Color
Texture2D materialTexture : register(t2); //Linear: R = Metalness, G = Roughness, B = Emissive, A = Strength
Texture2D normalTexture : register(t3); //Linear: R = null, G = Normal.y, B = AmbientOcclusion, A = Normal.x
Texture2D detailNormals[4] : register(t4);

Texture2D shadowDepthTexture : register(t22);

SamplerState defaultSampler : register(s0);
sampler shadowSampler : register(s1);

cbuffer FrameBuffer : register(b0)
{
    float4x4 toCamera;	//64
    float4x4 toProjection; //64
    float4 cameraPosition; //16
    float4 toDirectionalLight; //16
    float4 directionalLightColor; //16
    
    float4x4 toDirectionalLightView;
    float4x4 toDirectionalLightProjection;
    float4 directionalLightPosition;
    float2 directionalLightShadowMapResolution;
    float2 myDirectionalLightPadding;
};

cbuffer ObjectBuffer : register(b1)
{
    float4x4 toWorld;
    struct PointLight
    {
        float4 myPositionAndIntensity;
        float4 myColorAndRange;
    } myPointLights[8]; //Light count
    
    unsigned int myNumberOfUsedPointLights;
    unsigned int myNumberOfDetailNormals;
    unsigned int myNumberOfTextureSets;
    unsigned int myPaddington;
};

cbuffer BoneBuffer : register(b2)
{
    matrix myBones[64];
};

cbuffer OutlineBuffer : register(b3)
{
    float4 myOutlineColor;
};

struct VertexInput
{
	float4 myPosition : POSITION;       //16
    float4 myNormal : NORMAL;           //16
    float4 myTangent : TANGENT;         //16
    float4 myBiNormal : BITANGENT;      //16
	float2 myUV : UV;                   //8
	float4 myBoneID : BONEID;           //16
    float4 myBoneWeight : BONEWEIGHT; //16
    column_major float4x4 myTransform : INSTANCETRANSFORM;
};

struct AnimVertexToPixel
{
    float4 myPosition : SV_POSITION;
    float4 myColor : COLOR;
};

struct VertexToPixel
{
	float4 myPosition : SV_POSITION;
	float4 myWorldPosition : WORLD_POSITION;
	float4 myNormal : NORMAL;
	float4 myTangent : TANGENT;
	float4 myBiNormal : BITANGENT;
	float2 myUV : UV;
};

struct PixelOutPut
{
	float4 myColor : SV_TARGET;
};

