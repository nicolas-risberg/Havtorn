#pragma once
#include "Imgui/ImguiPopup.h"

namespace ImGui
{
	class CSettings : public CPopup
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

