// Copyright 2022 Team Havtorn. All Rights Reserved.

#include "Includes/FullscreenshaderStructs.hlsli"

Texture2D depthTexture : register(t21);
Texture2D noiseTexture : register(t23);

cbuffer FrameBuffer : register(b1)
{
    float4x4 toCameraSpace;
    float4x4 toWorldFromCamera;
    float4x4 toProjectionSpace;
    float4x4 toCameraFromProjection;
    float4 cameraPosition;
}

//static float near = 0.1f;
//static float far = 500.0f;
static int numberOfSamples = 16;

float LinearizeDepth(float depth)
{
    //float z = depth * 2.0f - 1.0f;
    //return (2.0f * near * far) / (far + near - z * (far - near));
    
    // We are only interested in the depth here
    const float4 ndcCoords = float4(0, 0, depth, 1.0f);

    // Unproject the vector into (homogenous) view-space vector
    float4 viewCoords = mul(toCameraFromProjection, ndcCoords);
    //float4 viewCoords = mul(ndcCoords, toCameraFromProjection);

    // Divide by w, which results in actual view-space z value
    const float linearDepth = viewCoords.z / viewCoords.w;
    //float linearDepth = -viewCoords.z / viewCoords.w;
    
    return linearDepth;
}

PixelOutput main(VertexToPixel input)
{
    PixelOutput output;
    
    float2 uv = input.myUV;
    float z = depthTexture.Sample(defaultSampler, uv).r;
    
    if (z > 0.999999f)
    {
        output.myColor = 1.0f;
        return output;
    }
    
    float x = uv.x * 2.0f - 1;
    float y = (1 - uv.y) * 2.0f - 1;
    const float4 projectedPos = float4(x, y, z, 1.0f);
    float4 viewSpacePos = mul(toCameraFromProjection, projectedPos);
    viewSpacePos /= viewSpacePos.w;
    
    float3 origin = viewSpacePos.xyz;
    
    float3 normal = fullscreenTexture3.Sample(defaultSampler, uv).rgb;
    
    normal = mul((float3x3)toCameraSpace, normal);
    normalize(normal);
    
    float3 randomVector = noiseTexture.Sample(wrapSampler, uv * myNoiseScale).xyz * 2.0f - 1.0f;
    randomVector.z = 0.0f;
    randomVector = normalize(randomVector);
    const float3 tangent = normalize(float3(1.0f, 0.0f, 0.0f) - normal * dot(float3(1.0f, 0.0f, 0.0f), normal));
    const float3 bitangent = cross(normal, tangent);
    const float3x3 tbn = float3x3(randomVector.x * tangent + randomVector.y * bitangent, randomVector.x * bitangent - randomVector.y * tangent, normal);
    
    //float aoRadius = 0.6f;
    //float bias = 0.005f;
    //float magnitude = 1.1;
    //float contrast = 1.5;
    const float constantBias = 0.0f;
    
    float occlusion = numberOfSamples;
    for (unsigned int i = 0; i < (unsigned int)numberOfSamples; ++i)
    {
        float3 samplePosition = mul(mySampleKernel[i].xyz, tbn);
        samplePosition = origin + samplePosition * mySSAORadius;
        
        float4 offsetUV = float4(samplePosition.xyz, 1.0);
        offsetUV = mul(toProjectionSpace, offsetUV);
        offsetUV.xyz /= offsetUV.w;
        offsetUV.xy = offsetUV.xy * 0.5 + 0.5;
        offsetUV.y = (1 - offsetUV.y);
       
        // ...
    	float depthZ = depthTexture.SampleLevel(defaultSampler, offsetUV.xy, 0).r;
        float depthX = uv.x * 2.0f - 1;
        float depthY = (1 - uv.y) * 2.0f - 1;
        const float4 fromDepthProjectedPos = float4(depthX, depthY, depthZ, 1.0f);
        float4 fromDepthViewSpacePos = mul(toCameraFromProjection, fromDepthProjectedPos);
        fromDepthViewSpacePos /= fromDepthViewSpacePos.w;
        // ...
        
        float4 offsetPosition = fromDepthViewSpacePos;
        
        float occluded;
        if (samplePosition.z + mySSAOSampleBias <= offsetPosition.z)
        {
            occluded = 0.0f;
        }
        else
        {
            occluded = 1.0f;
        }

        const float intensity = smoothstep(0.0f, 1.0f, mySSAORadius / abs(origin.z - offsetPosition.z));
        occluded *= intensity;
        
        occlusion -= occluded;
    }
    
    occlusion /= numberOfSamples;
    occlusion = pow(abs(occlusion), mySSAOMagnitude);
    //occlusion = pow(occlusion, mySSAOMagnitude);
    occlusion = mySSAOContrast * (occlusion - 0.5f) + 0.5f;
    
    output.myColor.rgb = saturate(occlusion + constantBias);
    output.myColor.a = 1.0f;
    return output;
}