// Copyright 2022 Team Havtorn. All Rights Reserved.

#include "DeferredShaderStructs.hlsli"
//#include "MathHelpers.hlsli"
#include "DetailNormalHelpers.hlsli"
//#include "ShadowSampling.hlsli"

//static float emissiveStrength = 20.0f;

float4 PixelShader_WorldPosition(float2 uv)
{   
    // World Pos texture // Deprecated
    //float4 worldPos = worldPositionTexture.Sample(defaultSampler, input.myUV.xy).rgba;
    
    // Depth sampling
    float z = depthTexture.Sample(defaultSampler, uv).r;
    float x = uv.x * 2.0f - 1;
    float y = (1 - uv.y) * 2.0f - 1;
    const float4 projectedPos = float4(x, y, z, 1.0f);
    float4 viewSpacePos = mul(toCameraFromProjection, projectedPos);
    viewSpacePos /= viewSpacePos.w;
    float4 worldPos = mul(toWorldFromCamera, viewSpacePos);

    worldPos.a = 1.0f;
    return worldPos;
}

float4 PixelShader_Exists(float2 uv)
{
    //PixelOutput output;
    //float4 worldPos = PixelShader_WorldPosition(input).myColor.rgba;
    //output.myColor.rgba = worldPos;
    //return output;
    //PixelOutput output;
    float depth = depthTexture.Sample(defaultSampler, uv).r;

    return float4(depth, 0.0f, 0.0f, 0.0f);
}

float4 PixelShader_Albedo(float2 uv)
{
    float4 color = albedoTexture.Sample(defaultSampler, uv).rgba;
    //color.rgb = GammaToLinear(color.rgb);// Used for when testing Detail normals, tufted leather model had incorrect compression format (sLinear instead of sRGB) or something lika that i don't remember
    return color;
}

float4 PixelShader_Albedo(Texture2D anAlbedoTexture, float2 uv)
{
    float4 color = anAlbedoTexture.Sample(defaultSampler, uv).rgba;
    return color;
}

// This function is used for renderpasses to isolate the normal texture.
float4 PixelShader_NormalForIsolatedRendering(float2 uv)
{
    float3 normal;
    normal.xy = normalTexture.Sample(defaultSampler, uv).ag;
    // Recreate z
    normal.z = 0.0f;
    normal.z = sqrt(1 - saturate((normal.x * normal.x) + (normal.y * normal.y)));
    normal = normalize(normal);
    
    return float4(normal.x, normal.y, normal.z, 1.0f);
}

float4 PixelShader_Normal(float2 uv)
{
    float3 normal;
    normal.xy = normalTexture.Sample(defaultSampler, uv).ag;
    // Recreate z
    normal.z = 0.0f;
    normal = (normal * 2.0f) - 1.0f;// Comment this for Normal shader render pass
    normal.z = sqrt(1 - saturate((normal.x * normal.x) + (normal.y * normal.y)));
    //normal = (normal * 0.5f) + 0.5f;// Found in TGA modelviewer shader code, but seems to cause issues here.
    normal = normalize(normal);
    
    return float4(normal.x, normal.y, normal.z, 1.0f);
}

float4 PixelShader_Normal(Texture2D aNormalTexture, float2 uv)
{
    float3 normal;
    normal.xy = aNormalTexture.Sample(defaultSampler, uv).ag;
    // Recreate z
    normal.z = 0.0f;
    normal = (normal * 2.0f) - 1.0f; // Comment this for Normal shader render pass
    normal.z = sqrt(1 - saturate((normal.x * normal.x) + (normal.y * normal.y)));
    //normal = (normal * 0.5f) + 0.5f;// Found in TGA modelviewer shader code, but seems to cause issues here.
    normal = normalize(normal);
    
    return float4(normal.x, normal.y, normal.z, 1.0f);
}

float PixelShader_DetailNormalStrength(float2 uv)
{
	const float output = materialTexture.Sample(defaultSampler, uv).a;
    return output;
}
float4 PixelShader_DetailNormal(float2 uv, int index)
{
	const float tilingModifier = DETAILNORMAL_TILING; // eq to scale
   
    float3 normal;
    
    normal.xy = detailNormals[index].Sample(defaultSampler, uv * tilingModifier).ag;
    normal.z = 0.0f;
    normal = (normal * 2.0f) - 1.0f;
    normal.z = sqrt(1 - saturate((normal.x * normal.x) + (normal.y * normal.y)));
    normal = normalize(normal);
    
    return float4(normal.x, normal.y, normal.z, 1.0f);
}

