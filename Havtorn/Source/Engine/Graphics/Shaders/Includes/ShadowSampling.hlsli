// Copyright 2022 Team Havtorn. All Rights Reserved.

float InverseLerp(float a, float b, float c)
{
    return (c - a) / (b - a);
}

float LinearizeDepth(float nearPlane, float farPlane, float depth)
{
    float z = depth * 2.0 - 1.0; // Back to NDC 
    return (2.0 * nearPlane * farPlane) / (farPlane + nearPlane - z * (farPlane - nearPlane));
}

int GetShadowmapViewIndex(float3 worldPosition, float3 lightPosition)
{
    float3 v = worldPosition - lightPosition;
    float3 vAbs = abs(v);
    int faceIndex;
    
    if (vAbs.z >= vAbs.x && vAbs.z >= vAbs.y)
    {
        faceIndex = v.z < 0.0 ? 2.0 : 0.0;
    }
    else if (vAbs.y >= vAbs.x)
    {
        faceIndex = v.y < 0.0 ? 5.0 : 4.0;
    }
    else
    {
        faceIndex = v.x < 0.0 ? 3.0 : 1.0;
    }
    return faceIndex;
}

float SampleShadowPos(float4 projectionPos, float2 startingUV, float2 mapResolution, float2 atlasResolution, Texture2D shadowDepthTexture, sampler shadowSampler, float shadowTestTolerance)
{
    // Perspective Divide
    projectionPos.xyz /= projectionPos.w;
    
    float2 uvCoords = projectionPos.xy;
    uvCoords *= float2(0.5f, -0.5f);
    uvCoords += float2(0.5f, 0.5f);

    // Remap UVs to Map in Atlas
    uvCoords *= (mapResolution / atlasResolution);
    uvCoords += startingUV;

    const float nonLinearDepth = shadowDepthTexture.SampleLevel(shadowSampler, uvCoords, 0).r;
    
    // Out of Bounds check
    float oob = 1.0f;
    if (projectionPos.x > 1.0f || projectionPos.x < -1.0f || projectionPos.y > 1.0f || projectionPos.y < -1.0f)
    {
        oob = 0.0f;
    }

    const float a = nonLinearDepth * oob;
    const float b = projectionPos.z * oob;
    
    // This breaks everything, where is it from?
    //b = InverseLerp(-0.5f, 0.5f, b) * oob;

    if (b - a < shadowTestTolerance)
    {
        return 0.0f;
    }
    else
    {
        return 1.0f;
    }
}

float3 ShadowFactor(float3 worldPosition, float3 lightPosition, float4x4 lightViewMatrix, float4x4 lightProjectionMatrix, Texture2D shadowDepthTexture, sampler shadowSampler, float2 shadowmapResolution, float2 shadowAtlasResolution, float2 shadowmapStartingUV, float shadowTestTolerance)
{
    worldPosition -= lightPosition.xyz;
    float4 viewPos = mul((float4x3)lightViewMatrix, worldPosition);
    viewPos.w = 1.0f;
    float4 projectionPos = mul(lightProjectionMatrix, viewPos);
    const float4 viewCoords = projectionPos;
    
    // 3x3 PCF
    float total = 0.0f;
    for (float x = -1.0; x <= 1.0f; x++)
    {
        for (float y = -1.0; y <= 1.0f; y++)
        {
            float4 off;
            off.x = x / shadowmapResolution.x;
            off.y = y / shadowmapResolution.y;
            off.z = 0.0f;
            off.w = 0.0f;
            total += SampleShadowPos(viewCoords + off, shadowmapStartingUV, shadowmapResolution, shadowAtlasResolution, shadowDepthTexture, shadowSampler, shadowTestTolerance);
        }
    }
    total /= 9.0f;
    return total;
}