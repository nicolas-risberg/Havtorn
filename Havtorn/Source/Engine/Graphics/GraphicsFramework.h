// Copyright 2022 Team Havtorn. All Rights Reserved.

#pragma once
#include "Core/WindowsInclude.h"

struct IDXGISwapChain;
struct ID3D11Device;
struct ID3D11DeviceContext;
struct ID3D11RenderTargetView;
struct ID3D11DepthStencilView;
struct ID3D11Texture2D;

namespace Havtorn
{
	class CWindowHandler;

	class CDirectXFramework
	{
	public:
		CDirectXFramework();
		~CDirectXFramework();

		void EndFrame();

		bool Init(CWindowHandler* windowHandler);

		void ToggleFullscreenState(bool setFullscreen);

		ID3D11Device* GetDevice() const;
		ID3D11DeviceContext* GetContext() const;
		ID3D11Texture2D* GetBackbufferTexture() const;

	private:
		WinComPtr<IDXGISwapChain> SwapChain;
		WinComPtr<ID3D11Device> Device;
		WinComPtr<ID3D11DeviceContext> Context;
	};
}
