// Copyright 2022 Team Havtorn. All Rights Reserved.

#include "hvpch.h"
#include "EditorWindow.h"
#include "Editor/Core/imgui.h"
#include "EditorManager.h"

ImGui::CWindow::CWindow(const char* displayName, Havtorn::CEditorManager* manager)
	: DisplayName(displayName)
	, Manager(manager)
	, IsEnabled(false)
{
}
