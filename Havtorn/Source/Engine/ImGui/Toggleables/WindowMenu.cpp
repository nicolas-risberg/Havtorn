// Copyright 2022 Team Havtorn. All Rights Reserved.

#include "hvpch.h"
#include "WindowMenu.h"
#include <imgui.h>
#include "Imgui/ImguiManager.h"

namespace ImGui
{
	CWindowMenu::CWindowMenu(const char* aName, Havtorn::CImguiManager* manager)
		: CToggleable(aName, manager)
	{
	}

	void CWindowMenu::OnEnable()
	{
	}

	void CWindowMenu::OnInspectorGUI()
	{
		if (ImGui::Button(Name()))
			ImGui::OpenPopup("window_popup");
		
		if (ImGui::BeginPopup("window_popup"))
		{
			Havtorn::F32 viewportPadding = Manager->GetViewportPadding();
			if (ImGui::DragFloat("Viewport Padding", &viewportPadding, 0.01f, 0.0f, 0.5f))
			{
				Manager->SetViewportPadding(viewportPadding);
			}

			ImGui::EndPopup();
		}
	}

	void CWindowMenu::OnDisable()
	{
	}
}
