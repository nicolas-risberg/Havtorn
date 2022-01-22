// Copyright 2022 Team Havtorn. All Rights Reserved.

#pragma once
#include "Imgui/ImguiToggleable.h"

namespace ImGui
{
	class CFileMenu : public CToggleable
	{
	public:
		CFileMenu(const char* aName, Havtorn::CImguiManager* manager);
		~CFileMenu() override;
		void OnEnable() override;
		void OnInspectorGUI() override;
		void OnDisable() override;

	private:

	};
}