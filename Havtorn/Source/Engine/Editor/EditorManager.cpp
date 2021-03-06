// Copyright 2022 Team Havtorn. All Rights Reserved.

#include "hvpch.h"
#include "EditorManager.h"

#include "Editor/Core/imgui.h"
#include "Editor/Core/imgui_impl_win32.h"
#include "Editor/Core/imgui_impl_dx11.h"

#include <psapi.h>

#include "Engine.h"
#include "FileSystem/FileSystem.h"
#include "Application/WindowHandler.h"
//#include "Scene.h"
//#include "SceneManager.h"
#include "Graphics/GraphicsFramework.h"
#include "Graphics/RenderManager.h"
#include "ECS/ECSInclude.h"
//#include "JsonReader.h"
#include "EditorResourceManager.h"
#include "EditorWindows.h"
#include "EditorToggleables.h"
//#include "PostMaster.h"


//#pragma comment(lib, "psapi.lib")

namespace Havtorn
{
	CEditorManager::CEditorManager()
	{
	}

	CEditorManager::~CEditorManager()
	{
		RenderManager = nullptr;
		SAFE_DELETE(ResourceManager);
		ImGui_ImplWin32_Shutdown();
		ImGui_ImplDX11_Shutdown();
		ImGui::DestroyContext();
	}

	bool CEditorManager::Init(const CGraphicsFramework* framework, const CWindowHandler* windowHandler, CRenderManager* renderManager, CScene* scene)
	{
		ImGui::DebugCheckVersionAndDataLayout("1.86 WIP", sizeof(ImGuiIO), sizeof(ImGuiStyle), sizeof(ImVec2), sizeof(ImVec4), sizeof(ImDrawVert), sizeof(unsigned int));
		ImGui::CreateContext();
		SetEditorTheme(EEditorColorTheme::HavtornDark, EEditorStyleTheme::Havtorn);
		//ImGui::GetIO().Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\verdana.ttf", 14.0f);
		ImGui::GetIO().Fonts->AddFontFromFileTTF("../External/imgui/misc/fonts/Roboto-Medium.ttf", 15.0f);
		ImGui::CreateContext();

		MenuElements.emplace_back(std::make_unique<ImGui::CFileMenu>("File", this));
		MenuElements.emplace_back(std::make_unique<ImGui::CEditMenu>("Edit", this));
		MenuElements.emplace_back(std::make_unique<ImGui::CViewMenu>("View", this));
		MenuElements.emplace_back(std::make_unique<ImGui::CWindowMenu>("Window", this));
		MenuElements.emplace_back(std::make_unique<ImGui::CHelpMenu>("Help", this));

		Windows.emplace_back(std::make_unique<ImGui::CViewportWindow>("Viewport", this));
		Windows.emplace_back(std::make_unique<ImGui::CAssetBrowserWindow>("Asset Browser", this));
		Windows.emplace_back(std::make_unique<ImGui::CHierarchyWindow>("Hierarchy", scene, this));
		Windows.emplace_back(std::make_unique<ImGui::CInspectorWindow>("Inspector", scene, this));

		ResourceManager = new CEditorResourceManager();
		bool success = ResourceManager->Init(renderManager, framework);
		if (!success)
			return false;
		
		InitEditorLayout();
		InitAssetRepresentations();

		success = ImGui_ImplWin32_Init(windowHandler->GetWindowHandle());
		if (!success)
			return false;

		success = ImGui_ImplDX11_Init(framework->GetDevice(), framework->GetContext());
		if (!success)
			return false;

		RenderManager = renderManager;

		return success;
	}

	void CEditorManager::BeginFrame()
	{
		ImGui_ImplDX11_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();
	}

	void CEditorManager::Render()
	{
		// Main Menu bar
		if (IsEnabled)
		{
			ImGui::BeginMainMenuBar();

			for (const auto& element : MenuElements)
				element->OnInspectorGUI();

			ImGui::EndMainMenuBar();
		}

		// Windows
		for (const auto& window : Windows)
			window->OnInspectorGUI();

		DebugWindow();
	}

	void CEditorManager::EndFrame()
	{
		ImGui::Render();
		ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
	}

