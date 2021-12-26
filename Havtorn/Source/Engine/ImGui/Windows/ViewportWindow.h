#pragma once
#include "Imgui/ImguiWindow.h"

namespace ImGui
{
	class CViewportWindow : public CWindow
	{
	public:
		CViewportWindow(const char* aName, Havtorn::CImguiManager* manager);
		~CViewportWindow() override;
		void OnEnable() override;
		void OnInspectorGUI() override;
		void OnDisable() override;

	private:

	};
}
