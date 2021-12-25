#include "hvpch.h"
#include "ImguiManager.h"

#include "Imgui/Core/imgui.h"
#include "Imgui/Core/imgui_impl_win32.h"
#include "Imgui/Core/imgui_impl_dx11.h"

#include <psapi.h>

//#include "Engine.h"
//#include "Scene.h"
//#include "SceneManager.h"
#include "Graphics/RenderManager.h"
//#include "JsonReader.h"
#include "ImguiWindows.h"
#include "ImguiToggleables.h"
//#include "PostMaster.h"


//#pragma comment(lib, "psapi.lib")

namespace Havtorn
{
	//static ImFont* ImGui_LoadFont(ImFontAtlas& atlas, const char* name, float size, const ImVec2 & /*displayOffset*/ = ImVec2(0, 0))
	//{
	//	char* windir = nullptr;
	//	if (_dupenv_s(&windir, nullptr, "WINDIR") || windir == nullptr)
	//		return nullptr;

	//	static const ImWchar ranges[] =
	//	{
	//		0x0020, 0x00FF, // Basic Latin + Latin Supplement
	//		0x0104, 0x017C, // Polish characters and more
	//		0,
	//	};

	//	ImFontConfig config;
	//	config.OversampleH = 4;
	//	config.OversampleV = 4;
	//	config.PixelSnapH = false;

	//	auto path = std::string(windir) + "\\Fonts\\" + name;
	//	auto font = atlas.AddFontFromFileTTF(path.c_str(), size, &config, ranges);
	//	//if (font)
	//	//font->DisplayOffset = displayOffset;

	//	free(windir);

	//	return font;
	//}
	ImFontAtlas myFontAtlas;

	CImguiManager::CImguiManager() : IsEnabled(true)
	{
	}

	CImguiManager::~CImguiManager()
	{
        ImGui_ImplWin32_Shutdown();
		ImGui_ImplDX11_Shutdown();
		ImGui::DestroyContext();
	}

	bool CImguiManager::Init(ID3D11Device* device, ID3D11DeviceContext* context, HWND windowHandle)
	{
		bool success = true;

		ImGui::DebugCheckVersionAndDataLayout("1.86 WIP", sizeof(ImGuiIO), sizeof(ImGuiStyle), sizeof(ImVec2), sizeof(ImVec4), sizeof(ImDrawVert), sizeof(unsigned int));
		ImGui::CreateContext();
		SetEditorTheme(EEditorColorTheme::HavtornDark, EEditorStyleTheme::Havtorn);
		//ImGui_LoadFont(myFontAtlas, "segoeui.ttf", 22.0f); // Doesn't really set the font
		myFontAtlas.Build();
		//ImGui::GetIO().Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\verdana.ttf", 14.0f);
		ImGui::GetIO().Fonts->AddFontFromFileTTF("../External/imgui/misc/fonts/Roboto-Medium.ttf", 15.0f);
		ImGui::CreateContext(&myFontAtlas);

		myPopups.emplace_back(std::make_unique<ImGui::CFileMenu>("File"));
		myPopups.emplace_back(std::make_unique<ImGui::CEditMenu>("Edit"));
		myPopups.emplace_back(std::make_unique<ImGui::CWindowMenu>("Window"));
		myPopups.emplace_back(std::make_unique<ImGui::CHelpMenu>("Help"));

		//myWindows.emplace_back(std::make_unique <ImGui::CLoadScene>("Load Scene", true));
		//myWindows.emplace_back(std::make_unique <ImGui::CCameraSetting>("Camera Settings"));
		//myWindows.emplace_back(std::make_unique <ImGui::CVFXEditorWindow>("VFX Editor"));
		//myWindows.emplace_back(std::make_unique <ImGui::CPlayerControlWindow>("Player"));
		//myWindows.emplace_back(std::make_unique <ImGui::CGravityGloveEditor>("GravityGlove"));
		//myWindows.emplace_back(std::make_unique <ImGui::CEnvironmentLightWindow>("Environment Light"));
		//myWindows.emplace_back(std::make_unique <ImGui::CPostProcessingWindow>("Post Processing"));
		//myWindows.emplace_back(std::make_unique <ImGui::CHierarchy>("Scene Hierarchy"));
		//myWindows.emplace_back(std::make_unique <ImGui::CDebugPrintoutWindow>("CMD"));

		//CMainSingleton::PostMaster().Subscribe(EMessageType::CursorHideAndLock, this);
		//CMainSingleton::PostMaster().Subscribe(EMessageType::CursorShowAndUnlock, this);

		success = ImGui_ImplWin32_Init(windowHandle);
		if (!success)
			return false;

		success = ImGui_ImplDX11_Init(device, context);
		if (!success)
			return false;

		return success;
	}