	void CEditorManager::DebugWindow()
	{
		if (IsDemoOpen)
		{
			ImGui::ShowDemoWindow(&IsDemoOpen);
		}

		if (IsDebugInfoOpen)
		{
			if (ImGui::Begin("Debug info", &IsDebugInfoOpen))
			{
				ImGui::Text(GetFrameRate().c_str());
				ImGui::Text(GetSystemMemory().c_str());
				ImGui::Text(GetDrawCalls().c_str());
			}

			ImGui::End();
		}
	}

	void CEditorManager::SetSelectedEntity(Ref<SEntity> entity)
	{
		SelectedEntity = entity;
	}

	Ref<SEntity> CEditorManager::GetSelectedEntity() const
	{
		return SelectedEntity;
	}

	const Ptr<SEditorAssetRepresentation>& CEditorManager::GetAssetRepFromDirEntry(const std::filesystem::directory_entry& dirEntry)
	{
		for (const auto& rep : AssetRepresentations)
		{
			if (dirEntry == rep->DirectoryEntry)
				return rep;
		}

		// NR: Return empty rep
		return AssetRepresentations[0];
	}

	const Ptr<SEditorAssetRepresentation>& CEditorManager::GetAssetRepFromImageRef(void* imageRef)
	{
		for (const auto& rep : AssetRepresentations)
		{
			if (imageRef == rep->TextureRef)
				return rep;
		}

		// NR: Return empty rep
		return AssetRepresentations[0];
	}

