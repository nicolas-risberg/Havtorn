// Copyright 2022 Team Havtorn. All Rights Reserved.

#pragma once
#include "Imgui/ImguiWindow.h"

struct ID3D11Texture2D;

namespace Havtorn
{
	class CFullscreenTexture;
}

namespace ImGui
{
	class CViewportWindow : public CWindow
	{
	public:
		CViewportWindow(const char* aName, Havtorn::CImguiManager* manager);
		~CViewportWindow() override;
		void OnEnable() override;
		void OnInspectorGUI() override;
		void OnDisable() override;

	private:
		const Havtorn::CFullscreenTexture* RenderedSceneTextureReference;
	};
}