float4 PixelShader_Material(float2 uv)
{
    float4 material = materialTexture.Sample(defaultSampler, uv).rgba;
    return material;
}

float4 PixelShader_Material(Texture2D aMaterialTexture, float2 uv)
{
    float4 material = aMaterialTexture.Sample(defaultSampler, uv).rgba;
    return material;
}

float PixelShader_AmbientOcclusion(float2 uv)
{
	const float ao = normalTexture.Sample(defaultSampler, uv).b;
    return ao;
}

float PixelShader_AmbientOcclusion(Texture2D aNormalTexture, float2 uv)
{
	const float ao = aNormalTexture.Sample(defaultSampler, uv).b;
    return ao;
}

float PixelShader_Metalness(float2 uv)
{
	const float metalness = PixelShader_Material(uv).r;
    return metalness;
}

float PixelShader_Metalness(Texture2D aMaterialTexture, float2 uv)
{
	const float metalness = PixelShader_Material(aMaterialTexture, uv).r;
    return metalness;
}

float PixelShader_PerceptualRoughness(float2 uv)
{
	const float roughness = PixelShader_Material(uv).g;
    return roughness;
}

float PixelShader_PerceptualRoughness(Texture2D aMaterialTexture, float2 uv)
{
	const float roughness = PixelShader_Material(aMaterialTexture, uv).g;
    return roughness;
}

float PixelShader_Emissive(float2 uv)
{
	const float emissive = PixelShader_Material(uv).b;
    return emissive;
}

float PixelShader_Emissive(Texture2D aMaterialTexture, float2 uv)
{
	const float emissive = PixelShader_Material(aMaterialTexture, uv).b;
    return emissive;
}

float PixelShader_SSAO(float2 uv)
{
	const float ssao = SSAOTexture.Sample(defaultSampler, uv).r;
    return ssao;
}

///////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////
// GBUFFER  GBUFFER GBUFFER GBUFFER GBUFFER GBUFFER GBUFFER GBUFFER GBUFFER GBUFFER GBUFFER
///////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////

float4 GBuffer_Albedo(float2 uv)
{
    float3 albedo = albedoTextureGBuffer.Sample(defaultSampler, uv).rgb;
    return float4(albedo.r, albedo.g, albedo.b, 1.0f);
}

float4 GBuffer_Normal(float2 uv)
{
    float3 normal = normalTextureGBuffer.Sample(defaultSampler, uv).rgb;

// Recreate z
    // Fr�ga bj�rn 18/2 :)
    //float3 normal;
    //normal.xy = normalTextureGBuffer.Sample(defaultSampler, input.myUV.xy).wy;
    //normal.z = 0.0f;
    //normal.z = sqrt(1.f - saturate((normal.x * normal.x) + (normal.y * normal.y)));   
    //normal.z = sqrt(1.f - (normal.x * normal.x) - (normal.y * normal.y));// Unity
    // random tests to see what works
        //normal = normalize(normal);
        //normal.z *= -1.0f;
        //normal = normal * 0.5 + 0.5;// dont do this
    
    return float4(normal.x, normal.y, normal.z, 1.0f);
}

float4 GBuffer_VertexNormal(float2 uv)
{
    float3 vertexNormal = vertexNormalTextureGBuffer.Sample(defaultSampler, uv).rgb;
    return float4(vertexNormal.r, vertexNormal.g, vertexNormal.b, 1.0f);
}

float GBuffer_AmbientOcclusion(float2 uv)
{
	const float ao = materialTextureGBuffer.Sample(defaultSampler, uv).a;
    return ao;
}

float GBuffer_Metalness(float2 uv)
{
	const float metalness = materialTextureGBuffer.Sample(defaultSampler, uv).r;
    return metalness;
}

float GBuffer_PerceptualRoughness(float2 uv)
{
	const float roughness = materialTextureGBuffer.Sample(defaultSampler, uv).g;
    return roughness;
}

float GBuffer_Emissive(float2 uv)
{
	const float emissive = materialTextureGBuffer.Sample(defaultSampler, uv).b;
    return emissive * myEmissiveStrength;
}

///////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////
// SHADOWS SHADOWS SHADOWS SHADOWS SHADOWS SHADOWS SHADOWS SHADOWS SHADOWS SHADOWS SHADOWS
///////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////

