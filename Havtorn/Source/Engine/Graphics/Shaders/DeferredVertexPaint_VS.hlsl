// Copyright 2022 Team Havtorn. All Rights Reserved.

#include "Includes/DeferredShaderStructs.hlsli"

VertexPaintModelToPixel main(VertexPaintedModelInput input)
{
    VertexPaintModelToPixel returnValue;

    const float4 vertexObjectPos = input.Position.xyzw;
    const float4 vertexWorldPos = mul(toWorld, vertexObjectPos);
    const float4 vertexViewPos = mul(toCameraSpace, vertexWorldPos);
    const float4 vertexProjectionPos = mul(toProjectionSpace, vertexViewPos);

    const float3x3 toWorldRotation = (float3x3) toWorld;
    float3 vertexWorldNormal = mul(toWorldRotation, input.Normal.xyz);
    float3 vertexWorldTangent = mul(toWorldRotation, input.Tangent.xyz);
    float3 vertexWorldBinormal = mul(toWorldRotation, input.Binormal.xyz);

    returnValue.Position = vertexProjectionPos;
    returnValue.WorldPosition = vertexWorldPos;
    returnValue.Normal = float4(vertexWorldNormal, 0);
    returnValue.Tangent = float4(vertexWorldTangent, 0);
    returnValue.Binormal = float4(vertexWorldBinormal, 0);
    returnValue.UV = input.UV;
    returnValue.Color = input.Color;
    return returnValue;
}