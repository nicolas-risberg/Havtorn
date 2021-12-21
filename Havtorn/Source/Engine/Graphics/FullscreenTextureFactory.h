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
		bool Init(CDirectXFramework* aFramework);
		CFullscreenTexture CreateTexture(SVector2<F32> aSize, DXGI_FORMAT aFormat);
		CFullscreenTexture CreateTexture(ID3D11Texture2D* aTexture);
		CFullscreenTexture CreateTexture(SVector2<F32> aSize, DXGI_FORMAT aFormat, const std::string& aFilePath);
		CFullscreenTexture CreateDepth(SVector2<F32> aSize, DXGI_FORMAT aFormat);
		class CGBuffer CreateGBuffer(SVector2<F32> aSize);

	private:
		ID3D11ShaderResourceView* GetShaderResourceView(ID3D11Device* aDevice, std::string aTexturePath);

	private:
		CDirectXFramework* myFramework;
	};
}
