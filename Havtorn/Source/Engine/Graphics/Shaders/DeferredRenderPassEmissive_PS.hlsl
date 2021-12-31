#include "Includes/PBRDeferredAmbiance.hlsli"
#include "Includes/DeferredPBRFunctions.hlsli"

PixelOutput main(VertexToPixel input)
{
    PixelOutput output;
    
    float emissiveData = GBuffer_Emissive(input.myUV);
    output.myColor.rgb = emissiveData;
    output.myColor.a = 1.0f;
    return output;
}