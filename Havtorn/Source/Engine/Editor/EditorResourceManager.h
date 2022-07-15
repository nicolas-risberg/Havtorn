// Copyright 2022 Team Havtorn. All Rights Reserved.

#pragma once

struct ID3D11ShaderResourceView;

namespace Havtorn
{
	class CGraphicsFramework;
	class CRenderManager;

	enum class EEditorTexture
	{
		FolderIcon,
		FileIcon,
		PlayIcon,
		PauseIcon,
		StopIcon,
		Count
	};

	class CEditorResourceManager
	{
	public:
		CEditorResourceManager() = default;
		~CEditorResourceManager() = default;

		bool Init(CRenderManager* renderManager, const CGraphicsFramework* framework);
		ID3D11ShaderResourceView* GetEditorTexture(EEditorTexture texture) const;

		void* RenderAssetTexure(EAssetType assetType, const std::string& fileName);

	private:
		std::string GetFileName(EEditorTexture texture);
	
	private:
		std::vector<ID3D11ShaderResourceView*> Textures;
		CRenderManager* RenderManager = nullptr;
		std::string ResourceAssetPath = "Resources/";
	};
}