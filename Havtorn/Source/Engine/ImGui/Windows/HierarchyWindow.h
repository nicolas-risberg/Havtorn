#pragma once
#include "Imgui/ImguiWindow.h"

namespace ImGui
{
	class CHierarchyWindow : public CWindow
	{
	public:
		CHierarchyWindow(const char* aName, Havtorn::CImguiManager* manager);
		~CHierarchyWindow() override;
		void OnEnable() override;
		void OnInspectorGUI() override;
		void OnDisable() override;

	private:

	};
}
