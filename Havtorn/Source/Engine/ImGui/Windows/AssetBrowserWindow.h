// Copyright 2022 Team Havtorn. All Rights Reserved.

#pragma once
#include "Imgui/ImguiWindow.h"

namespace ImGui
{
	class CAssetBrowserWindow : public CWindow
	{
	public:
		CAssetBrowserWindow(const char* name, Havtorn::CImguiManager* manager);
		~CAssetBrowserWindow() override;
		void OnEnable() override;
		void OnInspectorGUI() override;
		void OnDisable() override;

	private:

	};
}