	void CEditorManager::SetEditorTheme(EEditorColorTheme colorTheme, EEditorStyleTheme styleTheme)
	{
		switch (colorTheme)
		{
		case Havtorn::EEditorColorTheme::HavtornDark:
		{
			SEditorColorProfile colorProfile;
			colorProfile.BackgroundBase = SVector4(0.11f, 0.11f, 0.11f, 1.00f);
			colorProfile.BackgroundMid = SVector4(0.198f, 0.198f, 0.198f, 1.00f);
			colorProfile.ElementBackground = SVector4(0.278f, 0.271f, 0.267f, 1.00f);
			colorProfile.ElementHovered = SVector4(0.478f, 0.361f, 0.188f, 1.00f);
			colorProfile.ElementActive = SVector4(0.814f, 0.532f, 0.00f, 1.00f);
			colorProfile.ElementHighlight = SVector4(1.00f, 0.659f, 0.00f, 1.00f);
			SetEditorColorProfile(colorProfile);
		}
		break;

		case Havtorn::EEditorColorTheme::HavtornRed:
		{
			SEditorColorProfile colorProfile;
			colorProfile.BackgroundBase = SVector4(0.11f, 0.11f, 0.11f, 1.00f);
			colorProfile.BackgroundMid = SVector4(0.198f, 0.198f, 0.198f, 1.00f);
			colorProfile.ElementBackground = SVector4(0.278f, 0.271f, 0.267f, 1.00f);
			colorProfile.ElementHovered = SVector4(0.478f, 0.188f, 0.188f, 1.00f);
			colorProfile.ElementActive = SVector4(0.814f, 0.00f, 0.00f, 1.00f);
			colorProfile.ElementHighlight = SVector4(1.00f, 0.00f, 0.00f, 1.00f);
			SetEditorColorProfile(colorProfile);
		}
		break;

		case Havtorn::EEditorColorTheme::HavtornGreen:
		{
			SEditorColorProfile colorProfile;
			colorProfile.BackgroundBase = SVector4(0.11f, 0.11f, 0.11f, 1.00f);
			colorProfile.BackgroundMid = SVector4(0.198f, 0.198f, 0.198f, 1.00f);
			colorProfile.ElementBackground = SVector4(0.278f, 0.271f, 0.267f, 1.00f);
			colorProfile.ElementHovered = SVector4(0.355f, 0.478f, 0.188f, 1.00f);
			colorProfile.ElementActive = SVector4(0.469f, 0.814f, 0.00f, 1.00f);
			colorProfile.ElementHighlight = SVector4(0.576f, 1.00f, 0.00f, 1.00f);
			SetEditorColorProfile(colorProfile);
		}
		break;

		case Havtorn::EEditorColorTheme::Count:
		case Havtorn::EEditorColorTheme::DefaultDark:
			ImGui::StyleColorsDark();
			break;
		}

		ImGuiStyle* style = &ImGui::GetStyle();

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
			style->WindowRounding = 1;
			style->ChildRounding = 1;
			style->FrameRounding = 1;
			style->PopupRounding = 1;
			style->ScrollbarRounding = 1;
			style->GrabRounding = 1;
			style->LogSliderDeadzone = 4;
			style->TabRounding = 1;
			break;
		case Havtorn::EEditorStyleTheme::Count:
		case Havtorn::EEditorStyleTheme::Default:
			break;
		}
	}

	std::string CEditorManager::GetEditorColorThemeName(const EEditorColorTheme colorTheme)
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
			return {};
		}
		return {};
	}

	ImVec4 CEditorManager::GetEditorColorThemeRepColor(const EEditorColorTheme colorTheme)
	{
		switch (colorTheme)
		{
		case Havtorn::EEditorColorTheme::DefaultDark:
			return { 0.11f, 0.16f, 0.55f, 1.00f };
		case Havtorn::EEditorColorTheme::HavtornDark:
			return { 0.478f, 0.361f, 0.188f, 1.00f };
		case Havtorn::EEditorColorTheme::HavtornRed:
			return { 0.478f, 0.188f, 0.188f, 1.00f };
		case Havtorn::EEditorColorTheme::HavtornGreen:
			return { 0.355f, 0.478f, 0.188f, 1.00f };
		case Havtorn::EEditorColorTheme::Count:
			return {};
		}
		return {};
	}

	const SEditorLayout& CEditorManager::GetEditorLayout() const
	{
		return EditorLayout;
	}

	F32 CEditorManager::GetViewportPadding() const
	{
		return ViewportPadding;
	}

	void CEditorManager::SetViewportPadding(const F32 padding)
	{
		ViewportPadding = padding;
		InitEditorLayout();
	}

	const CRenderManager* CEditorManager::GetRenderManager() const
	{
		return RenderManager;
	}

	const CEditorResourceManager* CEditorManager::GetResourceManager() const
	{
		return ResourceManager;
	}

	void CEditorManager::ToggleDebugInfo()
	{
		IsDebugInfoOpen = !IsDebugInfoOpen;
	}

	void CEditorManager::ToggleDemo()
	{
		IsDemoOpen = !IsDemoOpen;
	}

	void CEditorManager::InitEditorLayout()
	{
		EditorLayout = SEditorLayout();

		const Havtorn::SVector2<F32> resolution = CEngine::GetInstance()->GetWindowHandler()->GetResolution();

		constexpr F32 viewportAspectRatioInv = (9.0f / 16.0f);
		const F32 viewportPaddingX = ViewportPadding;
		constexpr F32 viewportPaddingY = 0.0f;

		I16 viewportPosX = static_cast<I16>(resolution.X * viewportPaddingX);
		I16 viewportPosY = static_cast<I16>(viewportPaddingY);
		U16 viewportSizeX = static_cast<U16>(resolution.X - (2.0f * static_cast<F32>(viewportPosX)));
		U16 viewportSizeY = static_cast<U16>(static_cast<F32>(viewportSizeX) * viewportAspectRatioInv);

		// NR: This might be windows menu bar height?
		U16 sizeOffsetY = 20;

		EditorLayout.ViewportPosition = { viewportPosX, viewportPosY };
		EditorLayout.ViewportSize = { viewportSizeX, viewportSizeY };
		EditorLayout.AssetBrowserPosition = { viewportPosX, static_cast<I16>(viewportPosY + viewportSizeY) };
		EditorLayout.AssetBrowserSize = { viewportSizeX, static_cast<U16>(resolution.Y - static_cast<F32>(viewportSizeY) - sizeOffsetY) };
		EditorLayout.HierarchyViewPosition = { 0, viewportPosY };
		EditorLayout.HierarchyViewSize = { static_cast<U16>(viewportPosX), static_cast<U16>(resolution.Y - sizeOffsetY) };
		EditorLayout.InspectorPosition = { static_cast<I16>(resolution.X - static_cast<F32>(viewportPosX)), viewportPosY };
		EditorLayout.InspectorSize = { static_cast<U16>(viewportPosX), static_cast<U16>(resolution.Y - sizeOffsetY) };
	}

	void CEditorManager::InitAssetRepresentations()
	{
		// NR: Fill first slot with a null entry
		AssetRepresentations.emplace_back(std::make_unique<SEditorAssetRepresentation>());

		// Preprocess - Import non-.hva files to .hva
		for (const auto& entry : std::filesystem::recursive_directory_iterator(std::filesystem::path("Assets")))
		{
			if (entry.path().extension() == ".hva" || entry.is_directory())
				continue;

			std::string fileName = entry.path().string();
			std::string extension = entry.path().extension().string();
			for (auto& character : extension)
				character = std::tolower(character, {});

			EAssetType assetType = EAssetType::None;

			// TODO.NR: Make sure this works for animations as well
			if (extension == ".fbx")
				assetType = EAssetType::StaticMesh;
			else if (extension == ".dds" || extension == ".tga")
				assetType = EAssetType::Texture;

			ResourceManager->ConvertToHVA(fileName, assetType);
		}

		for (const auto& entry : std::filesystem::recursive_directory_iterator(std::filesystem::path("Assets")))
		{
			if (entry.is_directory())
				continue;

			std::string fileName = entry.path().string();
			const U64 fileSize = CEngine::GetInstance()->GetFileSystem()->GetFileSize(fileName);
			char* data = new char[fileSize];

			CEngine::GetInstance()->GetFileSystem()->Deserialize(fileName, data, static_cast<U32>(fileSize));

			SEditorAssetRepresentation rep;

			const U32 size = sizeof(EAssetType);
			memcpy(&rep.AssetType, &data[0], size);

			rep.DirectoryEntry = entry;
			rep.Name = entry.path().filename().string();
			rep.TextureRef = ResourceManager->RenderAssetTexure(rep.AssetType, fileName);

			AssetRepresentations.emplace_back(std::make_unique<SEditorAssetRepresentation>(rep));

			delete[] data;
		}
	}

	void CEditorManager::SetEditorColorProfile(const SEditorColorProfile& colorProfile)
	{
		ImVec4* colors = (&ImGui::GetStyle())->Colors;

		const ImVec4 backgroundBase = { colorProfile.BackgroundBase.X, colorProfile.BackgroundBase.Y, colorProfile.BackgroundBase.Z, colorProfile.BackgroundBase.W };
		const ImVec4 backgroundMid = { colorProfile.BackgroundMid.X, colorProfile.BackgroundMid.Y, colorProfile.BackgroundMid.Z, colorProfile.BackgroundMid.W };

		const ImVec4 elementBackground = { colorProfile.ElementBackground.X, colorProfile.ElementBackground.Y, colorProfile.ElementBackground.Z, colorProfile.ElementBackground.W };
		const ImVec4 elementHovered = { colorProfile.ElementHovered.X, colorProfile.ElementHovered.Y, colorProfile.ElementHovered.Z, colorProfile.ElementHovered.W };
		const ImVec4 elementActive = { colorProfile.ElementActive.X, colorProfile.ElementActive.Y, colorProfile.ElementActive.Z, colorProfile.ElementActive.W };
		const ImVec4 elementHighlight = { colorProfile.ElementHighlight.X, colorProfile.ElementHighlight.Y, colorProfile.ElementHighlight.Z, colorProfile.ElementHighlight.W };

		colors[ImGuiCol_Text] = ImVec4(0.92f, 0.92f, 0.92f, 1.00f);
		colors[ImGuiCol_TextDisabled] = ImVec4(0.44f, 0.44f, 0.44f, 1.00f);
		colors[ImGuiCol_WindowBg] = backgroundMid;
		colors[ImGuiCol_ChildBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
		colors[ImGuiCol_PopupBg] = ImVec4(0.13f, 0.13f, 0.13f, 0.94f);
		colors[ImGuiCol_Border] = ImVec4(0.05f, 0.05f, 0.04f, 0.94f);
		colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
		colors[ImGuiCol_FrameBg] = elementBackground;
		colors[ImGuiCol_FrameBgHovered] = elementHovered;
		colors[ImGuiCol_FrameBgActive] = elementBackground;
		colors[ImGuiCol_TitleBg] = elementHovered;
		colors[ImGuiCol_TitleBgActive] = elementActive;
		colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.00f, 0.00f, 0.00f, 0.51f);
		colors[ImGuiCol_MenuBarBg] = backgroundBase;
		colors[ImGuiCol_ScrollbarBg] = backgroundBase;
		colors[ImGuiCol_ScrollbarGrab] = elementBackground;
		colors[ImGuiCol_ScrollbarGrabHovered] = elementHovered;
		colors[ImGuiCol_ScrollbarGrabActive] = elementActive;
		colors[ImGuiCol_CheckMark] = elementActive;
		colors[ImGuiCol_SliderGrab] = elementActive;
		colors[ImGuiCol_SliderGrabActive] = elementActive;
		colors[ImGuiCol_Button] = elementBackground;
		colors[ImGuiCol_ButtonHovered] = elementHovered;
		colors[ImGuiCol_ButtonActive] = elementActive;
		colors[ImGuiCol_Header] = elementBackground;
		colors[ImGuiCol_HeaderHovered] = elementHovered;
		colors[ImGuiCol_HeaderActive] = elementActive;
		colors[ImGuiCol_Separator] = elementBackground;
		colors[ImGuiCol_SeparatorHovered] = elementHovered;
		colors[ImGuiCol_SeparatorActive] = elementActive;
		colors[ImGuiCol_ResizeGrip] = backgroundBase;
		colors[ImGuiCol_ResizeGripHovered] = elementHovered;
		colors[ImGuiCol_ResizeGripActive] = elementActive;
		colors[ImGuiCol_Tab] = elementBackground;
		colors[ImGuiCol_TabHovered] = elementHovered;
		colors[ImGuiCol_TabActive] = elementActive;
		colors[ImGuiCol_TabUnfocused] = ImVec4(0.07f, 0.10f, 0.15f, 0.97f);
		colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.14f, 0.26f, 0.42f, 1.00f);
		colors[ImGuiCol_PlotLines] = ImVec4(0.61f, 0.61f, 0.61f, 1.00f);
		colors[ImGuiCol_PlotLinesHovered] = elementHighlight;
		colors[ImGuiCol_PlotHistogram] = elementHighlight;
		colors[ImGuiCol_PlotHistogramHovered] = elementActive;
		colors[ImGuiCol_TextSelectedBg] = ImVec4(0.26f, 0.59f, 0.98f, 0.35f);
		colors[ImGuiCol_DragDropTarget] = elementHighlight;
		colors[ImGuiCol_NavHighlight] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
		colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.00f, 1.00f, 1.00f, 0.70f);
		colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.20f);
		colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.35f);
	}

	std::string CEditorManager::GetFrameRate() const
	{
		std::string frameRateString = "Framerate: ";
		const U16 frameRate = static_cast<U16>(CTimer::AverageFrameRate());
		frameRateString.append(std::to_string(frameRate));
		return frameRateString;
	}

	std::string CEditorManager::GetSystemMemory() const
	{
		PROCESS_MEMORY_COUNTERS memCounter;
		if (GetProcessMemoryInfo(GetCurrentProcess(),
			&memCounter,
			sizeof memCounter))
		{
			const SIZE_T memUsed = (memCounter.WorkingSetSize) / 1024;
			const SIZE_T memUsedMb = (memCounter.WorkingSetSize) / 1024 / 1024;

			std::string mem = "System Memory: ";
			mem.append(std::to_string(memUsed));
			mem.append("Kb (");
			mem.append(std::to_string(memUsedMb));
			mem.append("Mb)");

			return mem;
		}

		return "";
	}

	std::string CEditorManager::GetDrawCalls() const
	{
		std::string drawCalls = "Draw Calls: ";
		drawCalls.append(std::to_string(CRenderManager::NumberOfDrawCallsThisFrame));
		return drawCalls;
	}
}
