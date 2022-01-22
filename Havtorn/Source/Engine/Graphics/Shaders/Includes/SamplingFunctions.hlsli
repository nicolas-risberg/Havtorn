// Copyright 2022 Team Havtorn. All Rights Reserved.

#include "ShaderStructs.hlsli"
#include "MathHelpers.hlsli"
#include "DetailNormalHelpers.hlsli"
//#include "ShadowSampling.hlsli"

static float emissiveStrength = 20.0f;

PixelOutPut PixelShader_Color(VertexToPixel input)
{
    PixelOutPut output;
    float4 color = colorTexture.Sample(defaultSampler, input.myUV.xy).rgba;
	color.rgb = GammaToLinear(color.rgb);
	output.myColor.rgb = color.rgb;
	output.myColor.a = color.a;
    return output;
}

float PixelShader_DetailNormalStrength(VertexToPixel input)
{
	const float output = materialTexture.Sample(defaultSampler, input.myUV.xy).a;
    return output;
}

PixelOutPut PixelShader_DetailNormal(VertexToPixel input, int index)
{
	const float tilingModifier = DETAILNORMAL_TILING; // eq to scale
   
    float3 normal;
    normal.xy = detailNormals[index].Sample(defaultSampler, input.myUV.xy * tilingModifier).ag;
    normal.z = 0.0f;
    normal = (normal * 2.0f) - 1.0f;
    normal.z = sqrt(1 - saturate((normal.x * normal.x) + (normal.y * normal.y)));
    normal = normalize(normal);
    
    PixelOutPut output;
    output.myColor.xyz = normal.xyz;
    output.myColor.a = 1.0f;
    return output;
}

PixelOutPut PixelShader_Normal(VertexToPixel input)
{    
    float3 normal; 
    normal.xy = normalTexture.Sample(defaultSampler, input.myUV.xy).ag;
    normal.z = 0.0f;
    normal = (normal * 2.0f) - 1.0f;
    normal.z = sqrt(1 - saturate((normal.x * normal.x) + (normal.y * normal.y)));
    //normal = (normal * 0.5f) + 0.5f;// Found in TGA modelviewer shader code, but seems to cause issues here.
    normal = normalize(normal);
    
    PixelOutPut output;
    
    output.myColor.xyz = normal.xyz;
    output.myColor.a = 1.0f;
    return output;
}

// Unused?
PixelOutPut PixelShader_TextureNormal(VertexToPixel input)
{
    float3 normal = normalTexture.Sample(defaultSampler, input.myUV.xy).agr;
    
    PixelOutPut output;
    output.myColor.xyz = normal.xyz;
    output.myColor.a = 1.0f;
    return output;
}

PixelOutPut PixelShader_AmbientOcclusion(VertexToPixel input)
{
    PixelOutPut output;
    float ao = normalTexture.Sample(defaultSampler, input.myUV.xy).b;
    output.myColor.rgb = ao.xxx;
    output.myColor.a = 1.0f;
    return output;
}

PixelOutPut PixelShader_Material(VertexToPixel input)
{
    PixelOutPut output;
    float3 material = materialTexture.Sample(defaultSampler, input.myUV.xy).rgb;
    output.myColor.rgb = material.rgb;
    output.myColor.a = 1.0f;
    return output;
}

PixelOutPut PixelShader_Metalness(VertexToPixel input)
{
    PixelOutPut output;
    PixelOutPut material = PixelShader_Material(input);
    output.myColor.rgb = material.myColor.rrr;
    output.myColor.a = 1.0f;
    return output;
}

PixelOutPut PixelShader_PerceptualRoughness(VertexToPixel input)
{
    PixelOutPut output;
    PixelOutPut material = PixelShader_Material(input);
    output.myColor.rgb = material.myColor.ggg;
    output.myColor.a = 1.0f;
    return output;
}

PixelOutPut PixelShader_Emissive(VertexToPixel input)
{
    PixelOutPut output;
    PixelOutPut material = PixelShader_Material(input);
    output.myColor.rgb = material.myColor.bbb * emissiveStrength;
    output.myColor.a = 1.0f;
    return output;
}

//float SampleShadowPos(float3 projectionPos)
//{
//    float2 uvCoords = projectionPos.xy;
//    uvCoords *= float2(0.5f, -0.5f);
//    uvCoords += float2(0.5f, 0.5f);

//    float nonLinearDepth = shadowDepthTexture.Sample(shadowSampler, uvCoords).r;
//    float oob = 1.0f;
//    if (projectionPos.x > 1.0f || projectionPos.x < -1.0f || projectionPos.y > 1.0f || projectionPos.y < -1.0f)
//    {
//        oob = 0.0f;
//    }

//    float a = nonLinearDepth * oob;
//    float b = projectionPos.z;
//    b = invLerp(-0.5f, 0.5f, b) * oob;

//    b *= oob;

//    if (b - a < 0.001f)
//    {
//        return 0.0f;
//    }
//    else
//    {
//        return 1.0f;
//    }
//}

//float3 ShadowFactor(float3 worldPosition, float3 lightPosition, float4x4 lightViewMatrix, float4x4 lightProjectionMatrix)
//{
//    worldPosition -= lightPosition.xyz;
//    float4 viewPos = mul(lightViewMatrix, worldPosition);
//    float4 projectionPos = mul(lightProjectionMatrix, viewPos);
//    float3 viewCoords = projectionPos.xyz;

//    float total = 0.0f;
//    for (float x = -1.0; x < 1.5f; x += 1.0f)
//    {
//        for (float y = -1.0; y < 1.5f; y += 1.0f)
//        {
//            //2048.0f * 4.0f,
//            float3 off;
//            off.x = x / (2048.0f * 4.0f);
//            off.y = y / (2048.0f * 4.0f);
//            off.z = 0.0f;
//            total += SampleShadowPos(viewCoords + off);
//        }
//    }
//    total /= 9.0f;
//    return total;
//}