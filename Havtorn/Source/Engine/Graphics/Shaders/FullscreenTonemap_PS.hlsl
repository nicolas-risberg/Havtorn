// Copyright 2022 Team Havtorn. All Rights Reserved.

#include "Includes/FullscreenShaderStructs.hlsli"

float3 Uncharted2Tonemap(const float3 x)
{
	const float a = 0.15f;
	const float b = 0.5f;
	const float c = 0.1f;
	const float d = 0.2f;
	const float e = 0.02f;
	const float f = 0.3f;
    
    return ((x * (a * x + c * b) + d * e) / (x * (a * x + b) + d * f)) - e / f;
}

// sRGB => XYZ => D65_2_D60 => AP1 => RRT_SAT
static const float3x3 ACESInputMat =
{
    { 0.59719, 0.35458, 0.04823 },
    { 0.07600, 0.90834, 0.01566 },
    { 0.02840, 0.13383, 0.83777 }
};

// ODT_SAT => XYZ => D60_2_D65 => sRGB
static const float3x3 ACESOutputMat =
{
    { 1.60475, -0.53108, -0.07367 },
    { -0.10208, 1.10813, -0.00605 },
    { -0.00327, -0.07276, 1.07602 }
};

float3 RRTAndODTFit(float3 v)
{
	const float3 a = v * (v + 0.0245786f) - 0.000090537f;
	const float3 b = v * (0.983729f * v + 0.4329510f) + 0.238081f;
    return a / b;
}

float3 ACESFitted(float3 color)
{
    color = mul(ACESInputMat, color);

    // Apply RRT and ODT
    color = RRTAndODTFit(color);

    color = mul(ACESOutputMat, color);

    // Clamp to [0, 1]
    color = saturate(color);

    return color;
}

float3 ACESApprox(const float3 x)
{
	const float a = 2.51f;
	const float b = 0.03f;
	const float c = 2.43f;
	const float d = 0.59f;
	const float e = 0.14f;
    return saturate((x * (a * x + b)) / (x * (c * x + d) + e));
}

PixelOutput main(VertexToPixel input)
{
    PixelOutput returnValue;
    returnValue.myColor.rgba = 1.0f;
    const float3 resource = fullscreenTexture1.Sample(defaultSampler, input.myUV.xy).rgb;
	
	// No Tonemapping
	//{
    //     returnValue.myColor.rgb = resource;
    //}
	// No Tonemapping

	// Reinhard
    if (myIsReinhard)
    {
        returnValue.myColor.rgb = resource / (resource + 1.0f);
    }
	// Reinhard
    
	// Uncharted 2
    if (myIsUncharted)
    {
	    const float3 whitePoint = (float3)myWhitePointColor * myWhitePointIntensity; //10.0f;
	    const float exposure = myExposure; //3.0f;
        returnValue.myColor.rgb = Uncharted2Tonemap(resource * exposure) / Uncharted2Tonemap(whitePoint);
    }
	// Uncharted 2
    
    // ACES
    if (myIsACES)
    {
	    const float3 whitePoint = (float3)myWhitePointColor * myWhitePointIntensity; //10.0f;
	    const float exposure = myExposure; //3.0f;
        returnValue.myColor.rgb = ACESFitted(resource * exposure) / ACESFitted(whitePoint);
    }
    // ACES
    
    // ACES approx
    //{
    //    float3 whitePoint = 10.0f;
    //    float exposure = 1.0f;
    //    returnValue.myColor.rgb = ACESApprox(resource * exposure) / ACESApprox(whitePoint);
    //}
    // ACES approx
	
    returnValue.myColor.a = 0.0f;
    return returnValue;
};