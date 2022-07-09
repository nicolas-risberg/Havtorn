// Copyright 2022 Team Havtorn. All Rights Reserved.

#pragma once
#include "Editor/EditorWindow.h"
#include <filesystem>

namespace Havtorn
{
	class CFileSystem;
}

namespace ImGui
{
	class CAssetBrowserWindow : public CWindow
	{
	public:
		CAssetBrowserWindow(const char* name, Havtorn::CEditorManager* manager);
		~CAssetBrowserWindow() override;
		void OnEnable() override;
		void OnInspectorGUI() override;
		void OnDisable() override;

	private:
		Havtorn::SVector2<Havtorn::F32> ThumbnailSize = { 64.0f, 64.0f };
		Havtorn::CFileSystem* FileSystem = nullptr;
		const std::string DefaultAssetPath = "Assets";
		std::filesystem::path CurrentDirectory = "";
		bool IsCurrentlySelected = false;
	};
}
