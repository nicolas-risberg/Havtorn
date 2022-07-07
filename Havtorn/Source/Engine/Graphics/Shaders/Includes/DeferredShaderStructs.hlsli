// Copyright 2022 Team Havtorn. All Rights Reserved.

struct VertexInput
{
    unsigned int Index : SV_VertexID;
};

struct StaticMeshVertexInput
{
    float3 Position     : POSITION;
    float3 Normal       : NORMAL;
    float3 Tangent      : TANGENT;
    float3 Bitangent    : BINORMAL;
    float2 UV           : UV;
};

struct VertexModelInput
{
    float4 Position   : POSITION;
    float4 Normal     : NORMAL;
    float4 Tangent    : TANGENT;
    float4 Bitangent  : BITANGENT;
    float2 UV         : UV;
    float4 BoneID     : BONEID;
    float4 BoneWeight : BONEWEIGHT;
    column_major float4x4 Transform : INSTANCETRANSFORM;
};

struct VertexPaintedModelInput
{
    float4 Position   : POSITION;
    float4 Normal     : NORMAL;
    float4 Tangent    : TANGENT;
    float4 Binormal   : BINORMAL;
    float2 UV         : UV;
    float3 Color      : COLOR;
};

struct VertexToPixel
{
    float4 Position   : SV_POSITION;
    float2 UV         : UV;
};

struct VertexModelToPixel
{
    float4 Position       : SV_POSITION;
    float4 WorldPosition  : WORLD_POSITION;
    float4 Normal         : NORMAL;
    float4 Tangent        : TANGENT;
    float4 Bitangent      : BITANGENT;
    float2 UV             : UV;
};

struct VertexPaintModelToPixel
{
    float4 Position         : SV_POSITION;
    float4 WorldPosition    : WORLDPOSITION;
    float4 Normal           : NORMAL;
    float4 Tangent          : TANGENT;
    float4 Binormal         : BINORMAL;
    float2 UV               : UV;
    float3 Color            : COLOR;
};

struct PixelOutput
{
    float4 Color : SV_TARGET;
};

cbuffer FrameBuffer : register(b0)
{
    float4x4 ToCameraSpace;
    float4x4 ToWorldFromCamera;
    float4x4 ToProjectionSpace;
    float4x4 ToCameraFromProjection;
    float4 CameraPosition;
}

cbuffer ObjectBuffer : register(b1)
{
    float4x4 toWorld;
    unsigned int NumberOfDetailNormals;
    unsigned int NumberOfTextureSets;
}

cbuffer DirectionalLightBuffer : register(b2)
{
    float4 ToDirectionalLight;
    float4 DirectionalLightColor;
}

cbuffer PointLightBuffer : register(b3)
{
    float4 ColorAndIntensity;
    float4 PositionAndRange;
}

cbuffer SpotLightBuffer : register(b4)
{
    //float4x4 SpotLightToWorld;
    //float4x4 SpotLightToView;
    //float4x4 SpotLightToProjection;
    float4 SpotLightColorAndIntensity;
    float4 SpotLightPositionAndRange;
    float4 SpotLightDirection;
    float4 SpotLightDirectionNormal1;
    float4 SpotLightDirectionNormal2;
    //float4 UpLeftCorner;
    //float4 UpRightCorner;
    //float4 DownLeftCorner;
    //float4 DownRightCorner;
    //float2 InnerOuterAngle;
    float OuterAngle;
    float InnerAngle;
    float2 SpotLightPadding;
}

cbuffer ShadowmapBuffer : register(b5)
{
    struct SShadowmapViewData
    {
        float4x4 ToShadowMapView;
        float4x4 ToShadowMapProjection;
        float4 ShadowmapPosition;
        float2 ShadowmapResolution;
        float2 ShadowAtlasResolution;
        float2 ShadowmapStartingUV;
        float ShadowTestTolerance;
        float Padding;
    } ShadowmapViewData[6];
}

cbuffer BoneBuffer : register(b6)
{
    matrix Bones[64];
};

cbuffer EmissiveBuffer : register(b7)
{
    float EmissiveStrength;
    float3 Padding;
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