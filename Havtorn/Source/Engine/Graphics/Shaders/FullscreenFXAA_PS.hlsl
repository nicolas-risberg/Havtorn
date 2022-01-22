// Copyright 2022 Team Havtorn. All Rights Reserved.

#include "Includes/FullscreenShaderStructs.hlsli"
#pragma warning( disable : 4000 )

// Tutorial:
// https://catlikecoding.com/unity/tutorials/advanced-rendering/fxaa/

// Trims the algorithm from processing darks.
//   0.0833 - upper limit (default, the start of visible unfiltered edges)
//   0.0625 - high quality (faster)
//   0.0312 - visible limit (slower)
static const float contrastThreshold = 0.0625f;

// The minimum amount of local contrast required to apply algorithm.
//   0.333 - too little (faster)
//   0.250 - low quality
//   0.166 - default
//   0.125 - high quality 
//   0.063 - overkill (slower)
static const float relativeThreshold = 0.125f;

// Choose the amount of sub-pixel aliasing removal.
// This can effect sharpness.
//   1.00 - upper limit (softer)
//   0.75 - default amount of filtering
//   0.50 - lower limit (sharper, less sub-pixel aliasing removal)
//   0.25 - almost off
//   0.00 - completely off
static const float subpixelBlending = 1.0f;

// Maximum numbers of steps when getting edge blend
static const int edgeStepCount = 10; // Low: 4;
// Step size for each step in the blending process
static const float edgeSteps[10] = { 1.0f, 1.5f, 2.0f, 2.0f, 2.0f, 2.0f, 2.0f, 2.0f, 2.0f, 4.0f }; // Low: { 1.0f, 1.5f, 2.0f, 4.0f };
// Used if couldn't find edge blend when stepping
static const float edgeGuess = 8.0f; // Low: 12.0f;

struct LuminanceData
{
    float m, n, e, s, w;
    float ne, nw, se, sw;
    float highest, lowest, contrast;
};

float2 GetTexelSize()
{
    return float2(1.0f / myResolution.x, 1.0f / myResolution.y);
}

float LinearRGBToLuminance(float3 linearRgb)
{
    return dot(linearRgb, float3(0.2126729f, 0.7151522f, 0.0721750f));
}

float4 Sample(float2 uv)
{
    return fullscreenTexture1.SampleLevel(defaultSampler, uv, 0);
}

float SampleLuminance(float2 uv, float uOffset, float vOffset)
{
    uv += GetTexelSize() * float2(uOffset, vOffset);
    float4 color = Sample(uv);
    //color.rgb = GammaToLinear(color.rgb);
	// Might want to convert to linear, currently using gamma
    return LinearRGBToLuminance(saturate(color.rgb));
}

LuminanceData SampleLuminanceNeighborhood(float2 uv)
{
    LuminanceData l;
    l.m = SampleLuminance(uv, 0, 0);
    l.n = SampleLuminance(uv, 0, 1);
    l.e = SampleLuminance(uv, 1, 0);
    l.s = SampleLuminance(uv, 0, -1);
    l.w = SampleLuminance(uv, -1, 0);

    l.ne = SampleLuminance(uv, 1, 1);
    l.nw = SampleLuminance(uv, -1, 1);
    l.se = SampleLuminance(uv, 1, -1);
    l.sw = SampleLuminance(uv, -1, -1);

    l.highest = max(max(max(max(max(max(max(max(l.sw, l.se), l.nw), l.ne), l.n), l.e), l.s), l.w), l.m);
    l.lowest = min(min(min(min(min(min(min(min(l.sw, l.se), l.nw), l.ne), l.n), l.e), l.s), l.w), l.m);
    l.contrast = l.highest - l.lowest;
    return l;
}

bool ShouldSkipPixel(LuminanceData l)
{
	const float threshold = max(contrastThreshold, relativeThreshold * l.highest);
    return l.contrast < threshold;
}

float DeterminePixelBlendFactor(LuminanceData l)
{
    float filter = 2 * (l.n + l.e + l.s + l.w);
    filter += l.ne + l.nw + l.se + l.sw;
    filter *= 1.0 / 12;
    filter = abs(filter - l.m);
    filter = saturate(filter / l.contrast);
    const float blendFactor = smoothstep(0, 1, filter);
    return blendFactor * blendFactor * subpixelBlending;
}

struct EdgeData
{
    bool isHorizontal;
    float pixelStep;
    float oppositeLuminance, gradient;
};

