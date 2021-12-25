#include "hvpch.h"
#include "ImguiToggleable.h"
#include "ImGui/Core/imgui.h"

ImGui::CToggleable::CToggleable(const char* displayName)
	: DisplayName(displayName)
	, IsEnabled(false)
{
}
