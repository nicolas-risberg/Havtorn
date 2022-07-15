// Copyright 2022 Team Havtorn. All Rights Reserved.

struct VertexToPixel
{
    float4 Position : SV_POSITION;
    float4 Normal : NORMAL;
    float4 Tangent : TANGENT;
    float4 Bitangent : BINORMAL;
    float2 UV : UV;
};

//Texture2D albedoTexture : register(t0);
//Texture2D materialTexture : register(t1);
//Texture2D normalTexture : register(t2);
SamplerState defaultSampler : register(s0);

struct PixelOutput
{
    float4 Color : SV_TARGET0;
};

PixelOutput main(VertexToPixel input)
{
	const float ambientStrength = 0.2f;
	const float3 lightColor = float3(1.0f, 1.0f, 1.0f);
	const float3 ambientLight = ambientStrength * lightColor;
    
	//const float3 albedo = float3(0.966f, 0.420f, 0.106f);
    const float3 albedo = float3(0.8f, 0.8f, 0.8f);
    //const float3 albedo = albedoTexture.Sample(defaultSampler, input.UV).xyz;

    const float3 normal = input.Normal.xyz;
    //// === Packed Normals ===
    //float3 normal;
    //normal.xy = normalTexture.Sample(defaultSampler, input.UV).ag;
    //// Recreate z
    //normal.z = 0.0f;
    //normal = (normal * 2.0f) - 1.0f; // Comment this for Normal shader render pass
    //normal.z = sqrt(1 - saturate((normal.x * normal.x) + (normal.y * normal.y)));
    //normal = normalize(normal);
    //// TBN space
    //float3x3 tangentSpaceMatrix = float3x3(normalize(input.Tangent.xyz), normalize(input.Bitangent.xyz), normalize(input.Normal.xyz));
    //normal = mul(normal, tangentSpaceMatrix);
    //normal = normalize(normal);
    //// === !PackedNormals ===
    
    // === Directional light ===
    const float3 lightDir = normalize(float3(1.0f, -1.0f, 1.0f));

    const float NdL = saturate(dot(normal, -lightDir));
    const float lambert = NdL; // Angle attenuation

    const float3 diffuseLight = saturate(lightColor * lambert);
    // === !Directional Light ===
    
    //float3 lightPos = { 1.0f, 1.0f, 1.0f };
    //float3 lightDir = normalize(lightPos - input.Position.xyz);
    //float3 diffuseLight = max(dot(normal, lightDir), 0.0f) * lightColor;
    
    PixelOutput output;
    output.Color.rgb = (ambientLight + diffuseLight) * albedo;
    output.Color.a = 1.0f;
    return output;
}