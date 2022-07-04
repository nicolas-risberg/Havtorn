#include "SpotLightShaderStructs.hlsli"

SpotLightVertexToPixel main(SpotLightVertexInput input)
{
    // For Geometry Shader
    //SpotLightVertexToGeometry returnValue;    
    //float4 vertexObjectPos = input.myPosition.xyzw;
    ////float4 vertexWorldPos = mul(spotLightToWorld, vertexObjectPos);
    ////float4 vertexViewPos = mul(spotLightToCamera, vertexWorldPos);
    //returnValue.myPosition = /*vertexViewPos*/input.myPosition.xyzw;
    //return returnValue;
    
    SpotLightVertexToPixel output;
    output.myPosition = input.myPosition;
    output.myPosition.xyz *= spotLightPositionAndRange.w;
    output.myPosition = mul(spotLightToWorld, output.myPosition);
    output.myWorldPosition = output.myPosition;
    output.myPosition = mul(spotLightToCamera, output.myPosition);
    output.myPosition = mul(spotLightToProjectionFromCamera, output.myPosition);
    output.myUV = output.myPosition.xyw;
    output.myUV.y *= -1.0f;
    
    return output;
}