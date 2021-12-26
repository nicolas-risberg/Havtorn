#include "hvpch.h"
#include "ViewportWindow.h"
#include <imgui.h>
#include "Imgui/ImguiManager.h"

namespace ImGui
{
	CViewportWindow::CViewportWindow(const char* aName, Havtorn::CImguiManager* manager)
		: CWindow(aName, manager)
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

		if (ImGui::Begin(Name(), nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse))
		{
		}
		ImGui::End();
	}

	void CViewportWindow::OnDisable()
	{
	}
}