	void CImguiManager::BeginFrame()
	{
		ImGui_ImplDX11_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();
	}

	void CImguiManager::Update()
	{
		// Main Menu bar
		if (IsEnabled)
		{
			ImGui::BeginMainMenuBar();

			for (const auto& window : myWindows)
				window->OnMainMenuGUI();

			for (const auto& popup : myPopups)
				popup->OnInspectorGUI();

			ImGui::EndMainMenuBar();
		}

		// Windows
		for (const auto& window : myWindows) 
		{
			if (window->Enable() && !window->MainMenuBarChild()) 
			{
				window->OnInspectorGUI();
			}
		}

		DebugWindow();
	}

	void CImguiManager::EndFrame()
	{
		ImGui::Render();
		ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
	}

	void CImguiManager::DebugWindow()
	{
		ImGui::ShowDemoWindow();
		ImGui::ShowStyleEditor();

		if (ImGui::Begin("Debug info", nullptr))
		{
			ImGui::Text(GetFrameRate().c_str());
			ImGui::Text(GetSystemMemory().c_str());
			ImGui::Text(GetDrawCalls().c_str());
		}

		ImGui::End();
	}

	//void CImguiManager::Receive(const SMessage& aMessage)
	//{
	//	if (aMessage.myMessageType == EMessageType::CursorHideAndLock)
	//	{
	//		IsEnabled = false;
	//	}
	//	else if (aMessage.myMessageType == EMessageType::CursorShowAndUnlock)
	//	{
	//		IsEnabled = true;
	//	}
	//}

//void CImguiManager::LevelSelect()
//{
//	//std::vector<std::string> files = CJsonReader::GetFileNamesInFolder(ASSETPATH + "Assets/Generated");
//	ImGui::SetNextWindowPos(ImVec2(ImGui::GetIO().DisplaySize.x - 200, 18));
//	float x = 10.f + ((ImGui::GetFontSize() + 5.5f) * static_cast<float>(myLevelsToSelectFrom.size()));
//	ImGui::SetNextWindowSize({200.f,  x});
//
//	ImGui::Begin("LevelSelect", nullptr, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize);
//	/*if (ImGui::TreeNodeEx("Selection State: Single Selection", ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Leaf))
//	{*/
//		static int selected = -1;
//		for (int n = 0; n < myLevelsToSelectFrom.size(); n++)
//		{
//			char buf[512];
//			sprintf_s(buf, "%s", myLevelsToSelectFrom[n].c_str());
//
//			if (ImGui::Selectable(buf, selected == n, ImGuiSelectableFlags_AllowDoubleClick))
//			{
//				selected = n;
//
//				if (ImGui::IsMouseDoubleClicked(0))
//				{
//					std::cout << "Load Level: " << buf << std::endl;
//				
//
//					CScene* myUnityScene = CSceneManager::CreateScene(buf);
//					CEngine::GetInstance()->AddScene(CStateStack::EState::InGame, myUnityScene);
//					CEngine::GetInstance()->SetActiveScene(CStateStack::EState::InGame);
//					IsEnabled = !IsEnabled;
//				}
//			}
//		}
//	/*	ImGui::TreePop();
//	}*/
//
//
//	ImGui::End();
//}

