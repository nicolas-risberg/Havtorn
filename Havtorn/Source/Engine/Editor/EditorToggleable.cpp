// Copyright 2022 Team Havtorn. All Rights Reserved.

#include "hvpch.h"
#include "EditorToggleable.h"
#include "Editor/Core/imgui.h"
#include "EditorManager.h"

ImGui::CToggleable::CToggleable(const char* displayName, Havtorn::CEditorManager* manager)
	: Manager(manager)
	, DisplayName(displayName)
	, IsEnabled(false)
{
}
