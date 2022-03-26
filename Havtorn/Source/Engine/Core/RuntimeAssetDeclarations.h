// Copyright 2022 Team Havtorn. All Rights Reserved.

#pragma once

#include "FileSystem/FileHeaderDeclarations.h"

namespace Havtorn
{
	struct SStaticMeshAsset
	{
		SStaticMeshAsset() = default;

		explicit SStaticMeshAsset(const SStaticMeshFileHeader assetFileData)
			: AssetType(assetFileData.AssetType)
			, Name(assetFileData.Name)
			, IndexCount(assetFileData.NumberOfIndices)
		{
		}

		EAssetType AssetType = EAssetType::StaticMesh;
		std::string Name;
		U32 IndexCount = 0;

		U16 VertexBufferIndex = 0;
		U16 IndexBufferIndex = 0;
		U16 VertexStrideIndex = 0;
		U16 VertexOffsetIndex = 0;
	};
}