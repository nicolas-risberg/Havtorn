// Copyright 2022 Team Havtorn. All Rights Reserved.

#include "Includes/FullscreenShaderStructs.hlsli"

PixelOutput main(VertexToPixel input) {
	PixelOutput returnValue;
	const float3 resource = fullscreenTexture1.Sample(defaultSampler, input.myUV.xy).rgb;
	
	//Snippet 1
	{
		const float luminance = dot(resource, float3(0.2126f, 0.7152f, 0.0722f));
        returnValue.myColor.rgb = resource * pow(luminance, 5);
    }
	//Snippet 1
	
	//Snippet 2
	//{
	//	float luminance = dot(resource, float3(0.2126f, 0.7152f, 0.0722f));
	//	float cutoff = 0.8f;
		
	//	if (luminance >= cutoff) {
	//		returnValue.myColor.rgb = resource;
	//	} else {
	//		returnValue.myColor.rgb = float3(0.0f, 0.0f, 0.0f);
	//	}
	//}
	//Snippet 2
	
	//Snippet 3
	//{
 //       float luminance = dot(resource, float3(0.2126f, 0.7152f, 0.0722f));
 //       float cutoff = 0.5f;
 //       luminance = saturate(luminance - cutoff);
 //       returnValue.myColor.rgb = resource * luminance * (1.0f / cutoff);
 //   }
	//Snippet 3
	
	//Snippet 4
	//{
 //       float luminance = dot(resource, float3(0.2126f, 0.7152f, 0.0722f));
 //       float cutoff = 0.8f;
 //       if (luminance >= cutoff)
 //       {
 //           returnValue.myColor.rgb = resource;
 //       }
 //       else if (luminance >= cutoff * 0.5f)
 //       {
 //           float fade = luminance / cutoff;
 //           fade = pow(fade, 5);
 //           returnValue.myColor.rgb = resource * fade;
 //       }
 //       else
 //       {
 //           returnValue.myColor.rgb = float3(0.0f, 0.0f, 0.0f);
 //       }
 //   }
	//Snippet 4
	
	returnValue.myColor.a = 1.0f;
	return returnValue;
};