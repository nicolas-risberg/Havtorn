// Copyright 2022 Team Havtorn. All Rights Reserved.

#include "hvpch.h"
#include "GBuffer.h"
#include "FullscreenTexture.h"

namespace Havtorn
{
	CGBuffer::CGBuffer()
		: Context(nullptr)
		, Textures()
		, RenderTargets()
		, ShaderResources()
		, Viewport(nullptr)
	{
	}

	CGBuffer::~CGBuffer()
	{
	}

	void CGBuffer::ClearTextures(SVector4 clearColor)
	{
		for (UINT i = 0; i < static_cast<U64>(EGBufferTextures::Count); ++i) 
		{
			Context->ClearRenderTargetView(RenderTargets[i], &clearColor.X);
		}
	}

	void CGBuffer::ReleaseRenderTargets()
	{
		std::array<ID3D11RenderTargetView*, static_cast<U64>(EGBufferTextures::Count)> nullViews = { NULL, NULL, NULL, NULL };
		Context->OMSetRenderTargets(static_cast<U64>(EGBufferTextures::Count), &nullViews[0], nullptr);
	}

	void CGBuffer::SetAsActiveTarget(CFullscreenTexture* depth)
	{
		auto depthStencilView = depth ? depth->myDepth : nullptr;
		Context->OMSetRenderTargets(static_cast<U64>(EGBufferTextures::Count), &RenderTargets[0], depthStencilView);
		Context->RSSetViewports(1, Viewport);
	}

	void CGBuffer::SetAsResourceOnSlot(EGBufferTextures resource, U16 slot)
	{
		Context->PSSetShaderResources(slot, 1, &ShaderResources[static_cast<U64>(resource)]);
	}

	void CGBuffer::SetAllAsResources(U16 startSlot)
	{
		Context->PSSetShaderResources(startSlot, static_cast<U64>(EGBufferTextures::Count), &ShaderResources[0]);
	}

	void CGBuffer::ReleaseResources()
	{
		Context = nullptr;

		for (UINT i = 0; i < static_cast<U64>(EGBufferTextures::Count); ++i)
		{
			Textures[i]->Release();
			Textures[i] = nullptr;
			RenderTargets[i]->Release();
			RenderTargets[i] = nullptr;
			ShaderResources[i]->Release();
			ShaderResources[i] = nullptr;
		}

		delete Viewport;
		Viewport = nullptr;
	}
}
