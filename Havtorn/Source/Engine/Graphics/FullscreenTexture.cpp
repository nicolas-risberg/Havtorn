// Copyright 2022 Team Havtorn. All Rights Reserved.

#include "hvpch.h"
#include "FullScreenTexture.h"
#include <d3d11.h>

namespace Havtorn
{
	CFullscreenTexture::CFullscreenTexture() : Context(nullptr), myTexture(nullptr), myRenderTarget(nullptr), myShaderResource(nullptr), Viewport(nullptr) {}

	CFullscreenTexture::~CFullscreenTexture() {}

	void CFullscreenTexture::ClearTexture(SVector4 aClearColor) 
	{
		Context->ClearRenderTargetView(myRenderTarget, &aClearColor.X);
	}

	void CFullscreenTexture::ClearDepth(float /*aClearDepth*/, unsigned int /*aClearStencil*/) 
	{
		Context->ClearDepthStencilView(myDepth, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	}

	void CFullscreenTexture::SetAsActiveTarget(CFullscreenTexture* aDepth) 
	{
		if (aDepth) {
			Context->OMSetRenderTargets(1, &myRenderTarget, aDepth->myDepth);
		}
		else {
			Context->OMSetRenderTargets(1, &myRenderTarget, nullptr);
		}
		Context->RSSetViewports(1, Viewport);
	}

	void CFullscreenTexture::SetAsDepthTarget()
	{
		Context->OMSetRenderTargets(0, NULL, myDepth);
		Context->RSSetViewports(1, Viewport);
	}

	void CFullscreenTexture::SetAsDepthTarget(CFullscreenTexture* anIntermediateRenderTarget)
	{
		Context->OMSetRenderTargets(1, &anIntermediateRenderTarget->myRenderTarget, myDepth);
		Context->RSSetViewports(1, Viewport);
	}

	void CFullscreenTexture::SetAsResourceOnSlot(unsigned int aSlot) 
	{
		Context->PSSetShaderResources(aSlot, 1, &myShaderResource);
	}

	void CFullscreenTexture::ReleaseTexture()
	{
		Context = nullptr;
		myTexture->Release();
		myTexture = nullptr;
		myRenderTarget->Release();
		myRenderTarget = nullptr;
		if (myShaderResource)
		{
			myShaderResource->Release();
			myShaderResource = nullptr;
		}
		delete Viewport;
		Viewport = nullptr;
	}

	void CFullscreenTexture::ReleaseDepth()
	{
		Context = nullptr;
		myTexture->Release();
		myTexture = nullptr;
		myDepth->Release();
		myDepth = nullptr;
		if (myShaderResource)
		{
			myShaderResource->Release();
			myShaderResource = nullptr;
		}
		delete Viewport;
		Viewport = nullptr;
	}

	ID3D11Texture2D* const CFullscreenTexture::GetTexture() const
	{
		return myTexture;
	}
	ID3D11ShaderResourceView* const CFullscreenTexture::GetShaderResourceView() const
	{
		return myShaderResource;
	}
}
