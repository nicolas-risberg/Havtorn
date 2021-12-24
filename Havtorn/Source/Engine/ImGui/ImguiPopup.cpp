#include "hvpch.h"
#include "ImguiPopup.h"
#include "ImGui/Core/imgui.h"

ImGui::CPopup::CPopup(const char* aWindowName)
	: myName(aWindowName)
	, myIsEnabled(false)
{
}
