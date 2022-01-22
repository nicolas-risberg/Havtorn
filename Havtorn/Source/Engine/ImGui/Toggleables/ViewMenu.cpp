// Copyright 2022 Team Havtorn. All Rights Reserved.

#include "hvpch.h"
#include "ViewMenu.h"
#include <imgui.h>
#include "Imgui/ImguiManager.h"

namespace ImGui
{
	CViewMenu::CViewMenu(const char* aName, Havtorn::CImguiManager* manager)
		: CToggleable(aName, manager)
	{
	}

	void CViewMenu::OnEnable()
	{
	}

	void CViewMenu::OnInspectorGUI()
	{
		if (ImGui::Button(Name()))
			ImGui::OpenPopup("my_view_popup");

		if (ImGui::BeginPopup("my_view_popup"))
		{
			if(ImGui::MenuItem("Debug info"))
			{
				Manager->ToggleDebugInfo();
			}

			if(ImGui::MenuItem("Dear Imgui Demo"))
			{
				Manager->ToggleDemo();
			}

			ImGui::EndPopup();
		}
	}

	void CViewMenu::OnDisable()
	{
	}
}