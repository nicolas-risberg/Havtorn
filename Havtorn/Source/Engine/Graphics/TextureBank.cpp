// Copyright 2022 Team Havtorn. All Rights Reserved.

#include "hvpch.h"
#include "TextureBank.h"
#include "Engine.h"
#include "GraphicsFramework.h"
#include "GraphicsUtilities.h"

namespace Havtorn
{
	CTextureBank::~CTextureBank()
	{
		Device = nullptr;
		for (auto& texture : Textures)
		{
			texture->Release();
		}

		Textures.clear();
	}

	U32 CTextureBank::GetTextureIndex(const std::string& fileName)
	{
		if (!Indices.contains(fileName))
			AddTexture(fileName);

		return Indices[fileName];
	}

	ID3D11ShaderResourceView* CTextureBank::GetTexture(const std::string& fileName)
	{
		U32 index = GetTextureIndex(fileName);
		return Textures[index];
	}

	ID3D11ShaderResourceView* CTextureBank::GetTexture(U32 index)
	{
		// NR: Assume Textures[index] exists, otherwise we just crash here, which is fine. 
		// Might be unnecessarily expensive to check error popups here.
		return Textures[index];
	}

	const std::vector<ID3D11ShaderResourceView*>& CTextureBank::GetTextures()
	{
		return Textures;
	}

	bool CTextureBank::Init(const CGraphicsFramework* framework)
	{
		Device = framework->GetDevice();

		if (!Device)
			return false;

		return true;
	}

	void CTextureBank::AddTexture(const std::string& filePath)
	{
		Textures.emplace_back(std::move(UGraphicsUtils::TryGetShaderResourceView(Device, filePath)));
		//std::string fileName = filePath.substr(filePath.find_last_of("/") + 1);
		Indices.emplace(filePath, static_cast<U32>(Textures.size() - 1));
	}
}