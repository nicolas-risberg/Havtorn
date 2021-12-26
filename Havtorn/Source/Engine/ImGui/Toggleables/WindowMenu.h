#pragma once
#include "Imgui/ImguiToggleable.h"

namespace ImGui
{
	class CWindowMenu : public CToggleable
	{
	public:
		CWindowMenu(const char* aName, Havtorn::CImguiManager* manager);
		~CWindowMenu() override;
		void OnEnable() override;
		void OnInspectorGUI() override;
		void OnDisable() override;

	private:

	};
}
