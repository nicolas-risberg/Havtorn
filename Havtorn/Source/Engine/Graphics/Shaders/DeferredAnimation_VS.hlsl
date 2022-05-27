// Copyright 2022 Team Havtorn. All Rights Reserved.

#include "Includes/DeferredShaderStructs.hlsli"

VertexModelToPixel main(VertexModelInput input)
{
    VertexModelToPixel returnValue;
     
    //returnValue.myPosition = input.myPosition;

    float4 vWeights = input.BoneWeight;
    uint4 vBones = uint4((uint) input.BoneID.x, (uint) input.BoneID.y, (uint) input.BoneID.z, (uint) input.BoneID.w);
    //returnValue.myPosition = mul(returnValue.myPosition, toWorld);
    
    float4 skinnedPos = 0;

    /// Bone 0
    uint iBone = vBones.x;
    float fWeight = vWeights.x;
    const float4 pos = input.Position;
    skinnedPos += fWeight * mul(pos, Bones[iBone]);
    
    /// Bone 1
    iBone = vBones.y;
    fWeight = vWeights.y;
    skinnedPos += fWeight * mul(pos, Bones[iBone]);
    
    /// Bone 2
    iBone = vBones.z;
    fWeight = vWeights.z;
    skinnedPos += fWeight * mul(pos, Bones[iBone]);
    
    /// Bone 3
    iBone = vBones.w;
    fWeight = vWeights.w;
    skinnedPos += fWeight * mul(pos, Bones[iBone]);
    
    input.Position.x = skinnedPos.x;
    input.Position.y = skinnedPos.y;
    input.Position.z = skinnedPos.z;
    input.Position.w = skinnedPos.w;

    const float4 vertexObjectPos = input.Position.xyzw;
    const float4 vertexWorldPos = mul(toWorld, vertexObjectPos);
    const float4 vertexViewPos = mul(ToCameraSpace, vertexWorldPos);
    const float4 vertexProjectionPos = mul(ToProjectionSpace, vertexViewPos);

    const float3x3 toWorldRotation = (float3x3) toWorld;
    float3 vertexWorldNormal = mul(toWorldRotation, input.Normal.xyz);
    float3 vertexWorldTangent = mul(toWorldRotation, input.Tangent.xyz);
    float3 vertexWorldBinormal = mul(toWorldRotation, input.Bitangent.xyz);

    returnValue.Position = vertexProjectionPos;
    returnValue.WorldPosition = vertexWorldPos;
    returnValue.Normal = float4(vertexWorldNormal, 0);
    returnValue.Tangent = float4(vertexWorldTangent, 0);
    returnValue.Bitangent = float4(vertexWorldBinormal, 0);
    returnValue.UV = input.UV;
    return returnValue;
}