// Copyright 2022 Team Havtorn. All Rights Reserved.

#pragma once
#include "Editor/EditorWindow.h"

namespace Havtorn
{
	class CScene;
}

namespace ImGui
{
	class CInspectorWindow : public CWindow
	{
	public:
		CInspectorWindow(const char* name, Havtorn::CScene* scene, Havtorn::CEditorManager* manager);
		~CInspectorWindow() override;
		void OnEnable() override;
		void OnInspectorGUI() override;
		void OnDisable() override;
	
	private:
		void InspectTransformComponent(Havtorn::I64 transformComponentIndex);
		void InspectStaticMeshComponent(Havtorn::I64 staticMeshComponentIndex);
		void InspectCameraComponent(Havtorn::I64 cameraComponentIndex);
		void InspectMaterialComponent(Havtorn::I64 materialComponentIndex);
		void InspectDirectionalLightComponent(Havtorn::I64 directionalLightComponentIndex);
		void InspectPointLightComponent(Havtorn::I64 pointLightComponentIndex);
		void InspectSpotLightComponent(Havtorn::I64 spotLightComponentIndex);

	private:
		Havtorn::CScene* Scene = nullptr;

		Havtorn::SVector2<Havtorn::F32> TexturePreviewSize = { 64.0f, 64.0f };
		Havtorn::SVector2<Havtorn::F32> DummySize = { 0.0f, 0.5f };
		Havtorn::F32 SlideSpeed = 0.1f;
		bool SelectTextureWindowOpen = false;
	};
}
