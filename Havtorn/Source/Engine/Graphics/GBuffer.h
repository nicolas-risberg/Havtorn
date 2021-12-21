#pragma once

namespace Havtorn
{
	class CGBuffer
	{
	public:
		friend class CFullscreenTextureFactory;

	public:
		enum class EGBufferTextures
		{
			ALBEDO,
			NORMAL,
			VERTEXNORMAL,
			MATERIAL,
			COUNT
		};

	public:
		CGBuffer();
		~CGBuffer();

		void ClearTextures(SVector4 aClearColor = (0.0f, 0.0f, 0.0f, 0.0f));
		void ReleaseRenderTargets();
		void SetAsActiveTarget(class CFullscreenTexture* aDepth = nullptr);
		void SetAsResourceOnSlot(EGBufferTextures aResource, UINT aSlot);
		void SetAllAsResources();

		void ReleaseResources();

	private:
		ID3D11DeviceContext* myContext;
		std::array<ID3D11Texture2D*, static_cast<size_t>(EGBufferTextures::COUNT)> myTextures;
		std::array<ID3D11RenderTargetView*, static_cast<size_t>(EGBufferTextures::COUNT)> myRenderTargets;
		std::array<ID3D11ShaderResourceView*, static_cast<size_t>(EGBufferTextures::COUNT)> myShaderResources;
		D3D11_VIEWPORT* myViewport;
	};
}
