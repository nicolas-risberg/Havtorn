// Copyright 2022 Team Havtorn. All Rights Reserved.

#pragma once

struct ID3D11ShaderResourceView;

namespace Havtorn
{
	class CGraphicsFramework;

	enum class EEditorTexture
	{
		FolderIcon,
		FileIcon,
		Count
	};

	class CEditorResourceManager
	{
	public:
		CEditorResourceManager() = default;
		~CEditorResourceManager() = default;

		bool Init(const CGraphicsFramework* framework);
		ID3D11ShaderResourceView* GetEditorTexture(EEditorTexture texture) const;

	private:
		std::string GetFileName(EEditorTexture texture);
	
	private:
		std::vector<ID3D11ShaderResourceView*> Textures;
		std::string ResourceAssetPath = "Resources/";
	};
}