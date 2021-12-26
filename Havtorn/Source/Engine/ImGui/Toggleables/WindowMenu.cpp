#include "hvpch.h"
#include "WindowMenu.h"
#include <imgui.h>
#include "Imgui/ImguiManager.h"

namespace ImGui
{
	CWindowMenu::CWindowMenu(const char* aName, Havtorn::CImguiManager* manager)
		: CToggleable(aName, manager)
	{
	}

	CWindowMenu::~CWindowMenu()
	{
	}

	void CWindowMenu::OnEnable()
	{
	}

	void CWindowMenu::OnInspectorGUI()
	{
		if (ImGui::Button(Name()))
		{

		}
	}

	void CWindowMenu::OnDisable()
	{
	}
}
