#pragma once
#include "Imgui/ImguiToggleable.h"

namespace ImGui
{
	class CHelpMenu : public CToggleable
	{
	public:
		CHelpMenu(const char* aName);
		~CHelpMenu() override;
		void OnEnable() override;
		void OnInspectorGUI() override;
		void OnDisable() override;

	private:

	};
}
