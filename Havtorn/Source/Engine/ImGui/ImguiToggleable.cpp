#include "hvpch.h"
#include "ImguiToggleable.h"
#include "ImGui/Core/imgui.h"
#include "ImguiManager.h"

ImGui::CToggleable::CToggleable(const char* displayName, Havtorn::CImguiManager* manager)
	: DisplayName(displayName)
	, Manager(manager)
	, IsEnabled(false)
{
}
