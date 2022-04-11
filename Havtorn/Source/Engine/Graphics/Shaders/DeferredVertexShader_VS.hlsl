// Copyright 2022 Team Havtorn. All Rights Reserved.

#include "Includes/DeferredShaderStructs.hlsli"

VertexToPixel main(VertexInput input)
{
	const float4 positions[3] =
    {
        float4(-1.0f, -1.0f, 0.0f, 1.0f),
        float4(-1.0f, 3.0f, 0.0f, 1.0f),
        float4(3.0f, -1.0f, 0.0f, 1.0f),
    };

	const float2 uvs[3] =
    {
        float2(0.0f, 1.0f),     
        float2(0.0f, -1.0f), 
        float2(2.0f, 1.0f) 
    };
    
    VertexToPixel returnValue;
    returnValue.Position = positions[input.Index];
    returnValue.UV = uvs[input.Index];
    
    return returnValue;
}