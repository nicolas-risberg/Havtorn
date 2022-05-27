// Copyright 2022 Team Havtorn. All Rights Reserved.

float InverseLerp(float a, float b, float c)
{
    return (c - a) / (b - a);
}

float SampleShadowPos(float3 projectionPos, float2 startingUV, float2 mapResolution, float2 atlasResolution, Texture2D shadowDepthTexture, sampler shadowSampler)
{
    float2 uvCoords = projectionPos.xy;
    uvCoords *= float2(0.5f, -0.5f);
    uvCoords += float2(0.5f, 0.5f);

    // Remap UVs to Map in Atlas
    uvCoords *= (mapResolution / atlasResolution);
    uvCoords += startingUV;

    const float nonLinearDepth = shadowDepthTexture.SampleLevel(shadowSampler, uvCoords, 0).r;
    float oob = 1.0f;
    if (projectionPos.x > 1.0f || projectionPos.x < -1.0f || projectionPos.y > 1.0f || projectionPos.y < -1.0f)
    {
        oob = 0.0f;
    }

    const float a = nonLinearDepth * oob;
    float b = projectionPos.z;
    b = InverseLerp(-0.5f, 0.5f, b) * oob;

    b *= oob;

    if (b - a < 0.001f)
    {
        return 0.0f;
    }
    else
    {
        return 1.0f;
    }
}

float3 ShadowFactor(float3 worldPosition, float3 lightPosition, float4x4 lightViewMatrix, float4x4 lightProjectionMatrix, Texture2D shadowDepthTexture, sampler shadowSampler, float2 shadowmapResolution, float2 shadowAtlasResolution, float2 shadowmapStartingUV)
{
    worldPosition -= lightPosition.xyz;
    const float4 viewPos = mul((float4x3)lightViewMatrix, worldPosition);
    float4 projectionPos = mul(lightProjectionMatrix, viewPos);
    const float3 viewCoords = projectionPos.xyz;

    // 3x3 PCF
    float total = 0.0f;
    for (float x = -1.0; x < 1.5f; x += 1.0f)
    {
        for (float y = -1.0; y < 1.5f; y += 1.0f)
        {
            float3 off;
            off.x = x / shadowmapResolution.x;
            off.y = y / shadowmapResolution.y;
            off.z = 0.0f;
            total += SampleShadowPos(viewCoords + off, shadowmapStartingUV, shadowmapResolution, shadowAtlasResolution, shadowDepthTexture, shadowSampler);
        }
    }
    total /= 9.0f;
    return total;
}