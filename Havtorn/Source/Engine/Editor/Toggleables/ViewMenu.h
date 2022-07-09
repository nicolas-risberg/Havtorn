// Copyright 2022 Team Havtorn. All Rights Reserved.

#pragma once
#include "Editor/EditorToggleable.h"

namespace ImGui
{
	class CViewMenu final : public CToggleable
	{
	public:
		CViewMenu(const char* aName, Havtorn::CEditorManager* manager);
		~CViewMenu() override = default;
		void OnEnable() override;
		void OnInspectorGUI() override;
		void OnDisable() override;

	private:

	};
}
