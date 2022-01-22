// Copyright 2022 Team Havtorn. All Rights Reserved.

#include "hvpch.h"
#include "HelpMenu.h"
#include <imgui.h>
#include "Imgui/ImguiManager.h"

namespace ImGui
{
	CHelpMenu::CHelpMenu(const char* aName, Havtorn::CImguiManager* manager)
		: CToggleable(aName, manager)
	{
	}

	CHelpMenu::~CHelpMenu()
	{
	}

	void CHelpMenu::OnEnable()
	{
	}

	void CHelpMenu::OnInspectorGUI()
	{
		if (ImGui::Button(Name()))
		{

		}
	}

	void CHelpMenu::OnDisable()
	{
	}
}
