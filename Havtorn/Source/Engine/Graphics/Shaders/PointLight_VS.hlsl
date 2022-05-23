// Copyright 2022 Team Havtorn. All Rights Reserved.

#include "Includes/PointLightShaderStructs.hlsli"

PointLightVertexToPixel main(PointLightVertexInput input)
{
    PointLightVertexToPixel output;
    output.myPosition = input.myPosition;
    output.myPosition.xyz *= pointLightPositionAndRange.w;
    output.myPosition = mul(pointLightToWorld, output.myPosition);
    output.myWorldPosition = output.myPosition;
    output.myPosition = mul(pointLightToCamera, output.myPosition);
    output.myPosition = mul(pointLightToProjection, output.myPosition);
    output.myUV = output.myPosition.xyw;
    output.myUV.y *= -1.0f;
    
    return output;
}