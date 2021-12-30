#include "hvpch.h"
#include "HierarchyWindow.h"
#include <imgui.h>
#include "Imgui/ImguiManager.h"

namespace ImGui
{
	CHierarchyWindow::CHierarchyWindow(const char* aName, Havtorn::CImguiManager* manager)
		: CWindow(aName, manager)
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
		}
		ImGui::End();
	}

	void CHierarchyWindow::OnDisable()
	{
	}
}