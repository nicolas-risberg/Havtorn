// Copyright 2022 Team Havtorn. All Rights Reserved.

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
			Albedo,
			Normal,
			VertexNormal,
			Material,
			Count
		};

	public:
		CGBuffer();
		~CGBuffer();

		void ClearTextures(SVector4 clearColor = (0.0f, 0.0f, 0.0f, 0.0f));
		void ReleaseRenderTargets();
		void SetAsActiveTarget(class CFullscreenTexture* depth = nullptr);
		void SetAsResourceOnSlot(EGBufferTextures resource, U16 slot);
		void SetAllAsResources(U16 startSlot);

		void ReleaseResources();

	private:
		ID3D11DeviceContext* Context;
		std::array<ID3D11Texture2D*, static_cast<U64>(EGBufferTextures::Count)> Textures;
		std::array<ID3D11RenderTargetView*, static_cast<U64>(EGBufferTextures::Count)> RenderTargets;
		std::array<ID3D11ShaderResourceView*, static_cast<U64>(EGBufferTextures::Count)> ShaderResources;
		D3D11_VIEWPORT* Viewport;
	};
}
