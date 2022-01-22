// Copyright 2022 Team Havtorn. All Rights Reserved.

#include "hvpch.h"
#include "GraphicsFramework.h"
#include "Application/WindowHandler.h"
#include <d3d11.h>

#pragma comment(lib, "runtimeobject.lib")
#pragma comment(lib, "d3d11.lib")

namespace Havtorn
{
	CDirectXFramework::CDirectXFramework()
	{
		SwapChain = nullptr;
		Device = nullptr;
		Context = nullptr;
	}

	CDirectXFramework::~CDirectXFramework()
	{
		BOOL isFullscreen;
		SwapChain->GetFullscreenState(&isFullscreen, nullptr);

		// if isFullscreen
		if (isFullscreen <= 0)
		{
			SwapChain->SetFullscreenState(FALSE, NULL);
		}
	}

	void CDirectXFramework::EndFrame()
	{
		SwapChain->Present(0, 0);
	}

	bool CDirectXFramework::Init(Havtorn::CWindowHandler* windowHandler)
	{
		if (!windowHandler)
		{
			return false;
		}

		D3D11_CREATE_DEVICE_FLAG createFlag = static_cast<D3D11_CREATE_DEVICE_FLAG>(0);
#if _DEBUG
		createFlag = D3D11_CREATE_DEVICE_DEBUG;
#endif

		DXGI_SWAP_CHAIN_DESC swapchainDesc = {};
		swapchainDesc.BufferCount = 1;
		swapchainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		swapchainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		swapchainDesc.OutputWindow = windowHandler->GetWindowHandle();
		swapchainDesc.SampleDesc.Count = 1;
		swapchainDesc.Windowed = true;
		ENGINE_HR_MESSAGE(D3D11CreateDeviceAndSwapChain(
			nullptr,
			D3D_DRIVER_TYPE_HARDWARE,
			nullptr,
			createFlag,
			nullptr,
			0,
			D3D11_SDK_VERSION,
			&swapchainDesc,
			&SwapChain,
			&Device,
			nullptr,
			&Context), "Failed to created Device and Swap Chain.");

		return true;
	}

	void CDirectXFramework::ToggleFullscreenState(bool setFullscreen)
	{
		SwapChain->SetFullscreenState(setFullscreen, NULL);
	}

	ID3D11Device* CDirectXFramework::GetDevice() const
	{
		return Device.Get();
	}

	ID3D11DeviceContext* CDirectXFramework::GetContext() const
	{
		return Context.Get();
	}

	ID3D11Texture2D* CDirectXFramework::GetBackbufferTexture() const
	{
		ID3D11Texture2D* backbufferTexture = nullptr;
		ENGINE_HR_MESSAGE(SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&backbufferTexture), "Failed to Get Buffer");
		return backbufferTexture;
	}
}
