#pragma once
#include "Imgui/ImguiToggleable.h"

namespace ImGui
{
	class CFileMenu : public CToggleable
	{
	public:
		CFileMenu(const char* aName);
		~CFileMenu() override;
		void OnEnable() override;
		void OnInspectorGUI() override;
		void OnDisable() override;

	private:

	};
}