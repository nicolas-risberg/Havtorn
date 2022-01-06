struct VertexToPixel
{
    float4 Position : SV_POSITION;
    float4 Normal : NORMAL;
    float4 Tangent : TANGENT;
    float4 Bitangent : BITANGENT;
    float2 UV : UV;
};

struct PixelOutput
{
    float4 color : SV_TARGET0;
};

PixelOutput main(VertexToPixel input)
{
    float ambientStrength = 0.1f;
    float3 lightColor = float3(1.0f, 1.0f, 1.0f);
    float3 ambientLight = ambientStrength * lightColor;
    
    float3 lightPos = { 1.0f, 1.0f, 1.0f }; 
    float3 albedo = float3(1.0f, 0.0f, 0.0f);

    float3x3 tangentSpaceMatrix = float3x3(normalize(input.Tangent.xyz), normalize(input.Bitangent.xyz), normalize(input.Normal.xyz));
    float3 normal = mul(input.Normal.xyz, tangentSpaceMatrix);
    normal = normalize(normal);
    
    float3 lightDir = normalize(lightPos - input.Position.xyz);
    float3 diffuseLight = max(dot(normal, lightDir), 0.0f) * lightColor;
    
    PixelOutput output;
    output.color.rgb = (ambientLight + diffuseLight) * albedo;
    output.color.a = 1.0f;
    //output.color = input.Normal;
    return output;
}