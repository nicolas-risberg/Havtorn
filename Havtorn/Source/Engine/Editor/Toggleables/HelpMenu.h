// Copyright 2022 Team Havtorn. All Rights Reserved.

#pragma once
#include "Editor/EditorToggleable.h"

namespace ImGui
{
	class CHelpMenu : public CToggleable
	{
	public:
		CHelpMenu(const char* aName, Havtorn::CEditorManager* manager);
		~CHelpMenu() override;
		void OnEnable() override;
		void OnInspectorGUI() override;
		void OnDisable() override;

	private:

	};
}
