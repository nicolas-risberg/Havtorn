// Copyright 2022 Team Havtorn. All Rights Reserved.

#include "hvpch.h"
#include "ImguiToggleable.h"
#include "ImGui/Core/imgui.h"
#include "ImguiManager.h"

ImGui::CToggleable::CToggleable(const char* displayName, Havtorn::CImguiManager* manager)
	: Manager(manager)
	, DisplayName(displayName)
	, IsEnabled(false)
{
}
