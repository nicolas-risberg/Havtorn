// Copyright 2022 Team Havtorn. All Rights Reserved.

#include "hvpch.h"
#include "InspectorWindow.h"
#include <imgui.h>

#include "ECS/ECSInclude.h"
#include "Imgui/ImguiManager.h"
#include "Graphics/RenderManager.h"
#include "Scene/Scene.h"

namespace ImGui
{
	using Havtorn::SEditorLayout;
	using Havtorn::SVector;
	using Havtorn::EComponentType;

	CInspectorWindow::CInspectorWindow(const char* aName, Havtorn::CScene* scene, Havtorn::CImguiManager* manager)
		: CWindow(aName, manager)
		, Scene(scene)
	{
	}

	CInspectorWindow::~CInspectorWindow()
	{
	}

	void CInspectorWindow::OnEnable()
	{
	}

	void CInspectorWindow::OnInspectorGUI()
	{
		const SEditorLayout& layout = Manager->GetEditorLayout();

		const ImGuiViewport* mainViewport = ImGui::GetMainViewport();
		ImGui::SetNextWindowPos(ImVec2(mainViewport->WorkPos.x + layout.InspectorPosition.X, mainViewport->WorkPos.y + layout.InspectorPosition.Y));
		ImGui::SetNextWindowSize(ImVec2(layout.InspectorSize.X, layout.InspectorSize.Y));

		if (ImGui::Begin(Name(), nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoBringToFrontOnFocus))
		{
			if (const auto selection = Manager->GetSelectedEntity())
			{
				ImGui::TextColored((&ImGui::GetStyle())->Colors[ImGuiCol_HeaderActive], selection->Name.c_str());
				ImGui::Separator();

				if (selection->HasComponent(EComponentType::TransformComponent))
				{
					InspectTransformComponent(selection->GetComponentIndex(EComponentType::TransformComponent));
					ImGui::Dummy({ DummySize.X, DummySize.Y });
				}

				if (selection->HasComponent(EComponentType::StaticMeshComponent))
				{
					InspectStaticMeshComponent(selection->GetComponentIndex(EComponentType::StaticMeshComponent));
					ImGui::Dummy({ DummySize.X, DummySize.Y });
				}

				if (selection->HasComponent(EComponentType::CameraComponent))
				{
					InspectCameraComponent(selection->GetComponentIndex(EComponentType::CameraComponent));
					ImGui::Dummy({ DummySize.X, DummySize.Y });
				}

				if (selection->HasComponent(EComponentType::MaterialComponent))
				{
					InspectMaterialComponent(selection->GetComponentIndex(EComponentType::MaterialComponent));
					ImGui::Dummy({ DummySize.X, DummySize.Y });
				}

				if (selection->HasComponent(EComponentType::DirectionalLightComponent))
				{
					InspectDirectionalLightComponent(selection->GetComponentIndex(EComponentType::DirectionalLightComponent));
					ImGui::Dummy({ DummySize.X, DummySize.Y });
				}

				if (selection->HasComponent(EComponentType::PointLightComponent))
				{
					InspectPointLightComponent(selection->GetComponentIndex(EComponentType::PointLightComponent));
					ImGui::Dummy({ DummySize.X, DummySize.Y });
				}

				if (selection->HasComponent(EComponentType::SpotLightComponent))
				{
					InspectSpotLightComponent(selection->GetComponentIndex(EComponentType::SpotLightComponent));
					ImGui::Dummy({ DummySize.X, DummySize.Y });
				}
			}
		}
		ImGui::End();
	}

	void CInspectorWindow::OnDisable()
	{
	}

	void CInspectorWindow::InspectTransformComponent(Havtorn::I64 transformComponentIndex)
	{
		if (ImGui::CollapsingHeader("Transform", ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_DefaultOpen))
		{
			auto& transformMatrix = Scene->GetTransformComponents()[transformComponentIndex]->Transform.GetMatrix();
			const SVector position = transformMatrix.Translation();
			Havtorn::F32 data[3] = { position.X, position.Y, position.Z };
			ImGui::DragFloat3("Position", data, SlideSpeed);
			transformMatrix.Translation({ data[0], data[1], data[2] });
		}
	}

	void CInspectorWindow::InspectStaticMeshComponent(Havtorn::I64 /*staticMeshComponentIndex*/)
	{
		if (ImGui::CollapsingHeader("Static Mesh", ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_DefaultOpen))
		{
			//auto& staticMesh = Scene->GetStaticMeshComponents()[staticMeshComponentIndex];
		}
	}

	void CInspectorWindow::InspectCameraComponent(Havtorn::I64 /*cameraComponentIndex*/)
	{
		if (ImGui::CollapsingHeader("Camera", ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_DefaultOpen))
		{
		}
	}

	void CInspectorWindow::InspectMaterialComponent(Havtorn::I64 materialComponentIndex)
	{
		if (ImGui::CollapsingHeader("Material", ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_DefaultOpen))
		{
			auto& materialComp = Scene->GetMaterialComponents()[materialComponentIndex];
			auto renderManager = Manager->GetRenderManager();

			Havtorn::EMaterialConfiguration materialConfig = renderManager->GetMaterialConfiguration();

			for (Havtorn::U64 materialIndex = 0; materialIndex < materialComp->MaterialReferences.size(); ++materialIndex)
			{
				switch (materialConfig)
				{
				case Havtorn::EMaterialConfiguration::AlbedoMaterialNormal_Packed:
				{
					if (materialIndex % 3 == 0)
					{
						ImGui::Text("Material %i", materialIndex / 3);
						ImGui::Separator();
						ImGui::Text("Albedo");
					}

					if (materialIndex % 3 == 1)
						ImGui::Text("Material");

					if (materialIndex % 3 == 2)
						ImGui::Text("Normal");
				}
					break;
				default:
					break;
				}

				Havtorn::I64 ref = materialComp->MaterialReferences[materialIndex];
				if (ImGui::ImageButton((void*)renderManager->GetTexture(ref), { TexturePreviewSize.X, TexturePreviewSize.Y }))
				{

				}
			}
		}
	}

	void CInspectorWindow::InspectDirectionalLightComponent(Havtorn::I64 /*directionalLightComponentIndex*/)
	{
		if (ImGui::CollapsingHeader("Directional Light", ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_DefaultOpen))
		{
		}
	}

	void CInspectorWindow::InspectPointLightComponent(Havtorn::I64 /*pointLightComponentIndex*/)
	{
		if (ImGui::CollapsingHeader("Point Light", ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_DefaultOpen))
		{
		}
	}

	void CInspectorWindow::InspectSpotLightComponent(Havtorn::I64 /*spotLightComponentIndex*/)
	{
		if (ImGui::CollapsingHeader("Spot Light", ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_DefaultOpen))
		{
		}
	}
}
