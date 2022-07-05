// Copyright 2022 Team Havtorn. All Rights Reserved.

#include "Scene.h"

#include "ECS/ECSInclude.h"
#include "ECS/Components/DirectionalLightComponent.h"
#include "Graphics/RenderManager.h"

namespace Havtorn
{
	bool CScene::Init(CRenderManager* renderManager)
	{
		// Setup systems
		Systems.emplace_back(std::make_unique<CCameraSystem>());
		Systems.emplace_back(std::make_unique<CLightSystem>());
		Systems.emplace_back(std::make_unique<CRenderSystem>(renderManager));

		// Create entities
		Entities.emplace_back(std::make_shared<SEntity>(1, "Camera"));
		auto cameraEntity = Entities[0];
		Entities.emplace_back(std::make_shared<SEntity>(2, "DirectionalLight"));
		auto directionalLightEntity = Entities[1];

		// Setup entities (create components)
		auto transform = AddTransformComponentToEntity(cameraEntity);
		transform->Transform.GetMatrix().Translation({ 2.0f, 1.0f, -3.0f });
		transform->Transform.Rotate({ 0.0f, UMath::DegToRad(35.0f), 0.0f });
		transform->Transform.Translate(SVector::Right * 0.25f);

		AddTransformComponentToEntity(directionalLightEntity);

		auto camera = AddCameraComponentToEntity(cameraEntity);
		camera->ProjectionMatrix = SMatrix::PerspectiveFovLH(UMath::DegToRad(70.0f), (16.0f / 9.0f), 0.1f, 1000.0f);
		camera->ViewMatrix = SMatrix::LookAtLH(SVector::Zero, SVector::Forward, SVector::Up);

		auto directionalLight = AddDirectionalLightComponentToEntity(directionalLightEntity);
		directionalLight->Direction = { 0.0f, 1.0f, -1.0f, 0.0f };
		directionalLight->Color = { 212.0f / 255.0f, 175.0f / 255.0f, 55.0f / 255.0f, 0.25f };
		directionalLight->ShadowmapView.ShadowmapViewportIndex = 0;
		directionalLight->ShadowmapView.ShadowProjectionMatrix = SMatrix::OrthographicLH(DirectionalLightComponents.back()->ShadowViewSize.X, DirectionalLightComponents.back()->ShadowViewSize.Y, -8.0f, 8.0f);

		InitDemoScene(renderManager);

		return true;
	}

	void CScene::Update()
	{
		for (const auto& system : Systems)
		{
			system->Update(this);
		}
	}

