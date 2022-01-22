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

#if _DEBUG
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
			0, //D3D11_CREATE_DEVICE_DEBUG ,
			nullptr,
			0,
			D3D11_SDK_VERSION,
			&swapchainDesc,
			&SwapChain,
			&Device,
			nullptr,
			&Context), "Failed to created Device and Swap Chain.");
#else
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
			0,
			nullptr,
			0,
			D3D11_SDK_VERSION,
			&swapchainDesc,
			&SwapChain,
			&Device,
			nullptr,
			&Context), "Failed to created Device and Swap Chain.");
#endif

		return true;
	}

	void CDirectXFramework::ToggleFullscreenState(bool setFullscreen)
	{
		SwapChain->SetFullscreenState(setFullscreen, NULL);
	}

	bool CDirectXFramework::ResizeBackBufferTexture()
	{
		//HRESULT result;
		//ID3D11Texture2D* backbufferTexture;
		//result = SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&backbufferTexture);
		//if (FAILED(result))
		//{
		//	return false;
		//}
		//return true;

		//ENGINE_HR_BOOL_MESSAGE(SwapChain->ResizeBuffers(0, 0, 0, DXGI_FORMAT_UNKNOWN, 0), "Could not resize buffers!");

		//ID3D11Texture2D* pBuffer = nullptr;
		//ENGINE_HR_BOOL_MESSAGE(SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&pBuffer), "Could not resize buffers!");

		//if (!pBuffer)
		//	return false;

		//ENGINE_HR_BOOL_MESSAGE(Device->CreateRenderTargetView(pBuffer, NULL, &BackBuffer), "Could not resize buffers!");

		//pBuffer->Release();
		return true;
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
		ID3D11Texture2D* backbufferTexture;
		ENGINE_HR_MESSAGE(SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&backbufferTexture), "Failed to Get Buffer");
		return backbufferTexture;
	}
}
