// Copyright 2022 Team Havtorn. All Rights Reserved.

#pragma once
#include "Editor/EditorWindow.h"

namespace Havtorn
{
	class CScene;
}

namespace ImGui
{
	class CHierarchyWindow : public CWindow
	{
	public:
		CHierarchyWindow(const char* name, Havtorn::CScene* scene, Havtorn::CEditorManager* manager);
		~CHierarchyWindow() override;
		void OnEnable() override;
		void OnInspectorGUI() override;
		void OnDisable() override;

	private:
		Havtorn::CScene* Scene = nullptr;

		Havtorn::I32 SelectedIndex = 0;
	};
}
