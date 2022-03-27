// Copyright 2022 Team Havtorn. All Rights Reserved.

#include "FBXImporter.h"

// Assimp
#include <assimp/cimport.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/texture.h>

#include "FileSystem/FileSystem.h"

#ifdef _DEBUG
#pragma comment(lib, "assimp-vc142-mtd.lib")
#else
#pragma comment(lib, "assimp-vc142-mt.lib")
#endif

namespace Havtorn
{
	void CFBXImporter::ImportFBX(const std::string& fileName)
	{
		const struct aiScene* assimpScene = nullptr;

		if (!CFileSystem::DoesFileExist(fileName))
			return;

		assimpScene = aiImportFile(fileName.c_str(), aiProcessPreset_TargetRealtime_Fast | aiProcess_ConvertToLeftHanded);

		if (!assimpScene)
			HV_LOG_ERROR("FBXImporter failed to import %s!", fileName.c_str());
	}
}
