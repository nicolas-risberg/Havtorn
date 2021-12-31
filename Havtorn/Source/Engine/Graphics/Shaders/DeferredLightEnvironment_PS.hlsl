#include "Includes/PBRDeferredAmbiance.hlsli"
#include "Includes/DeferredPBRFunctions.hlsli"

PixelOutput main(VertexToPixel input)
{
    PixelOutput output;

    float depth = PixelShader_Exists(input.myUV).r;
    if (depth == 1)
    {
        output.myColor = GBuffer_Albedo(input.myUV);
        return output;
    }
    
    float3 worldPosition = PixelShader_WorldPosition(input.myUV).rgb;
    float3 toEye = normalize( cameraPosition.xyz - worldPosition.xyz);
    float3 albedo = GBuffer_Albedo(input.myUV).rgb;
    albedo = GammaToLinear(albedo);
    float3 normal = GBuffer_Normal(input.myUV).xyz;
    float3 vertexNormal = GBuffer_VertexNormal(input.myUV).xyz;
    float ambientOcclusion = GBuffer_AmbientOcclusion(input.myUV);
    float metalness = GBuffer_Metalness(input.myUV);
    float perceptualRoughness = GBuffer_PerceptualRoughness(input.myUV);
    float emissiveData = GBuffer_Emissive(input.myUV);
    float ssao = PixelShader_SSAO(input.myUV);
    
    float3 specularColor = lerp((float3) 0.04, albedo, metalness);
    float3 diffuseColor = lerp((float3) 0.00, albedo, 1 - metalness);
    
    float3 ambiance = EvaluateAmbiance(environmentTexture, normal, vertexNormal, toEye, perceptualRoughness, metalness, albedo, ambientOcclusion, diffuseColor, specularColor);
    float3 directionalLight = EvaluateDirectionalLight(diffuseColor, specularColor, normal, perceptualRoughness, directionalLightColor.rgb * directionalLightColor.a, toDirectionalLight.xyz, toEye.xyz);
    float3 emissive = albedo * emissiveData;
    float3 radiance = (ambiance * ssao) + directionalLight * (1.0f - ShadowFactor(worldPosition, directionalLightPosition.xyz, toDirectionalLightView, toDirectionalLightProjection, shadowDepthTexture, shadowSampler, directionalLightShadowMapResolution)) + emissive;

    output.myColor.rgb = radiance;
    output.myColor.a = 1.0f;
    return output;
}