#pragma once
#include "Imgui/ImguiToggleable.h"

namespace ImGui
{
	class CSettings : public CToggleable
	{
	public:
		CSettings(const char* aName);
		~CSettings() override;
		void OnEnable() override;
		void OnInspectorGUI() override;
		void OnDisable() override;

	private:

	};
}

