// Copyright 2022 Team Havtorn. All Rights Reserved.

#include "hvpch.h"
#include "ImguiWindow.h"
#include "ImGui/Core/imgui.h"
#include "ImguiManager.h"

ImGui::CWindow::CWindow(const char* displayName, Havtorn::CImguiManager* manager)
	: DisplayName(displayName)
	, Manager(manager)
	, IsEnabled(false)
{
}
