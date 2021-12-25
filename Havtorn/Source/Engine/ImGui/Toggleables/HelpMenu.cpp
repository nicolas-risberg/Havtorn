#include "hvpch.h"
#include "HelpMenu.h"
#include <imgui.h>
#include "Imgui/ImguiManager.h"

namespace ImGui
{
	CHelpMenu::CHelpMenu(const char* aName)
		: CToggleable(aName)
	{
	}

	CHelpMenu::~CHelpMenu()
	{
	}

	void CHelpMenu::OnEnable()
	{
	}

	void CHelpMenu::OnInspectorGUI()
	{
		if (ImGui::Button(Name()))
		{

		}
	}

	void CHelpMenu::OnDisable()
	{
	}
}
