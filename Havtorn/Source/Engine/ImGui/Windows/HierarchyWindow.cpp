// Copyright 2022 Team Havtorn. All Rights Reserved.

#include "hvpch.h"
#include "HierarchyWindow.h"
#include <imgui.h>
#include "Imgui/ImguiManager.h"

#include "Scene/Scene.h"
#include "ECS/Entity.h"

namespace ImGui
{
	CHierarchyWindow::CHierarchyWindow(const char* name, Havtorn::CScene* scene, Havtorn::CImguiManager* manager)
		: CWindow(name, manager)
		, Scene(scene)
		, SelectedIndex(0)
	{
	}

	CHierarchyWindow::~CHierarchyWindow()
	{
	}

	void CHierarchyWindow::OnEnable()
	{
		
	}

	void CHierarchyWindow::OnInspectorGUI()
	{
		const Havtorn::SEditorLayout& layout = Manager->GetEditorLayout();

		const ImGuiViewport* mainViewport = ImGui::GetMainViewport();
		ImGui::SetNextWindowPos(ImVec2(mainViewport->WorkPos.x + layout.HierarchyViewPosition.X, mainViewport->WorkPos.y + layout.HierarchyViewPosition.Y));
		ImGui::SetNextWindowSize(ImVec2(layout.HierarchyViewSize.X, layout.HierarchyViewSize.Y));

		if (ImGui::Begin(Name(), nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoBringToFrontOnFocus))
		{
			Havtorn::I32 index = 0;
			auto& entities = Scene->GetEntities();
			
			for (auto& entity : entities) 
			{
				ImGui::PushID(static_cast<int>(entity->ID));
	
				if (ImGui::Selectable(entity->Name.c_str(), index == SelectedIndex, ImGuiSelectableFlags_None)) 
				{
					SelectedIndex = index;
				}

				ImGui::PopID();
			}
		}
		ImGui::End();
	}

	void CHierarchyWindow::OnDisable()
	{
	}
}