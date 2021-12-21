#include "hvpch.h"
#include "GBuffer.h"
#include "FullscreenTexture.h"

namespace Havtorn
{
	CGBuffer::CGBuffer()
	{
	}

	CGBuffer::~CGBuffer()
	{
	}

	void CGBuffer::ClearTextures(SVector4 aClearColor)
	{
		for (UINT i = 0; i < static_cast<size_t>(EGBufferTextures::COUNT); ++i) {
			myContext->ClearRenderTargetView(myRenderTargets[i], &aClearColor.X);
		}
	}

	void CGBuffer::ReleaseRenderTargets()
	{
		std::array<ID3D11RenderTargetView*, static_cast<size_t>(EGBufferTextures::COUNT)> nullViews = { NULL, NULL, NULL, NULL };
		myContext->OMSetRenderTargets(static_cast<size_t>(EGBufferTextures::COUNT), &nullViews[0], nullptr);
	}

	void CGBuffer::SetAsActiveTarget(CFullscreenTexture* aDepth)
	{
		if (aDepth)
		{
			myContext->OMSetRenderTargets(static_cast<size_t>(EGBufferTextures::COUNT), &myRenderTargets[0], aDepth->myDepth);
		}
		else
		{
			myContext->OMSetRenderTargets(static_cast<size_t>(EGBufferTextures::COUNT), &myRenderTargets[0], nullptr);
		}
		myContext->RSSetViewports(1, myViewport);
	}

	void CGBuffer::SetAsResourceOnSlot(EGBufferTextures aResource, UINT aSlot)
	{
		myContext->PSSetShaderResources(aSlot, 1, &myShaderResources[static_cast<size_t>(aResource)]);
	}

	void CGBuffer::SetAllAsResources()
	{
		myContext->PSSetShaderResources(1, static_cast<size_t>(EGBufferTextures::COUNT), &myShaderResources[0]);
	}

	void CGBuffer::ReleaseResources()
	{
		myContext = nullptr;

		for (UINT i = 0; i < static_cast<size_t>(EGBufferTextures::COUNT); ++i)
		{
			myTextures[i]->Release();
			myTextures[i] = nullptr;
			myRenderTargets[i]->Release();
			myRenderTargets[i] = nullptr;
			myShaderResources[i]->Release();
			myShaderResources[i] = nullptr;
		}

		delete myViewport;
		myViewport = nullptr;
	}
}
