// Copyright 2022 Team Havtorn. All Rights Reserved.

#include "Includes/DeferredShaderStructs.hlsli"

VertexPaintModelToPixel main(VertexPaintedModelInput input)
{
    VertexPaintModelToPixel returnValue;

    const float4 vertexObjectPos = input.myPosition.xyzw;
    const float4 vertexWorldPos = mul(toWorld, vertexObjectPos);
    const float4 vertexViewPos = mul(toCameraSpace, vertexWorldPos);
    const float4 vertexProjectionPos = mul(toProjectionSpace, vertexViewPos);

    const float3x3 toWorldRotation = (float3x3) toWorld;
    float3 vertexWorldNormal = mul(toWorldRotation, input.myNormal.xyz);
    float3 vertexWorldTangent = mul(toWorldRotation, input.myTangent.xyz);
    float3 vertexWorldBinormal = mul(toWorldRotation, input.myBinormal.xyz);

    returnValue.myPosition = vertexProjectionPos;
    returnValue.myWorldPosition = vertexWorldPos;
    returnValue.myNormal = float4(vertexWorldNormal, 0);
    returnValue.myTangent = float4(vertexWorldTangent, 0);
    returnValue.myBinormal = float4(vertexWorldBinormal, 0);
    returnValue.myUV = input.myUV;
    returnValue.myColor = input.myColor;
    return returnValue;
}