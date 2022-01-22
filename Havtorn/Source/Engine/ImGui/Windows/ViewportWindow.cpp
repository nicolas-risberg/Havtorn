// Copyright 2022 Team Havtorn. All Rights Reserved.

#include "hvpch.h"
#include "ViewportWindow.h"
#include <imgui.h>
#include "Imgui/ImguiManager.h"
#include "Graphics/RenderManager.h"
#include "Graphics/FullscreenTexture.h"

namespace ImGui
{
	CViewportWindow::CViewportWindow(const char* aName, Havtorn::CImguiManager* manager)
		: CWindow(aName, manager)
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

		if (ImGui::Begin(Name(), nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoBringToFrontOnFocus))
		{
			RenderedSceneTextureReference = &(Manager->GetRenderManager()->GetRenderedSceneTexture());
			
			if (RenderedSceneTextureReference)
			{
				ImVec2 vMin = ImGui::GetWindowContentRegionMin();
				ImVec2 vMax = ImGui::GetWindowContentRegionMax();

				Havtorn::F32 width = static_cast<Havtorn::F32>(vMax.x - vMin.x);
				Havtorn::F32 height = static_cast<Havtorn::F32>(vMax.y - vMin.y);
				ImGui::Image((void*)RenderedSceneTextureReference->GetShaderResourceView(), ImVec2(width, height));
			}
		}

		ImGui::PopStyleVar();
		ImGui::End();
	}

	void CViewportWindow::OnDisable()
	{
	}
}