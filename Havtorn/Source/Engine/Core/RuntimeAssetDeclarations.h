// Copyright 2022 Team Havtorn. All Rights Reserved.

#pragma once

#include "FileSystem/FileHeaderDeclarations.h"
#include "Graphics/GraphicsStructs.h"

namespace Havtorn
{
	struct SStaticMeshAsset
	{
		SStaticMeshAsset() = default;

		explicit SStaticMeshAsset(const SStaticModelFileHeader assetFileData)
			: AssetType(assetFileData.AssetType)
			, Name(assetFileData.Name)
		{
			for (auto& mesh : assetFileData.Meshes)
			{
				DrawCallData.emplace_back();
				DrawCallData.back().IndexCount = mesh.NumberOfIndices;
			}
		}

		EAssetType AssetType = EAssetType::StaticModel;
		std::string Name;
		std::vector<SDrawCallData> DrawCallData;
	};
}