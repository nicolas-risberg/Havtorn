// Copyright 2022 Team Havtorn. All Rights Reserved.

#pragma once

struct ID3D11DeviceContext;
struct ID3D11Texture2D;
struct ID3D11RenderTargetView;
struct ID3D11DepthStencilView;
struct ID3D11ShaderResourceView;
struct D3D11_VIEWPORT;
class CFullscreenTextureFactory;

namespace Havtorn
{
	class CFullscreenTexture
	{
	public:
		CFullscreenTexture();
		~CFullscreenTexture();
		void ClearTexture(SVector4 aClearColor = { 0.0f, 0.0f, 0.0f, 0.0f });
		void ClearDepth(float aClearDepth = 1.0f, unsigned int aClearStencil = 0);
		void SetAsActiveTarget(CFullscreenTexture* aDepth = nullptr);
		void SetAsDepthTarget();
		void SetAsDepthTarget(CFullscreenTexture* anIntermediateRenderTarget);
		void SetAsResourceOnSlot(unsigned int aSlot);
		void ReleaseTexture();
		void ReleaseDepth();
		
		ID3D11Texture2D* const GetTexture() const;
		ID3D11ShaderResourceView* const GetShaderResourceView() const;

		friend CFullscreenTextureFactory;
		friend class CGBuffer;

	private:
		ID3D11DeviceContext* Context;
		ID3D11Texture2D* myTexture;

		union {
			ID3D11RenderTargetView* myRenderTarget;
			ID3D11DepthStencilView* myDepth;
		};

		ID3D11ShaderResourceView* myShaderResource;
		D3D11_VIEWPORT* Viewport;
	};
}
