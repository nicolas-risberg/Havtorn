#include "hvpch.h"
#include "ImguiManager.h"

#include "ImGui/Core/imgui.h"
#include "ImGui/Core/imgui_impl_win32.h"
#include "ImGui/Core/imgui_impl_dx11.h"

#include <psapi.h>

//#include "Engine.h"
//#include "Scene.h"
//#include "SceneManager.h"
#include "Graphics/RenderManager.h"
//#include "JsonReader.h"
#include "ImGuiWindows.h"
//#include "PostMaster.h"


//#pragma comment(lib, "psapi.lib")

namespace Havtorn
{
	static ImFont* ImGui_LoadFont(ImFontAtlas& atlas, const char* name, float size, const ImVec2 & /*displayOffset*/ = ImVec2(0, 0))
	{
		char* windir = nullptr;
		if (_dupenv_s(&windir, nullptr, "WINDIR") || windir == nullptr)
			return nullptr;

		static const ImWchar ranges[] =
		{
			0x0020, 0x00FF, // Basic Latin + Latin Supplement
			0x0104, 0x017C, // Polish characters and more
			0,
		};

		ImFontConfig config;
		config.OversampleH = 4;
		config.OversampleV = 4;
		config.PixelSnapH = false;

		auto path = std::string(windir) + "\\Fonts\\" + name;
		auto font = atlas.AddFontFromFileTTF(path.c_str(), size, &config, ranges);
		//if (font)
		//font->DisplayOffset = displayOffset;

		free(windir);

		return font;
	}
	ImFontAtlas myFontAtlas;

	CImguiManager::CImguiManager() : myIsEnabled(true)
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
		ImGui::StyleColorsDark();
		ImGui_LoadFont(myFontAtlas, "segoeui.ttf", 22.0f);
		myFontAtlas.Build();

		ImGui::CreateContext(&myFontAtlas);

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
		if (myIsEnabled)
		{
			ImGui::BeginMainMenuBar();

			for (const auto& window : myWindows)
				window->OnMainMenuGUI();

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

		if (ImGui::Begin("Debug info", nullptr))
		{
			ImGui::Text("Framerate: %.0f", ImGui::GetIO().Framerate);
			ImGui::Text(GetSystemMemory().c_str());
			ImGui::Text(GetDrawCalls().c_str());
		}
		ImGui::End();
	}

	//void CImguiManager::Receive(const SMessage& aMessage)
	//{
	//	if (aMessage.myMessageType == EMessageType::CursorHideAndLock)
	//	{
	//		myIsEnabled = false;
	//	}
	//	else if (aMessage.myMessageType == EMessageType::CursorShowAndUnlock)
	//	{
	//		myIsEnabled = true;
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
//					myIsEnabled = !myIsEnabled;
//				}
//			}
//		}
//	/*	ImGui::TreePop();
//	}*/
//
//
//	ImGui::End();
//}

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