EdgeData DetermineEdge(LuminanceData l)
{
    EdgeData e;
    const float horizontal =
		abs(l.n + l.s - 2 * l.m) * 2 +
		abs(l.ne + l.se - 2 * l.e) +
		abs(l.nw + l.sw - 2 * l.w);
    const float vertical =
		abs(l.e + l.w - 2 * l.m) * 2 +
		abs(l.ne + l.nw - 2 * l.n) +
		abs(l.se + l.sw - 2 * l.s);
    e.isHorizontal = horizontal >= vertical;

    const float pLuminance = e.isHorizontal ? l.n : l.e;
    const float nLuminance = e.isHorizontal ? l.s : l.w;
    const float pGradient = abs(pLuminance - l.m);
    const float nGradient = abs(nLuminance - l.m);

    const float2 texSize = GetTexelSize();
    e.pixelStep = e.isHorizontal ? texSize.y : texSize.x;

    if (pGradient < nGradient)
    {
        e.pixelStep = -e.pixelStep;
        e.oppositeLuminance = nLuminance;
        e.gradient = nGradient;
    }
    else
    {
        e.oppositeLuminance = pLuminance;
        e.gradient = pGradient;
    }

    return e;
}

float DetermineEdgeBlendFactor(LuminanceData l, EdgeData e, float2 uv)
{
    float2 uvEdge = uv;
    float2 edgeStep;
    const float2 texSize = GetTexelSize();
    if (e.isHorizontal)
    {
        uvEdge.y += e.pixelStep * 0.5;
        edgeStep = float2(texSize.x, 0);
    }
    else
    {
        uvEdge.x += e.pixelStep * 0.5;
        edgeStep = float2(0, texSize.y);
    }

    const float edgeLuminance = (l.m + e.oppositeLuminance) * 0.5;
    const float gradientThreshold = e.gradient * 0.25;
	
	// Sample positive
    float2 puv = uvEdge + edgeStep * edgeSteps[0];
    float pLuminanceDelta = SampleLuminance(puv, 0, 0) - edgeLuminance;
    bool pAtEnd = abs(pLuminanceDelta) >= gradientThreshold;
	
    for (int posSampleIndex = 1; posSampleIndex < edgeStepCount && !pAtEnd; posSampleIndex++)
    {
        puv += edgeStep * edgeSteps[posSampleIndex];
        pLuminanceDelta = SampleLuminance(puv, 0, 0) - edgeLuminance;
        pAtEnd = abs(pLuminanceDelta) >= gradientThreshold;
    }
    if (!pAtEnd)
    {
        puv += edgeStep * edgeGuess;
    }

	// Sample negative
    float2 nuv = uvEdge - edgeStep * edgeSteps[0];
    float nLuminanceDelta = SampleLuminance(nuv, 0, 0) - edgeLuminance;
    bool nAtEnd = abs(nLuminanceDelta) >= gradientThreshold;

    for (int negSampleIndex = 1; negSampleIndex < edgeStepCount && !nAtEnd; negSampleIndex++)
    {
        nuv -= edgeStep * edgeSteps[negSampleIndex];
        nLuminanceDelta = SampleLuminance(nuv, 0, 0) - edgeLuminance;
        nAtEnd = abs(nLuminanceDelta) >= gradientThreshold;
    }
    if (!nAtEnd)
    {
        nuv -= edgeStep * edgeGuess;
    }

    float pDistance, nDistance;
    if (e.isHorizontal)
    {
        pDistance = puv.x - uv.x;
        nDistance = uv.x - nuv.x;
    }
    else
    {
        pDistance = puv.y - uv.y;
        nDistance = uv.y - nuv.y;
    }
	
    float shortestDistance;
    bool deltaSign;
    if (pDistance <= nDistance)
    {
        shortestDistance = pDistance;
        deltaSign = pLuminanceDelta >= 0;
    }
    else
    {
        shortestDistance = nDistance;
        deltaSign = nLuminanceDelta >= 0;
    }
	
    if (deltaSign == (l.m - edgeLuminance >= 0))
    {
        return 0;
    }
    return 0.5 - shortestDistance / (pDistance + nDistance);
}

float4 ApplyFXAA(VertexToPixel input)
{
    float4 returnValue;
    float2 uv = input.myUV.xy;
    LuminanceData l = SampleLuminanceNeighborhood(uv);
    if (ShouldSkipPixel(l))
    {
        returnValue = Sample(input.myUV.xy);
        return returnValue;
    }

    const float pixelBlend = DeterminePixelBlendFactor(l);
    const EdgeData e = DetermineEdge(l);
    const float edgeBlend = DetermineEdgeBlendFactor(l, e, uv);
    const float finalBlend = max(pixelBlend, edgeBlend);

    if (e.isHorizontal)
    {
        uv.y += e.pixelStep * finalBlend;
    }
    else
    {
        uv.x += e.pixelStep * finalBlend;
    }
    returnValue = float4(Sample(uv).rgb, l.m);
    return returnValue;
}

PixelOutput main(VertexToPixel input)
{
    PixelOutput returnValue;
    returnValue.myColor = ApplyFXAA(input);
    return returnValue;
}