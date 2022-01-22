// Copyright 2022 Team Havtorn. All Rights Reserved.

struct VertexInput
{
    unsigned int myIndex : SV_VertexID;
};

struct VertexModelInput
{
    float4 myPosition   : POSITION;
    float4 myNormal     : NORMAL;
    float4 myTangent    : TANGENT;
    float4 myBiTangent  : BITANGENT;
    float2 myUV         : UV;
    float4 myBoneID     : BONEID;
    float4 myBoneWeight : BONEWEIGHT;
    column_major float4x4 myTransform : INSTANCETRANSFORM;
};

struct VertexPaintedModelInput
{
    float4 myPosition   : POSITION;
    float4 myNormal     : NORMAL;
    float4 myTangent    : TANGENT;
    float4 myBinormal   : BINORMAL;
    float2 myUV         : UV;
    float3 myColor      : COLOR;
};

struct VertexToPixel
{
    float4 myPosition   : SV_POSITION;
    float2 myUV         : UV;
};

struct VertexModelToPixel
{
    float4 myPosition       : SV_POSITION;
    float4 myWorldPosition  : WORLDPOSITION;
    float4 myNormal         : NORMAL;
    float4 myTangent        : TANGENT;
    float4 myBinormal       : BINORMAL;
    float2 myUV             : UV;
};

struct VertexPaintModelToPixel
{
    float4 myPosition : SV_POSITION;
    float4 myWorldPosition : WORLDPOSITION;
    float4 myNormal : NORMAL;
    float4 myTangent : TANGENT;
    float4 myBinormal : BINORMAL;
    float2 myUV : UV;
    float3 myColor : COLOR;
};

struct PixelOutput
{
    float4 myColor : SV_TARGET;
};

cbuffer FrameBuffer : register(b0)
{
    float4x4 toCameraSpace;
    float4x4 toWorldFromCamera;
    float4x4 toProjectionSpace;
    float4x4 toCameraFromProjection;
    float4 cameraPosition;
}

cbuffer ObjectBuffer : register(b1)
{
    float4x4 toWorld;
    unsigned int myNumberOfDetailNormals;
    unsigned int myNumberOfTextureSets;
}

cbuffer LightBuffer : register(b2)
{
    float4x4 toDirectionalLightView;
    float4x4 toDirectionalLightProjection;
    float4 directionalLightPosition; // For shadow calculations
    float4 toDirectionalLight;
    float4 directionalLightColor;
    float2 directionalLightShadowMapResolution;
    float2 directionalLightPadding;
}

cbuffer PointLightBuffer : register(b3)
{
    float4 myColorAndIntensity;
    float4 myPositionAndRange;
}

cbuffer BoneBuffer : register(b4)
{
    matrix myBones[64];
};

cbuffer EmissiveBuffer : register(b5)
{
    float myEmissiveStrength;
    float3 myPadding;
}

// Cubemap used for environment light shading
TextureCube environmentTexture : register(t0);

// GBuffer Textures: textures stored in the GBuffer, contains data for models. Used for environment and point light calculations
Texture2D albedoTextureGBuffer        : register(t1);
Texture2D normalTextureGBuffer        : register(t2);
Texture2D vertexNormalTextureGBuffer  : register(t3);
Texture2D materialTextureGBuffer      : register(t4);

// Model textures (used to create GBuffer textures )
Texture2D albedoTexture     : register(t5);
Texture2D materialTexture   : register(t6);
Texture2D normalTexture     : register(t7);
// Detail normals
Texture2D detailNormals[4] : register(t8);
// Vertex Paint Materials
Texture2D vertexPaintTextures[9] : register(t12);

Texture2D depthTexture          : register(t21);
Texture2D shadowDepthTexture    : register(t22);
Texture2D SSAOTexture           : register(t23);

sampler defaultSampler : register(s0);
sampler shadowSampler  : register(s1);

#define RED_ALBEDO      0
#define RED_MATERIAL    1
#define RED_NORMAL      2
#define GREEN_ALBEDO    3
#define GREEN_MATERIAL  4
#define GREEN_NORMAL    5
#define BLUE_ALBEDO     6
#define BLUE_MATERIAL   7
#define BLUE_NORMAL     8