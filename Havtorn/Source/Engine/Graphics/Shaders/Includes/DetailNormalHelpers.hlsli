// Copyright 2022 Team Havtorn. All Rights Reserved.

#ifndef DetailNormalHelpers
#define DetailNormalHelpers

#define DETAILNORMAL_TILING 4.0f

#define DETAILNORMAL_1 0
#define DETAILNORMAL_2 1
#define DETAILNORMAL_3 2
#define DETAILNORMAL_4 3

// Detail normal strength value 0.1f - 0.24f == DETAILNORMAL_1
#define DETAILNORMAL_1_STR_RANGE_MIN 0.02f
// Detail normal strength value 0.26f - 0.49f == DETAILNORMAL_2
#define DETAILNORMAL_2_STR_RANGE_MIN 0.26f
// Detail normal strength value 0.51f - 0.74f == DETAILNORMAL_3
#define DETAILNORMAL_3_STR_RANGE_MIN 0.51f
// Detail normal strength value 0.76f - 1.0f == DETAILNORMAL_4
#define DETAILNORMAL_4_STR_RANGE_MIN 0.76f
#define DETAILNORMAL_STR_RANGE_DIFF 0.23f

#define DETAILNORMAL_DISTANCE_MAX 1.0f /* Don't go higher than 4.0f. Gives no noticable effect. */

float3 SetDetailNormalStrength(float3 detailNormal, float detailNormalStrength, float strengthMultiplier)
{
    float3 returnValue = lerp(float3(0.0, 0.0, 1.0), detailNormal, clamp(detailNormalStrength * strengthMultiplier, 0.0, 4.0));
    return returnValue;
}

//Blending together 2 normals. They should be in 0.0-1.0 range when using this function. https://blog.selfshadow.com/publications/blending-in-detail/
float3 BlendRNM(float3 normal1, float3 normal2)
{
    float3 t = normal1 * float3(2, 2, 2) + float3(-1, -1, 0);
    const float3 u = normal2.xyz * float3(-2, -2, 2) + float3(1, 1, -1);
    const float3 r = t * dot(t, u) - u * t.z;
    return normalize(r);
}

float DetailStrengthDistanceMultiplier(float3 eyePos, float3 vertWorldPos)
{
	const float dist = distance(eyePos, vertWorldPos);
	const float multiplier = clamp((1.0f / (dist * dist)), 0.0f, DETAILNORMAL_DISTANCE_MAX);
    return multiplier;
}
#endif
