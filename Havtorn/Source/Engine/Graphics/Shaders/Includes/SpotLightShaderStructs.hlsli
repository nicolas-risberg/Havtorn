// Copyright 2022 Team Havtorn. All Rights Reserved.

struct SpotLightVertexInput
{
    float4 myPosition : POSITION;
};

struct SpotLightVertexToPixel
{
    float4 myPosition : SV_POSITION;
    float4 myWorldPosition : WORLD_POSITION;
    float3 myUV : UV;
};

struct SpotLightVertexToGeometry
{
    float4 myPosition : POSITION;
};

struct SpotLightGeometryToPixel
{
    float4 myPosition : SV_POSITION;
    float4 myWorldPosition : WORLDPOSITION;
    float3 myUV : UV;
};

struct SpotLightPixelOutput
{
    float4 myColor : SV_TARGET;
};

cbuffer SpotLightFrameBuffer : register(b0)
{
    float4x4 spotLightToCamera;
    float4x4 spotLightToWorldFromCamera;
    float4x4 spotLightToProjectionFromCamera;
    float4x4 spotLightToCameraFromProjection;
    float4 spotLightCameraPosition;
}

cbuffer SpotLightObjectBuffer : register(b1)
{
    float4x4 spotLightTransform;
    unsigned int spotLightMyNumberOfDetailNormals;
    unsigned int spotLightMyNumberOfTextureSets;
}

cbuffer SpotLightValueBuffer : register(b3)
{
    float4x4 spotLightToWorld;
    float4x4 spotLightToView;
    float4x4 spotLightToProjection;
    float4 spotLightColorAndIntensity;
    float4 spotLightPositionAndRange;
    float4 spotLightDirectionAndAngleExponent;
    float4 spotLightDirectionNormal1;
    float4 spotLightDirectionNormal2;
    float4 myUpLeftCorner;
    float4 myUpRightCorner;
    float4 myDownLeftCorner;
    float4 myDownRightCorner;
    float2 myInnerOuterAngle;
    float2 padding;
}