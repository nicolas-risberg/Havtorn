#include "hvpch.h"
#include "FullScreenTexture.h"
#include <d3d11.h>

namespace Havtorn
{
	CFullscreenTexture::CFullscreenTexture() : myContext(nullptr), myTexture(nullptr), myRenderTarget(nullptr), myShaderResource(nullptr), myViewport(nullptr) {}

	CFullscreenTexture::~CFullscreenTexture() {}

	void CFullscreenTexture::ClearTexture(SVector4 aClearColor) 
	{
		myContext->ClearRenderTargetView(myRenderTarget, &aClearColor.X);
	}

	void CFullscreenTexture::ClearDepth(float /*aClearDepth*/, unsigned int /*aClearStencil*/) 
	{
		myContext->ClearDepthStencilView(myDepth, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	}

	void CFullscreenTexture::SetAsActiveTarget(CFullscreenTexture* aDepth) 
	{
		if (aDepth) {
			myContext->OMSetRenderTargets(1, &myRenderTarget, aDepth->myDepth);
		}
		else {
			myContext->OMSetRenderTargets(1, &myRenderTarget, nullptr);
		}
		myContext->RSSetViewports(1, myViewport);
	}

	void CFullscreenTexture::SetAsDepthTarget()
	{
		myContext->OMSetRenderTargets(0, NULL, myDepth);
		myContext->RSSetViewports(1, myViewport);
	}

	void CFullscreenTexture::SetAsDepthTarget(CFullscreenTexture* anIntermediateRenderTarget)
	{
		myContext->OMSetRenderTargets(1, &anIntermediateRenderTarget->myRenderTarget, myDepth);
		myContext->RSSetViewports(1, myViewport);
	}

	void CFullscreenTexture::SetAsResourceOnSlot(unsigned int aSlot) 
	{
		myContext->PSSetShaderResources(aSlot, 1, &myShaderResource);
	}

	void CFullscreenTexture::ReleaseTexture()
	{
		myContext = nullptr;
		myTexture->Release();
		myTexture = nullptr;
		myRenderTarget->Release();
		myRenderTarget = nullptr;
		if (myShaderResource)
		{
			myShaderResource->Release();
			myShaderResource = nullptr;
		}
		delete myViewport;
		myViewport = nullptr;
	}

	void CFullscreenTexture::ReleaseDepth()
	{
		myContext = nullptr;
		myTexture->Release();
		myTexture = nullptr;
		myDepth->Release();
		myDepth = nullptr;
		if (myShaderResource)
		{
			myShaderResource->Release();
			myShaderResource = nullptr;
		}
		delete myViewport;
		myViewport = nullptr;
	}
}
