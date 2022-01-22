// Copyright 2022 Team Havtorn. All Rights Reserved.

#include "hvpch.h"
#include "InspectorWindow.h"
#include <imgui.h>
#include "Imgui/ImguiManager.h"

namespace ImGui
{
	CInspectorWindow::CInspectorWindow(const char* aName, Havtorn::CImguiManager* manager)
		: CWindow(aName, manager)
	{
	}

	CInspectorWindow::~CInspectorWindow()
	{
	}

	void CInspectorWindow::OnEnable()
	{
	}

	void CInspectorWindow::OnInspectorGUI()
	{
		const Havtorn::SEditorLayout& layout = Manager->GetEditorLayout();

		const ImGuiViewport* mainViewport = ImGui::GetMainViewport();
		ImGui::SetNextWindowPos(ImVec2(mainViewport->WorkPos.x + layout.InspectorPosition.X, mainViewport->WorkPos.y + layout.InspectorPosition.Y));
		ImGui::SetNextWindowSize(ImVec2(layout.InspectorSize.X, layout.InspectorSize.Y));

		if (ImGui::Begin(Name(), nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoBringToFrontOnFocus))
		{
		}
		ImGui::End();
	}

	void CInspectorWindow::OnDisable()
	{
	}
}