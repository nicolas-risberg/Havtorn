// Copyright 2022 Team Havtorn. All Rights Reserved.

#include "hvpch.h"
#include "AssetBrowserWindow.h"
#include <imgui.h>
#include "Imgui/ImguiManager.h"

namespace ImGui
{
	CAssetBrowserWindow::CAssetBrowserWindow(const char* aName, Havtorn::CImguiManager* manager)
		: CWindow(aName, manager)
	{
	}

	CAssetBrowserWindow::~CAssetBrowserWindow()
	{
	}

	void CAssetBrowserWindow::OnEnable()
	{
	}

	void CAssetBrowserWindow::OnInspectorGUI()
	{
		const Havtorn::SEditorLayout& layout = Manager->GetEditorLayout();

		const ImGuiViewport* mainViewport = ImGui::GetMainViewport();
		ImGui::SetNextWindowPos(ImVec2(mainViewport->WorkPos.x + layout.AssetBrowserPosition.X, mainViewport->WorkPos.y + layout.AssetBrowserPosition.Y));
		ImGui::SetNextWindowSize(ImVec2(layout.AssetBrowserSize.X, layout.AssetBrowserSize.Y));

		if (ImGui::Begin(Name(), nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoBringToFrontOnFocus))
		{
		}
		ImGui::End();
	}

	void CAssetBrowserWindow::OnDisable()
	{
	}
}