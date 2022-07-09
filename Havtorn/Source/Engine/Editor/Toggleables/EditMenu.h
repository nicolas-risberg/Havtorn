// Copyright 2022 Team Havtorn. All Rights Reserved.

#pragma once
#include "Editor/EditorToggleable.h"

namespace ImGui
{
	class CEditMenu : public CToggleable
	{
	public:
		CEditMenu(const char* aName, Havtorn::CEditorManager* manager);
		~CEditMenu() override;
		void OnEnable() override;
		void OnInspectorGUI() override;
		void OnDisable() override;

	private:

	};
}
