struct VertexToPixel
{
    float4 Position : SV_POSITION;
    float4 Normal : NORMAL;
    float4 Tangent : TANGENT;
    float4 Bitangent : BINORMAL;
    float2 UV : UV;
};

struct PixelOutput
{
    float4 color : SV_TARGET0;
};

PixelOutput main(VertexToPixel input)
{
    float ambientStrength = 0.2f;
    float3 lightColor = float3(1.0f, 1.0f, 1.0f);
    float3 ambientLight = ambientStrength * lightColor;
    
    //float3 lightPos = { 1.0f, 1.0f, 1.0f }; 
    float3 albedo = float3(0.966f, 0.420f, 0.106f);

    float3 normal = input.Normal.xyz;
    //float3x3 tangentSpaceMatrix = float3x3(normalize(input.Tangent.xyz), normalize(input.Bitangent.xyz), normalize(input.Normal.xyz));
    //normal = mul(normal, tangentSpaceMatrix);
    //normal = normalize(normal);
    
    // === Directional light
    float3 lightDir = normalize(float3(1.0f, -1.0f, 1.0f));
    
    float NdL = saturate(dot(normal, -lightDir));
    float lambert = NdL; // Angle attenuation
    
    float3 diffuseLight = saturate(lightColor * lambert);
    // === !Directional Light
    //float3 lightDir = normalize(lightPos - input.Position.xyz);
    //float3 diffuseLight = max(dot(normal, lightDir), 0.0f) * lightColor;
    
    PixelOutput output;
    output.color.rgb = (ambientLight + diffuseLight) * albedo;
    output.color.a = 1.0f;
    //output.color.rgb = abs(normal);
    return output;
}