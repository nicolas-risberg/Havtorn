// Copyright 2022 Team Havtorn. All Rights Reserved.

#include "hvpch.h"
#include "AssetBrowserWindow.h"
#include <imgui.h>
#include "Editor/EditorManager.h"
#include "Editor/EditorResourceManager.h"
#include "Engine.h"
#include "FileSystem/FileSystem.h"

namespace ImGui
{
	using Havtorn::F32;

	CAssetBrowserWindow::CAssetBrowserWindow(const char* name, Havtorn::CEditorManager* manager)
		: CWindow(name, manager)
		, FileSystem(Havtorn::CEngine::GetInstance()->GetFileSystem())
	{
		CurrentDirectory = std::filesystem::path(DefaultAssetPath);
	}

	CAssetBrowserWindow::~CAssetBrowserWindow()
	{
	}

	void CAssetBrowserWindow::OnEnable()
	{
	}

	void CAssetBrowserWindow::OnInspectorGUI()
	{
		const Havtorn::SEditorLayout& layout = Manager->GetEditorLayout();

		const ImGuiViewport* mainViewport = ImGui::GetMainViewport();
		ImGui::SetNextWindowPos(ImVec2(mainViewport->WorkPos.x + layout.AssetBrowserPosition.X, mainViewport->WorkPos.y + layout.AssetBrowserPosition.Y));
		ImGui::SetNextWindowSize(ImVec2(layout.AssetBrowserSize.X, layout.AssetBrowserSize.Y));

		if (ImGui::Begin(Name(), nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoBringToFrontOnFocus))
		{
			if (CurrentDirectory != std::filesystem::path(DefaultAssetPath))
			{
				if (ImGui::Button("<-"))
				{
					CurrentDirectory = CurrentDirectory.parent_path();
				}
			}

			F32 thumbnailPadding = 8.0f;
			F32 cellWidth = ThumbnailSize.X + thumbnailPadding;
			F32 panelWidth = ImGui::GetContentRegionAvail().x;
			Havtorn::I32 columnCount = static_cast<Havtorn::I32>(panelWidth / cellWidth);

			void* folderIconID = (void*)Manager->GetResourceManager()->GetEditorTexture(Havtorn::EEditorTexture::FolderIcon);

			Havtorn::U32 id = 0;
			if (ImGui::BeginTable("FileStructure", columnCount))
			{
				for (const auto& entry : std::filesystem::directory_iterator(CurrentDirectory))
				{
					ImGui::TableNextColumn();
					ImGui::PushID(id++);

					const auto& path = entry.path();
					auto relativePath = std::filesystem::relative(path);
					std::string filenameString = relativePath.filename().string();

					if (entry.is_directory())
					{	
						if (ImGui::ImageButton(folderIconID, { ThumbnailSize.X, ThumbnailSize.Y }))
						{
							CurrentDirectory /= path.filename();
						}
						
						ImGui::Text(filenameString.c_str());
						if (ImGui::IsItemHovered())
							ImGui::SetTooltip(filenameString.c_str());
					}
					else
					{
						const auto& rep = Manager->GetAssetRepFromDirEntry(entry);
						if (!rep->TextureRef)
							rep->TextureRef = (void*)Manager->GetResourceManager()->GetEditorTexture(Havtorn::EEditorTexture::FileIcon);

						if (ImGui::ImageButton(rep->TextureRef, { ThumbnailSize.X, ThumbnailSize.Y }))
						{
							// NR: Open Tool depending on asset type
						}

						ImGui::Text(rep->Name.c_str());
						if (ImGui::IsItemHovered())
							ImGui::SetTooltip(rep->Name.c_str());
					}

					ImGui::PopID();
				}

				ImGui::EndTable();
			}
		}
		ImGui::End();
	}

	void CAssetBrowserWindow::OnDisable()
	{
	}
}