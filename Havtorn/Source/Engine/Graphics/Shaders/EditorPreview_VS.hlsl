// Copyright 2022 Team Havtorn. All Rights Reserved.

struct VertexInput
{
    float3 Position     : POSITION;
    float3 Normal       : NORMAL;
    float3 Tangent      : TANGENT;
    float3 Bitangent    : BINORMAL;
    float2 UV           : UV;
};

struct VertexToPixel
{
    float4 Position     : SV_POSITION;
    float4 Normal       : NORMAL;
    float4 Tangent      : TANGENT;
    float4 Bitangent    : BINORMAL;
    float2 UV           : UV;
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
}

VertexToPixel main(VertexInput input)
{
    VertexToPixel returnValue;

    const float4 vertexObjectPos = float4(input.Position, 1.0f);
    const float4 vertexWorldPos = mul(toWorld, vertexObjectPos);
    const float4 vertexViewPos = mul(toCameraSpace, vertexWorldPos);
    const float4 vertexProjectionPos = mul(toProjectionSpace, vertexViewPos);

    const float3x3 toWorldRotation = (float3x3) toWorld;
    float3 vertexWorldNormal = mul(toWorldRotation, input.Normal.xyz);
    float3 vertexWorldTangent = mul(toWorldRotation, input.Tangent.xyz);
    float3 vertexWorldBitangent = mul(toWorldRotation, input.Bitangent.xyz);

    returnValue.Position = vertexProjectionPos;
    returnValue.Normal = float4(vertexWorldNormal, 0);
    returnValue.Tangent = float4(vertexWorldTangent, 0);
    returnValue.Bitangent = float4(vertexWorldBitangent, 0);
    returnValue.UV = input.UV;
    return returnValue;
}