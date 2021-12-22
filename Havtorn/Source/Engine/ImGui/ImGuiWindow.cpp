#include "hvpch.h"
#include "ImguiWindow.h"
#include "ImGui/Core/imgui.h"

ImGui::CWindow::CWindow(const char* aWindowName, const bool aIsMainMenuBarChild)
	: myName(aWindowName)
	, myIsEnabled(false)
	, myIsMainMenuBarChild(aIsMainMenuBarChild)
{
}

bool ImGui::CWindow::OnMainMenuGUI()
{
	if (::ImGui::Button(Name()))
	{
		if (!Enable())
			OnEnable();
		else
			OnDisable();
		Enable(!Enable());
	}

	return false;
}
