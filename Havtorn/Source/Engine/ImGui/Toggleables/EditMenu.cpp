// Copyright 2022 Team Havtorn. All Rights Reserved.

#include "hvpch.h"
#include "EditMenu.h"
#include <imgui.h>
#include "Imgui/ImguiManager.h"

namespace ImGui
{
	CEditMenu::CEditMenu(const char* aName, Havtorn::CImguiManager* manager)
		: CToggleable(aName, manager)
	{
	}

	CEditMenu::~CEditMenu()
	{
	}

	void CEditMenu::OnEnable()
	{
	}

	void CEditMenu::OnInspectorGUI()
	{
		if (ImGui::Button(Name()))
			ImGui::OpenPopup("my_edit_popup");
		
        if (ImGui::BeginPopup("my_edit_popup"))
		{
            if (ImGui::BeginMenu("Editor Themes"))
            {
                Havtorn::F32 sz = ImGui::GetTextLineHeight();
                for (Havtorn::U16 i = 0; i < static_cast<Havtorn::U16>(Havtorn::EEditorColorTheme::Count); i++)
                {
                    auto colorTheme = static_cast<Havtorn::EEditorColorTheme>(i);
                    std::string name = Manager->GetEditorColorThemeName(colorTheme).c_str();
                    ImVec2 p = ImGui::GetCursorScreenPos();
                    auto previewColor = Manager->GetEditorColorThemeRepColor(colorTheme);
                    ImGui::GetWindowDrawList()->AddRectFilled(p, ImVec2(p.x + sz, p.y + sz), ImGui::ColorConvertFloat4ToU32(previewColor));
                    ImGui::Dummy(ImVec2(sz, sz));
                    ImGui::SameLine();
                    if (ImGui::MenuItem(name.c_str()))
                    {
                        Manager->SetEditorTheme(static_cast<Havtorn::EEditorColorTheme>(i));
                    }
                }
                ImGui::EndMenu();
            }

			ImGui::EndPopup();
		}
	}

	void CEditMenu::OnDisable()
	{
	}
}