	void CImguiManager::SetEditorTheme(EEditorColorTheme colorTheme, EEditorStyleTheme styleTheme)
	{
		ImGuiStyle* style = &ImGui::GetStyle();
		ImVec4* colors = style->Colors;

		switch (colorTheme)
		{
		case Havtorn::EEditorColorTheme::HavtornDark:

			colors[ImGuiCol_Text] = ImVec4(0.92f, 0.92f, 0.92f, 1.00f);
			colors[ImGuiCol_TextDisabled] = ImVec4(0.44f, 0.44f, 0.44f, 1.00f);
			colors[ImGuiCol_WindowBg] = ImVec4(0.06f, 0.06f, 0.06f, 1.00f);
			colors[ImGuiCol_ChildBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
			colors[ImGuiCol_PopupBg] = ImVec4(0.08f, 0.08f, 0.08f, 0.94f);
			colors[ImGuiCol_Border] = ImVec4(0.51f, 0.36f, 0.15f, 1.00f);
			colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
			colors[ImGuiCol_FrameBg] = ImVec4(0.11f, 0.11f, 0.11f, 1.00f);
			colors[ImGuiCol_FrameBgHovered] = ImVec4(0.51f, 0.36f, 0.15f, 1.00f);
			colors[ImGuiCol_FrameBgActive] = ImVec4(0.78f, 0.55f, 0.21f, 1.00f);
			colors[ImGuiCol_TitleBg] = ImVec4(0.51f, 0.36f, 0.15f, 1.00f);
			colors[ImGuiCol_TitleBgActive] = ImVec4(0.91f, 0.64f, 0.13f, 1.00f);
			colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.00f, 0.00f, 0.00f, 0.51f);
			colors[ImGuiCol_MenuBarBg] = ImVec4(0.11f, 0.11f, 0.11f, 1.00f);
			colors[ImGuiCol_ScrollbarBg] = ImVec4(0.06f, 0.06f, 0.06f, 0.53f);
			colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.21f, 0.21f, 0.21f, 1.00f);
			colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.47f, 0.47f, 0.47f, 1.00f);
			colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.81f, 0.83f, 0.81f, 1.00f);
			colors[ImGuiCol_CheckMark] = ImVec4(0.78f, 0.55f, 0.21f, 1.00f);
			colors[ImGuiCol_SliderGrab] = ImVec4(0.91f, 0.64f, 0.13f, 1.00f);
			colors[ImGuiCol_SliderGrabActive] = ImVec4(0.91f, 0.64f, 0.13f, 1.00f);
			colors[ImGuiCol_Button] = ImVec4(0.51f, 0.36f, 0.15f, 1.00f);
			colors[ImGuiCol_ButtonHovered] = ImVec4(0.91f, 0.64f, 0.13f, 1.00f);
			colors[ImGuiCol_ButtonActive] = ImVec4(0.78f, 0.55f, 0.21f, 1.00f);
			colors[ImGuiCol_Header] = ImVec4(0.51f, 0.36f, 0.15f, 1.00f);
			colors[ImGuiCol_HeaderHovered] = ImVec4(0.91f, 0.64f, 0.13f, 1.00f);
			colors[ImGuiCol_HeaderActive] = ImVec4(0.93f, 0.65f, 0.14f, 1.00f);
			colors[ImGuiCol_Separator] = ImVec4(0.21f, 0.21f, 0.21f, 1.00f);
			colors[ImGuiCol_SeparatorHovered] = ImVec4(0.91f, 0.64f, 0.13f, 1.00f);
			colors[ImGuiCol_SeparatorActive] = ImVec4(0.78f, 0.55f, 0.21f, 1.00f);
			colors[ImGuiCol_ResizeGrip] = ImVec4(0.21f, 0.21f, 0.21f, 1.00f);
			colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.91f, 0.64f, 0.13f, 1.00f);
			colors[ImGuiCol_ResizeGripActive] = ImVec4(0.78f, 0.55f, 0.21f, 1.00f);
			colors[ImGuiCol_Tab] = ImVec4(0.51f, 0.36f, 0.15f, 1.00f);
			colors[ImGuiCol_TabHovered] = ImVec4(0.91f, 0.64f, 0.13f, 1.00f);
			colors[ImGuiCol_TabActive] = ImVec4(0.78f, 0.55f, 0.21f, 1.00f);
			colors[ImGuiCol_TabUnfocused] = ImVec4(0.07f, 0.10f, 0.15f, 0.97f);
			colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.14f, 0.26f, 0.42f, 1.00f);
			colors[ImGuiCol_PlotLines] = ImVec4(0.61f, 0.61f, 0.61f, 1.00f);
			colors[ImGuiCol_PlotLinesHovered] = ImVec4(1.00f, 0.43f, 0.35f, 1.00f);
			colors[ImGuiCol_PlotHistogram] = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
			colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
			colors[ImGuiCol_TextSelectedBg] = ImVec4(0.26f, 0.59f, 0.98f, 0.35f);
			colors[ImGuiCol_DragDropTarget] = ImVec4(1.00f, 1.00f, 0.00f, 0.90f);
			colors[ImGuiCol_NavHighlight] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
			colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.00f, 1.00f, 1.00f, 0.70f);
			colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.20f);
			colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.35f);
			break;

		case Havtorn::EEditorColorTheme::HavtornRed:

			colors[ImGuiCol_Text] = ImVec4(0.75f, 0.75f, 0.75f, 1.00f);
			colors[ImGuiCol_TextDisabled] = ImVec4(0.35f, 0.35f, 0.35f, 1.00f);
			colors[ImGuiCol_WindowBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.94f);
			colors[ImGuiCol_ChildBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
			colors[ImGuiCol_PopupBg] = ImVec4(0.08f, 0.08f, 0.08f, 0.94f);
			colors[ImGuiCol_Border] = ImVec4(0.00f, 0.00f, 0.00f, 0.50f);
			colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
			colors[ImGuiCol_FrameBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.54f);
			colors[ImGuiCol_FrameBgHovered] = ImVec4(0.37f, 0.14f, 0.14f, 0.67f);
			colors[ImGuiCol_FrameBgActive] = ImVec4(0.39f, 0.20f, 0.20f, 0.67f);
			colors[ImGuiCol_TitleBg] = ImVec4(0.04f, 0.04f, 0.04f, 1.00f);
			colors[ImGuiCol_TitleBgActive] = ImVec4(0.48f, 0.16f, 0.16f, 1.00f);
			colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.48f, 0.16f, 0.16f, 1.00f);
			colors[ImGuiCol_MenuBarBg] = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
			colors[ImGuiCol_ScrollbarBg] = ImVec4(0.02f, 0.02f, 0.02f, 0.53f);
			colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.31f, 0.31f, 0.31f, 1.00f);
			colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.41f, 0.41f, 0.41f, 1.00f);
			colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.51f, 0.51f, 0.51f, 1.00f);
			colors[ImGuiCol_CheckMark] = ImVec4(0.56f, 0.10f, 0.10f, 1.00f);
			colors[ImGuiCol_SliderGrab] = ImVec4(1.00f, 0.19f, 0.19f, 0.40f);
			colors[ImGuiCol_SliderGrabActive] = ImVec4(0.89f, 0.00f, 0.19f, 1.00f);
			colors[ImGuiCol_Button] = ImVec4(1.00f, 0.19f, 0.19f, 0.40f);
			colors[ImGuiCol_ButtonHovered] = ImVec4(0.80f, 0.17f, 0.00f, 1.00f);
			colors[ImGuiCol_ButtonActive] = ImVec4(0.89f, 0.00f, 0.19f, 1.00f);
			colors[ImGuiCol_Header] = ImVec4(0.33f, 0.35f, 0.36f, 0.53f);
			colors[ImGuiCol_HeaderHovered] = ImVec4(0.76f, 0.28f, 0.44f, 0.67f);
			colors[ImGuiCol_HeaderActive] = ImVec4(0.47f, 0.47f, 0.47f, 0.67f);
			colors[ImGuiCol_Separator] = ImVec4(0.32f, 0.32f, 0.32f, 1.00f);
			colors[ImGuiCol_SeparatorHovered] = ImVec4(0.32f, 0.32f, 0.32f, 1.00f);
			colors[ImGuiCol_SeparatorActive] = ImVec4(0.32f, 0.32f, 0.32f, 1.00f);
			colors[ImGuiCol_ResizeGrip] = ImVec4(1.00f, 1.00f, 1.00f, 0.85f);
			colors[ImGuiCol_ResizeGripHovered] = ImVec4(1.00f, 1.00f, 1.00f, 0.60f);
			colors[ImGuiCol_ResizeGripActive] = ImVec4(1.00f, 1.00f, 1.00f, 0.90f);
			colors[ImGuiCol_Tab] = ImVec4(0.07f, 0.07f, 0.07f, 0.51f);
			colors[ImGuiCol_TabHovered] = ImVec4(0.86f, 0.23f, 0.43f, 0.67f);
			colors[ImGuiCol_TabActive] = ImVec4(0.19f, 0.19f, 0.19f, 0.57f);
			colors[ImGuiCol_TabUnfocused] = ImVec4(0.05f, 0.05f, 0.05f, 0.90f);
			colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.13f, 0.13f, 0.13f, 0.74f);
			//colors[ImGuiCol_DockingPreview] = ImVec4(0.47f, 0.47f, 0.47f, 0.47f);
			//colors[ImGuiCol_DockingEmptyBg] = ImVec4(0.20f, 0.20f, 0.20f, 1.00f);
			colors[ImGuiCol_PlotLines] = ImVec4(0.61f, 0.61f, 0.61f, 1.00f);
			colors[ImGuiCol_PlotLinesHovered] = ImVec4(1.00f, 0.43f, 0.35f, 1.00f);
			colors[ImGuiCol_PlotHistogram] = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
			colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
			colors[ImGuiCol_TableHeaderBg] = ImVec4(0.19f, 0.19f, 0.20f, 1.00f);
			colors[ImGuiCol_TableBorderStrong] = ImVec4(0.31f, 0.31f, 0.35f, 1.00f);
			colors[ImGuiCol_TableBorderLight] = ImVec4(0.23f, 0.23f, 0.25f, 1.00f);
			colors[ImGuiCol_TableRowBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
			colors[ImGuiCol_TableRowBgAlt] = ImVec4(1.00f, 1.00f, 1.00f, 0.07f);
			colors[ImGuiCol_TextSelectedBg] = ImVec4(0.26f, 0.59f, 0.98f, 0.35f);
			colors[ImGuiCol_DragDropTarget] = ImVec4(1.00f, 1.00f, 0.00f, 0.90f);
			colors[ImGuiCol_NavHighlight] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
			colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.00f, 1.00f, 1.00f, 0.70f);
			colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.20f);
			colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.35f);
			break;

		case Havtorn::EEditorColorTheme::HavtornGreen:

			colors[ImGuiCol_Text] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
			colors[ImGuiCol_TextDisabled] = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
			colors[ImGuiCol_WindowBg] = ImVec4(0.06f, 0.06f, 0.06f, 0.94f);
			colors[ImGuiCol_ChildBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
			colors[ImGuiCol_PopupBg] = ImVec4(0.08f, 0.08f, 0.08f, 0.94f);
			colors[ImGuiCol_Border] = ImVec4(0.43f, 0.43f, 0.50f, 0.50f);
			colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
			colors[ImGuiCol_FrameBg] = ImVec4(0.44f, 0.44f, 0.44f, 0.60f);
			colors[ImGuiCol_FrameBgHovered] = ImVec4(0.57f, 0.57f, 0.57f, 0.70f);
			colors[ImGuiCol_FrameBgActive] = ImVec4(0.76f, 0.76f, 0.76f, 0.80f);
			colors[ImGuiCol_TitleBg] = ImVec4(0.04f, 0.04f, 0.04f, 1.00f);
			colors[ImGuiCol_TitleBgActive] = ImVec4(0.16f, 0.16f, 0.16f, 1.00f);
			colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.00f, 0.00f, 0.00f, 0.60f);
			colors[ImGuiCol_MenuBarBg] = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
			colors[ImGuiCol_ScrollbarBg] = ImVec4(0.02f, 0.02f, 0.02f, 0.53f);
			colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.31f, 0.31f, 0.31f, 1.00f);
			colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.41f, 0.41f, 0.41f, 1.00f);
			colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.51f, 0.51f, 0.51f, 1.00f);
			colors[ImGuiCol_CheckMark] = ImVec4(0.13f, 0.75f, 0.55f, 0.80f);
			colors[ImGuiCol_SliderGrab] = ImVec4(0.13f, 0.75f, 0.75f, 0.80f);
			colors[ImGuiCol_SliderGrabActive] = ImVec4(0.13f, 0.75f, 1.00f, 0.80f);
			colors[ImGuiCol_Button] = ImVec4(0.13f, 0.75f, 0.55f, 0.40f);
			colors[ImGuiCol_ButtonHovered] = ImVec4(0.13f, 0.75f, 0.75f, 0.60f);
			colors[ImGuiCol_ButtonActive] = ImVec4(0.13f, 0.75f, 1.00f, 0.80f);
			colors[ImGuiCol_Header] = ImVec4(0.13f, 0.75f, 0.55f, 0.40f);
			colors[ImGuiCol_HeaderHovered] = ImVec4(0.13f, 0.75f, 0.75f, 0.60f);
			colors[ImGuiCol_HeaderActive] = ImVec4(0.13f, 0.75f, 1.00f, 0.80f);
			colors[ImGuiCol_Separator] = ImVec4(0.13f, 0.75f, 0.55f, 0.40f);
			colors[ImGuiCol_SeparatorHovered] = ImVec4(0.13f, 0.75f, 0.75f, 0.60f);
			colors[ImGuiCol_SeparatorActive] = ImVec4(0.13f, 0.75f, 1.00f, 0.80f);
			colors[ImGuiCol_ResizeGrip] = ImVec4(0.13f, 0.75f, 0.55f, 0.40f);
			colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.13f, 0.75f, 0.75f, 0.60f);
			colors[ImGuiCol_ResizeGripActive] = ImVec4(0.13f, 0.75f, 1.00f, 0.80f);
			colors[ImGuiCol_Tab] = ImVec4(0.13f, 0.75f, 0.55f, 0.80f);
			colors[ImGuiCol_TabHovered] = ImVec4(0.13f, 0.75f, 0.75f, 0.80f);
			colors[ImGuiCol_TabActive] = ImVec4(0.13f, 0.75f, 1.00f, 0.80f);
			colors[ImGuiCol_TabUnfocused] = ImVec4(0.18f, 0.18f, 0.18f, 1.00f);
			colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.36f, 0.36f, 0.36f, 0.54f);
			//colors[ImGuiCol_DockingPreview] = ImVec4(0.13f, 0.75f, 0.55f, 0.80f);
			//colors[ImGuiCol_DockingEmptyBg] = ImVec4(0.13f, 0.13f, 0.13f, 0.80f);
			colors[ImGuiCol_PlotLines] = ImVec4(0.61f, 0.61f, 0.61f, 1.00f);
			colors[ImGuiCol_PlotLinesHovered] = ImVec4(1.00f, 0.43f, 0.35f, 1.00f);
			colors[ImGuiCol_PlotHistogram] = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
			colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
			colors[ImGuiCol_TableHeaderBg] = ImVec4(0.19f, 0.19f, 0.20f, 1.00f);
			colors[ImGuiCol_TableBorderStrong] = ImVec4(0.31f, 0.31f, 0.35f, 1.00f);
			colors[ImGuiCol_TableBorderLight] = ImVec4(0.23f, 0.23f, 0.25f, 1.00f);
			colors[ImGuiCol_TableRowBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
			colors[ImGuiCol_TableRowBgAlt] = ImVec4(1.00f, 1.00f, 1.00f, 0.07f);
			colors[ImGuiCol_TextSelectedBg] = ImVec4(0.26f, 0.59f, 0.98f, 0.35f);
			colors[ImGuiCol_DragDropTarget] = ImVec4(1.00f, 1.00f, 0.00f, 0.90f);
			colors[ImGuiCol_NavHighlight] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
			colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.00f, 1.00f, 1.00f, 0.70f);
			colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.20f);
			colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.35f);
			break;

		case Havtorn::EEditorColorTheme::Count:
		default:
		case Havtorn::EEditorColorTheme::DefaultDark:
			ImGui::StyleColorsDark();
			break;
		}

		switch (styleTheme)
		{
		case Havtorn::EEditorStyleTheme::Havtorn:

			style->WindowPadding = ImVec2(8.00f, 8.00f);
			style->FramePadding = ImVec2(5.00f, 2.00f);
			style->CellPadding = ImVec2(6.00f, 6.00f);
			style->ItemSpacing = ImVec2(6.00f, 6.00f);
			style->ItemInnerSpacing = ImVec2(6.00f, 6.00f);
			style->TouchExtraPadding = ImVec2(0.00f, 0.00f);
			style->IndentSpacing = 25;
			style->ScrollbarSize = 15;
			style->GrabMinSize = 10;
			style->WindowBorderSize = 1;
			style->ChildBorderSize = 1;
			style->PopupBorderSize = 1;
			style->FrameBorderSize = 1;
			style->TabBorderSize = 1;
			style->WindowRounding = 7;
			style->ChildRounding = 4;
			style->FrameRounding = 3;
			style->PopupRounding = 4;
			style->ScrollbarRounding = 9;
			style->GrabRounding = 3;
			style->LogSliderDeadzone = 4;
			style->TabRounding = 4;
			break;

		default:
		case Havtorn::EEditorStyleTheme::Count:
		case Havtorn::EEditorStyleTheme::Default:
			break;
		}
	}

	std::string CImguiManager::GetEditorColorThemeName(EEditorColorTheme colorTheme)
	{
		switch (colorTheme)
		{
		case Havtorn::EEditorColorTheme::DefaultDark:
			return "ImGui Dark";
		case Havtorn::EEditorColorTheme::HavtornDark:
			return "Havtorn Dark";
		case Havtorn::EEditorColorTheme::HavtornRed:
			return "Havtorn Red";
		case Havtorn::EEditorColorTheme::HavtornGreen:
			return "Havtorn Green";
		case Havtorn::EEditorColorTheme::Count:
			return std::string();
		default:
			return std::string();
		}
	}

	::ImVec4 CImguiManager::GetEditorColorThemeRepColor(EEditorColorTheme colorTheme)
	{
		switch (colorTheme)
		{
		case Havtorn::EEditorColorTheme::DefaultDark:
			return ImVec4(0.11f, 0.16f, 0.55f, 1.00f);
		case Havtorn::EEditorColorTheme::HavtornDark:
			return ImVec4(0.51f, 0.36f, 0.15f, 1.00f);
		case Havtorn::EEditorColorTheme::HavtornRed:
			return ImVec4(0.48f, 0.16f, 0.16f, 1.00f);
		case Havtorn::EEditorColorTheme::HavtornGreen:
			return ImVec4(0.13f, 0.75f, 0.55f, 0.40f);
		case Havtorn::EEditorColorTheme::Count:
			return ImVec4();
		default:
			return ImVec4();
		}
	}

	const std::string CImguiManager::GetFrameRate()
	{
		std::string frameRateString = "Framerate: ";
		//I16 frameRate = static_cast<I16>(ImGui::GetIO().Framerate);
		//I16 frameRate = static_cast<I16>((1.0f / CTimer::Dt()));
		I16 frameRate = static_cast<I16>(CTimer::AverageFrameRate());
		frameRateString.append(std::to_string(frameRate));
		return frameRateString;
	}

	const std::string CImguiManager::GetSystemMemory()
	{
		PROCESS_MEMORY_COUNTERS memCounter;
		BOOL result = GetProcessMemoryInfo(GetCurrentProcess(),
			&memCounter,
			sizeof(memCounter));

		if (!result)
		{
			return "";
		}

		SIZE_T memUsed = (memCounter.WorkingSetSize) / 1024;
		SIZE_T memUsedMb = (memCounter.WorkingSetSize) / 1024 / 1024;

		std::string mem = "System Memory: ";
		mem.append(std::to_string(memUsed));
		mem.append("Kb (");
		mem.append(std::to_string(memUsedMb));
		mem.append("Mb)");

		return mem;
	}

	const std::string CImguiManager::GetDrawCalls()
	{
		std::string drawCalls = "Draw Calls: ";
		drawCalls.append(std::to_string(CRenderManager::NumberOfDrawCallsThisFrame));
		return drawCalls;
	}

	//void CImguiManager::LevelsToSelectFrom(std::vector<std::string> someLevelsToSelectFrom)
	//{
	//	for (unsigned int i = 0; i < someLevelsToSelectFrom.size(); ++i) {
	//		const auto& doc = CJsonReader::Get()->LoadDocument(ASSETPATH("Assets/Generated/" + someLevelsToSelectFrom[i]));
	//		if (!doc.HasParseError()) {
	//			if (doc.HasMember("instancedGameobjects") && 
	//				doc.HasMember("modelGameObjects")) {
	//				myLevelsToSelectFrom.push_back(someLevelsToSelectFrom[i]);
	//			}
	//		}
	//	}	
	//}
}
