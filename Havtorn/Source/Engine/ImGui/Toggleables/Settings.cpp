#include "hvpch.h"
#include "Settings.h"
#include <imgui.h>
#include "Imgui/ImguiManager.h"

namespace ImGui
{
	CSettings::CSettings(const char* aName)
		: CToggleable(aName)
	{
	}

	CSettings::~CSettings()
	{
	}

	void CSettings::OnEnable()
	{
	}

	void CSettings::OnInspectorGUI()
	{
		if (ImGui::Button(Name()))
			ImGui::OpenPopup("my_file_popup");
		
        if (ImGui::BeginPopup("my_file_popup"))
		{
            ImGui::MenuItem("(demo menu)", NULL, false, false);
            if (ImGui::MenuItem("New")) {}
            if (ImGui::MenuItem("Open", "Ctrl+O")) {}
            if (ImGui::BeginMenu("Open Recent"))
            {
                ImGui::MenuItem("fish_hat.c");
                ImGui::MenuItem("fish_hat.inl");
                ImGui::MenuItem("fish_hat.h");
                if (ImGui::BeginMenu("More.."))
                {
                    ImGui::MenuItem("Hello");
                    ImGui::MenuItem("Sailor");
                    //if (ImGui::BeginMenu("Recurse.."))
                    //{
                    //}
                    ImGui::EndMenu();
                }
                ImGui::EndMenu();
            }
            if (ImGui::MenuItem("Save", "Ctrl+S")) {}
            if (ImGui::MenuItem("Save As..")) {}

            ImGui::Separator();
            if (ImGui::BeginMenu("Options"))
            {
                static bool enabled = true;
                ImGui::MenuItem("Enabled", "", &enabled);
                ImGui::BeginChild("child", ImVec2(0, 60), true);
                for (int i = 0; i < 10; i++)
                    ImGui::Text("Scrolling Text %d", i);
                ImGui::EndChild();
                static float f = 0.5f;
                static int n = 0;
                ImGui::SliderFloat("Value", &f, 0.0f, 1.0f);
                ImGui::InputFloat("Input", &f, 0.1f);
                ImGui::Combo("Combo", &n, "Yes\0No\0Maybe\0\0");
                ImGui::EndMenu();
            }

            if (ImGui::BeginMenu("Editor Themes"))
            {
                Havtorn::F32 sz = ImGui::GetTextLineHeight();
                for (Havtorn::U16 i = 0; i < static_cast<Havtorn::U16>(Havtorn::EEditorColorTheme::Count); i++)
                {
                    auto colorTheme = static_cast<Havtorn::EEditorColorTheme>(i);
                    std::string name = Havtorn::CImguiManager::GetEditorColorThemeName(colorTheme).c_str();
                    ImVec2 p = ImGui::GetCursorScreenPos();
                    auto previewColor = Havtorn::CImguiManager::GetEditorColorThemeRepColor(colorTheme);
                    ImGui::GetWindowDrawList()->AddRectFilled(p, ImVec2(p.x + sz, p.y + sz), ImGui::ColorConvertFloat4ToU32(previewColor));
                    ImGui::Dummy(ImVec2(sz, sz));
                    ImGui::SameLine();
                    if (ImGui::MenuItem(name.c_str()))
                    {
                        Havtorn::CImguiManager::SetEditorTheme(static_cast<Havtorn::EEditorColorTheme>(i));
                    }
                }
                ImGui::EndMenu();
            }

            // Here we demonstrate appending again to the "Options" menu (which we already created above)
            // Of course in this demo it is a little bit silly that this function calls BeginMenu("Options") twice.
            // In a real code-base using it would make senses to use this feature from very different code locations.
            if (ImGui::BeginMenu("Options")) // <-- Append!
            {
                //IMGUI_DEMO_MARKER("Examples/Menu/Append to an existing menu");
                static bool b = true;
                ImGui::Checkbox("SomeOption", &b);
                ImGui::EndMenu();
            }

            if (ImGui::BeginMenu("Disabled", false)) // Disabled
            {
                IM_ASSERT(0);
            }
            if (ImGui::MenuItem("Checked", NULL, true)) {}
            if (ImGui::MenuItem("Quit", "Alt+F4")) {}



			ImGui::EndPopup();
		}
	}

	void CSettings::OnDisable()
	{
	}
}