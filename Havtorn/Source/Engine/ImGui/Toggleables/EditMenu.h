#pragma once
#include "Imgui/ImguiToggleable.h"

namespace ImGui
{
	class CEditMenu : public CToggleable
	{
	public:
		CEditMenu(const char* aName);
		~CEditMenu() override;
		void OnEnable() override;
		void OnInspectorGUI() override;
		void OnDisable() override;

	private:

	};
}
