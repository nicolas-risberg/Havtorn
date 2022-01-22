// Copyright 2022 Team Havtorn. All Rights Reserved.

#pragma once

struct ID3D11Texture2D;

namespace Havtorn
{
	class CDirectXFramework;
	class CFullscreenTexture;
	
	class CFullscreenTextureFactory 
	{
	public:
		CFullscreenTextureFactory();
		~CFullscreenTextureFactory();
		bool Init(CDirectXFramework* framework);
		CFullscreenTexture CreateTexture(SVector2<F32> size, DXGI_FORMAT format);
		CFullscreenTexture CreateTexture(ID3D11Texture2D* texture);
		CFullscreenTexture CreateTexture(SVector2<F32> size, DXGI_FORMAT format, const std::string& filePath);
		CFullscreenTexture CreateDepth(SVector2<F32> size, DXGI_FORMAT format);
		class CGBuffer CreateGBuffer(SVector2<F32> size);

	private:
		ID3D11ShaderResourceView* GetShaderResourceView(ID3D11Device* device, std::string filePath);

	private:
		CDirectXFramework* Framework;
	};
}
