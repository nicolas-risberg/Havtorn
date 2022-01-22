// Copyright 2022 Team Havtorn. All Rights Reserved.

#pragma once
#include "Imgui/ImguiWindow.h"


namespace Havtorn {
	class CScene;
}

namespace ImGui
{
	class CHierarchyWindow : public CWindow
	{
	public:
		CHierarchyWindow(const char* aName, Havtorn::CScene* scene, Havtorn::CImguiManager* manager);
		~CHierarchyWindow() override;
		void OnEnable() override;
		void OnInspectorGUI() override;
		void OnDisable() override;

	private:
		Havtorn::CScene* Scene;
		int SelectedIndex;
	};
}
