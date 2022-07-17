// Copyright 2022 Team Havtorn. All Rights Reserved.

#include "EditorResourceManager.h"
#include "Graphics/GraphicsUtilities.h"
#include "Graphics/RenderManager.h"

namespace Havtorn
{
	ID3D11ShaderResourceView* Havtorn::CEditorResourceManager::GetEditorTexture(EEditorTexture texture) const
	{
		U64 index = static_cast<I64>(texture);
		
		if (index >= Textures.size())
			return nullptr;

		return Textures[index];
	}

	void* CEditorResourceManager::RenderAssetTexure(EAssetType assetType, const std::string& fileName)
	{
		switch (assetType)
		{
		case Havtorn::EAssetType::StaticMesh:
			return std::move(RenderManager->RenderStaticMeshAssetTexture(fileName));
		case Havtorn::EAssetType::SkeletalMesh:
			break;
		case Havtorn::EAssetType::Texture:
			return std::move(RenderManager->GetTextureAssetTexture(fileName));
		case Havtorn::EAssetType::Animation:
			break;
		case Havtorn::EAssetType::AudioOneShot:
			break;
		case Havtorn::EAssetType::AudioCollection:
			break;
		case Havtorn::EAssetType::VisualFX:
			break;
		case Havtorn::EAssetType::None:
		default:
			break;
		}

		return nullptr;
	}

	std::string CEditorResourceManager::GetFileName(EEditorTexture texture)
	{
		std::string extension = ".dds";

		switch (texture)
		{
		case Havtorn::EEditorTexture::FolderIcon:
			return ResourceAssetPath + "FolderIcon" + extension;

		case Havtorn::EEditorTexture::FileIcon:
			return ResourceAssetPath + "FileIcon" + extension;

		case Havtorn::EEditorTexture::PlayIcon:
			return ResourceAssetPath + "PlayIcon" + extension;

		case Havtorn::EEditorTexture::PauseIcon:
			return ResourceAssetPath + "PauseIcon" + extension;

		case Havtorn::EEditorTexture::StopIcon:
			return ResourceAssetPath + "StopIcon" + extension;
		
		case Havtorn::EEditorTexture::Count:
		default:
			return std::string();
		}
	}

	bool Havtorn::CEditorResourceManager::Init(CRenderManager* renderManager, const CGraphicsFramework* framework)
	{
		RenderManager = renderManager;
		ID3D11Device* device = framework->GetDevice();
		I64 textureCount = static_cast<I64>(EEditorTexture::Count);
		
		Textures.assign(textureCount, nullptr);

		for (I64 index = 0; index < textureCount; index++)
		{
			UGraphicsUtils::CreateShaderResourceView(device, GetFileName(static_cast<EEditorTexture>(index)), &Textures[index]);
		}

		return true;
	}
}

