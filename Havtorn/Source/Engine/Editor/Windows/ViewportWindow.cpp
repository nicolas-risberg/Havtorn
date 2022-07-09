// Copyright 2022 Team Havtorn. All Rights Reserved.

#include "hvpch.h"
#include "ViewportWindow.h"
#include <imgui.h>
#include "Editor/EditorManager.h"
#include "Editor/EditorResourceManager.h"
#include "Graphics/RenderManager.h"
#include "Graphics/FullscreenTexture.h"

namespace ImGui
{
	CViewportWindow::CViewportWindow(const char* name, Havtorn::CEditorManager* manager)
		: CWindow(name, manager)
		, RenderedSceneTextureReference(nullptr)
	{
	}

	CViewportWindow::~CViewportWindow()
	{
	}

	void CViewportWindow::OnEnable()
	{
	}

	void CViewportWindow::OnInspectorGUI()
	{
		const Havtorn::SEditorLayout& layout = Manager->GetEditorLayout();

		const ImGuiViewport* mainViewport = ImGui::GetMainViewport();
		ImGui::SetNextWindowPos(ImVec2(mainViewport->WorkPos.x + layout.ViewportPosition.X, mainViewport->WorkPos.y + layout.ViewportPosition.Y));
		ImGui::SetNextWindowSize(ImVec2(layout.ViewportSize.X, layout.ViewportSize.Y));
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0.0f, 0.0f));

		ImTextureID playButtonID = Manager->GetResourceManager()->GetEditorTexture(Havtorn::EEditorTexture::PlayIcon);
		ImTextureID pauseButtonID = Manager->GetResourceManager()->GetEditorTexture(Havtorn::EEditorTexture::PauseIcon);
		ImTextureID stopButtonID = Manager->GetResourceManager()->GetEditorTexture(Havtorn::EEditorTexture::StopIcon);

		if (ImGui::Begin(Name(), nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoBringToFrontOnFocus))
		{
			ImGui::SameLine(layout.ViewportSize.X * 0.5f - 8.0f - 32.0f);
			if (ImGui::ImageButton(playButtonID, { 16.0f, 16.0f }))
			{

			}

			ImGui::SameLine(layout.ViewportSize.X * 0.5f - 8.0f);
			if (ImGui::ImageButton(pauseButtonID, { 16.0f, 16.0f }))
			{

			}

			ImGui::SameLine(layout.ViewportSize.X * 0.5f - 8.0f + 32.0f);
			if (ImGui::ImageButton(stopButtonID, { 16.0f, 16.0f }))
			{

			}

			RenderedSceneTextureReference = &(Manager->GetRenderManager()->GetRenderedSceneTexture());
			
			if (RenderedSceneTextureReference)
			{
				ImVec2 vMin = ImGui::GetWindowContentRegionMin();
				ImVec2 vMax = ImGui::GetWindowContentRegionMax();

				Havtorn::F32 width = static_cast<Havtorn::F32>(vMax.x - vMin.x);
				Havtorn::F32 height = static_cast<Havtorn::F32>(vMax.y - vMin.y - ViewportMenuHeight - 4.0f);
				ImGui::Image((void*)RenderedSceneTextureReference->GetShaderResourceView(), ImVec2(width, height));
			}
		}

		ImGui::PopStyleVar();
		ImGui::PopStyleVar();
		ImGui::End();
	}

	void CViewportWindow::OnDisable()
	{
	}
}