	void CScene::InitDemoScene(CRenderManager* renderManager)
	{
		U64 newID = Entities.back()->ID + 1;
		Entities.emplace_back(std::make_shared<SEntity>(newID, "Point Light"));
		auto pointLightEntity = Entities.back();

		// Setup entities (create components)
		auto pointLightTransform = AddTransformComponentToEntity(pointLightEntity);
		pointLightTransform->Transform.GetMatrix().Translation({ 1.25f, 0.35f, -1.65f });

		auto pointLightComp = AddPointLightComponentToEntity(pointLightEntity);
		pointLightComp->ColorAndIntensity = { 0.0f, 1.0f, 1.0f, 10.0f };
		pointLightComp->Range = 1.0f;

		const SMatrix constantProjectionMatrix = SMatrix::PerspectiveFovLH(UMath::DegToRad(90.0f), 1.0f, 0.001f, pointLightComp->Range);
		const SVector4 constantPosition = pointLightTransform->Transform.GetMatrix().Translation4();

		// Forward
		SShadowmapViewData& view1 = pointLightComp->ShadowmapViews[0];
		view1.ShadowPosition = constantPosition;
		view1.ShadowmapViewportIndex = 1;
		view1.ShadowViewMatrix = SMatrix::LookAtLH(constantPosition.ToVector3(), (constantPosition + SVector4::Forward).ToVector3(), SVector::Up);
		view1.ShadowProjectionMatrix = constantProjectionMatrix;

		// Right
		SShadowmapViewData& view2 = pointLightComp->ShadowmapViews[1];
		view2.ShadowPosition = constantPosition;
		view2.ShadowmapViewportIndex = 2;
		view2.ShadowViewMatrix = SMatrix::LookAtLH(constantPosition.ToVector3(), (constantPosition + SVector4::Right).ToVector3(), SVector::Up);
		view2.ShadowProjectionMatrix = constantProjectionMatrix;

		// Backward
		SShadowmapViewData& view3 = pointLightComp->ShadowmapViews[2];
		view3.ShadowPosition = constantPosition;
		view3.ShadowmapViewportIndex = 3;
		view3.ShadowViewMatrix = SMatrix::LookAtLH(constantPosition.ToVector3(), (constantPosition + SVector4::Backward).ToVector3(), SVector::Up);
		view3.ShadowProjectionMatrix = constantProjectionMatrix;

		// Left
		SShadowmapViewData& view4 = pointLightComp->ShadowmapViews[3];
		view4.ShadowPosition = constantPosition;
		view4.ShadowmapViewportIndex = 4;
		view4.ShadowViewMatrix = SMatrix::LookAtLH(constantPosition.ToVector3(), (constantPosition + SVector4::Left).ToVector3(), SVector::Up);
		view4.ShadowProjectionMatrix = constantProjectionMatrix;

		// Up
		SShadowmapViewData& view5 = pointLightComp->ShadowmapViews[4];
		view5.ShadowPosition = constantPosition;
		view5.ShadowmapViewportIndex = 5;
		view5.ShadowViewMatrix = SMatrix::LookAtLH(constantPosition.ToVector3(), (constantPosition + SVector4::Up).ToVector3(), SVector::Backward);
		view5.ShadowProjectionMatrix = constantProjectionMatrix;

		// Down
		SShadowmapViewData& view6 = pointLightComp->ShadowmapViews[5];
		view6.ShadowPosition = constantPosition;
		view6.ShadowmapViewportIndex = 6;
		view6.ShadowViewMatrix = SMatrix::LookAtLH(constantPosition.ToVector3(), (constantPosition + SVector4::Down).ToVector3(), SVector::Forward);
		view6.ShadowProjectionMatrix = constantProjectionMatrix;

		const std::string modelPath1 = "Assets/Tests/En_P_PendulumClock.hva";
		const std::vector<std::string> materialNames1 = { "T_PendulumClock", "Checkboard_128x128" };
		const std::string modelPath2 = "Assets/Tests/En_P_Bed.hva";
		const std::vector<std::string> materialNames2 = { "T_Bed", "T_Bedsheet" };
		const std::string modelPath3 = "Assets/Tests/Quad.hva";
		const std::vector<std::string> materialNames3 = { "T_Quad" };

		// === Pendulum ===
		newID = Entities.back()->ID + 1;
		Entities.emplace_back(std::make_shared<SEntity>(newID, "Clock"));
		auto pendulum = Entities.back();

		auto& transform1 = AddTransformComponentToEntity(pendulum)->Transform;
		transform1.GetMatrix().Translation({1.75f, 0.0f, 0.25f});

		renderManager->LoadStaticMeshComponent(modelPath1, AddStaticMeshComponentToEntity(pendulum).get());
		renderManager->LoadMaterialComponent(materialNames1, AddMaterialComponentToEntity(pendulum).get());
		// === !Pendulum ===

		// === Bed ===
		newID = Entities.back()->ID + 1;
		Entities.emplace_back(std::make_shared<SEntity>(newID, "Bed"));
		auto bed = Entities.back();

		auto& transform2 = AddTransformComponentToEntity(bed)->Transform;
		transform2.GetMatrix().Translation({ 0.25f, 0.0f, 0.25f });

		renderManager->LoadStaticMeshComponent(modelPath2, AddStaticMeshComponentToEntity(bed).get());
		renderManager->LoadMaterialComponent(materialNames2, AddMaterialComponentToEntity(bed).get());
		// === !Bed ===

		// === Floor ===
		std::vector<SVector> translations;
		translations.emplace_back(-1.0f, 0.0f, -2.0f);
		translations.emplace_back(0.0f, 0.0f, -2.0f);
		translations.emplace_back(1.0f, 0.0f, -2.0f);
		translations.emplace_back(2.0f, 0.0f, -2.0f);
		translations.emplace_back(-1.0f, 0.0f, -1.0f);
		translations.emplace_back(0.0f, 0.0f, -1.0f);
		translations.emplace_back(1.0f, 0.0f, -1.0f);
		translations.emplace_back(2.0f, 0.0f, -1.0f);
		translations.emplace_back(-1.0f, 0.0f, 0.0f);
		translations.emplace_back(0.0f, 0.0f, 0.0f);
		translations.emplace_back(1.0f, 0.0f, 0.0f);
		translations.emplace_back(2.0f, 0.0f, 0.0f);

		U16 meshStartIndex = 2;
		for (U8 i = 0; i < 12; ++i)
		{
			newID = Entities.back()->ID + 1;
			Entities.emplace_back(std::make_shared<SEntity>(newID, "Floor"));
			auto floor = Entities.back();

			auto& transform3 = AddTransformComponentToEntity(floor)->Transform;
			transform3.GetMatrix().Translation(translations[i]);
			transform3.GetMatrix().SetRotation(SMatrix::CreateRotationAroundZ(UMath::DegToRad(-90.0f)));
			//transform3.Rotate({ 0.0f, 0.0f, -90.0f });

			renderManager->LoadStaticMeshComponent(modelPath3, AddStaticMeshComponentToEntity(floor).get());
			renderManager->LoadMaterialComponent(materialNames3, AddMaterialComponentToEntity(floor).get());
		}
		// === !Floor ===

		// === Wall ===
		translations.clear();
		translations.emplace_back(-1.0f, 0.5f, 0.5f);
		translations.emplace_back(0.0f, 0.5f, 0.5f);
		translations.emplace_back(1.0f, 0.5f, 0.5f);
		translations.emplace_back(2.0f, 0.5f, 0.5f);
		translations.emplace_back(-1.0f, 1.5f, 0.5f);
		translations.emplace_back(0.0f, 1.5f, 0.5f);
		translations.emplace_back(1.0f, 1.5f, 0.5f);
		translations.emplace_back(2.0f, 1.5f, 0.5f);
		translations.emplace_back(-1.0f, 2.5f, 0.5f);
		translations.emplace_back(0.0f, 2.5f, 0.5f);
		translations.emplace_back(1.0f, 2.5f, 0.5f);
		translations.emplace_back(2.0f, 2.5f, 0.5f);

		meshStartIndex = 14;
		for (U8 i = 0; i < 12; ++i)
		{
			newID = Entities.back()->ID + 1;
			Entities.emplace_back(std::make_shared<SEntity>(newID, "Wall"));
			auto floor = Entities.back();

			auto& transform3 = AddTransformComponentToEntity(floor)->Transform;
			transform3.GetMatrix().Translation(translations[i]);
			transform3.GetMatrix().SetRotation(SMatrix::CreateRotationAroundZ(UMath::DegToRad(-90.0f)) * SMatrix::CreateRotationAroundX(UMath::DegToRad(-90.0f)));
			//transform3.Rotate({ -90.0f, 0.0f, -90.0f });

			renderManager->LoadStaticMeshComponent(modelPath3, AddStaticMeshComponentToEntity(floor).get());
			renderManager->LoadMaterialComponent(materialNames3, AddMaterialComponentToEntity(floor).get());
		}
		// === !Wall ===

		// === Other Wall ===
		translations.clear();
		translations.emplace_back(-1.0f, 0.5f, -2.5f);
		translations.emplace_back(-1.0f, 0.5f, -1.5f);
		translations.emplace_back(-1.0f, 0.5f, -0.5f);
		translations.emplace_back(-1.0f, 1.5f, -2.5f);
		translations.emplace_back(-1.0f, 1.5f, -1.5f);
		translations.emplace_back(-1.0f, 1.5f, -0.5f);
		translations.emplace_back(-1.0f, 2.5f, -2.5f);
		translations.emplace_back(-1.0f, 2.5f, -1.5f);
		translations.emplace_back(-1.0f, 2.5f, -0.5f);

		meshStartIndex = 26;
		for (U8 i = 0; i < 9; ++i)
		{
			newID = Entities.back()->ID + 1;
			Entities.emplace_back(std::make_shared<SEntity>(newID, "Wall"));
			auto floor = Entities.back();

			auto& transform3 = AddTransformComponentToEntity(floor)->Transform;
			transform3.GetMatrix().Translation(translations[i]);
			transform3.GetMatrix().SetRotation(SMatrix::CreateRotationAroundZ(UMath::DegToRad(-90.0f)) * SMatrix::CreateRotationAroundX(UMath::DegToRad(-90.0f)) * SMatrix::CreateRotationAroundY(UMath::DegToRad(-90.0f)));
			//transform3.Rotate({ -90.0f, -90.0f, -90.0f });
			
			renderManager->LoadStaticMeshComponent(modelPath3, AddStaticMeshComponentToEntity(floor).get());
			renderManager->LoadMaterialComponent(materialNames3, AddMaterialComponentToEntity(floor).get());
		}
		// === !Other Wall ===

		// === Spotlight ===
		newID = Entities.back()->ID + 1;
		Entities.emplace_back(std::make_shared<SEntity>(newID, "SpotLight"));
		auto spotlight = Entities.back();

		auto& spotlightTransform = AddTransformComponentToEntity(spotlight)->Transform;
		spotlightTransform.Translate({ 1.5f, 0.5f, -1.0f });

		auto spotlightComp = AddSpotLightComponentToEntity(spotlight);
		spotlightComp->Direction = SVector4::Forward;
		spotlightComp->DirectionNormal1 = SVector4::Right;
		spotlightComp->DirectionNormal2 = SVector4::Up;
		spotlightComp->ColorAndIntensity = { 0.0f, 1.0f, 0.0f, 5.0f };
		spotlightComp->AngleExponent = 0.1f;
		spotlightComp->OuterAngle = 25.0f;
		spotlightComp->InnerAngle = 5.0f;
		spotlightComp->Range = 3.0f;

		const SMatrix spotlightProjection = SMatrix::PerspectiveFovLH(UMath::DegToRad(90.0f), 1.0f, 0.001f, spotlightComp->Range);
		const SVector4 spotlightPosition = TransformComponents.back()->Transform.GetMatrix().Translation4();

		spotlightComp->ShadowmapView.ShadowPosition = spotlightPosition;
		spotlightComp->ShadowmapView.ShadowmapViewportIndex = 7;
		spotlightComp->ShadowmapView.ShadowViewMatrix = SMatrix::LookAtLH(spotlightPosition.ToVector3(), (spotlightPosition + spotlightComp->Direction).ToVector3(), spotlightComp->DirectionNormal2.ToVector3());
		spotlightComp->ShadowmapView.ShadowProjectionMatrix = spotlightProjection;
		// === !Spotlight ===
	}

	COMPONENT_ADDER_DEFINITION(TransformComponent)
	COMPONENT_ADDER_DEFINITION(StaticMeshComponent)
	COMPONENT_ADDER_DEFINITION(CameraComponent)
	COMPONENT_ADDER_DEFINITION(MaterialComponent)
	COMPONENT_ADDER_DEFINITION(DirectionalLightComponent)
	COMPONENT_ADDER_DEFINITION(PointLightComponent)
	COMPONENT_ADDER_DEFINITION(SpotLightComponent)
}
