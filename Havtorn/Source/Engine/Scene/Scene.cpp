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
		TransformComponents.emplace_back(std::make_shared<STransformComponent>(cameraEntity, EComponentType::TransformComponent));
		cameraEntity->AddComponent(EComponentType::TransformComponent, 0);
		TransformComponents.back()->Transform.GetMatrix().Translation({ 2.0f, 1.0f, -3.0f });
		TransformComponents.back()->Transform.Rotate({ 0.0f, UMath::DegToRad(35.0f), 0.0f });
		TransformComponents.back()->Transform.Translate(SVector::Right * 0.25f);

		TransformComponents.emplace_back(std::make_shared<STransformComponent>(directionalLightEntity, EComponentType::TransformComponent));
		directionalLightEntity->AddComponent(EComponentType::TransformComponent, 1);

		CameraComponents.emplace_back(std::make_shared<SCameraComponent>(cameraEntity, EComponentType::CameraComponent));
		cameraEntity->AddComponent(EComponentType::CameraComponent, 0);
		CameraComponents.back()->ProjectionMatrix = SMatrix::PerspectiveFovLH(UMath::DegToRad(70.0f), (16.0f / 9.0f), 0.1f, 1000.0f);
		CameraComponents.back()->ViewMatrix = SMatrix::LookAtLH(SVector::Zero, SVector::Forward, SVector::Up);

		DirectionalLightComponents.emplace_back(std::make_shared<SDirectionalLightComponent>(directionalLightEntity, EComponentType::DirectionalLightComponent));
		directionalLightEntity->AddComponent(EComponentType::DirectionalLightComponent, 0);
		DirectionalLightComponents.back()->Direction = { 0.0f, 1.0f, -1.0f, 0.0f };
		DirectionalLightComponents.back()->Color = { 212.0f / 255.0f, 175.0f / 255.0f, 55.0f / 255.0f, 0.25f };
		DirectionalLightComponents.back()->ShadowmapView.ShadowmapViewportIndex = 0;
		DirectionalLightComponents.back()->ShadowmapView.ShadowProjectionMatrix = SMatrix::OrthographicLH(DirectionalLightComponents.back()->ShadowViewSize.X, DirectionalLightComponents.back()->ShadowViewSize.Y, -8.0f, 8.0f);

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
		TransformComponents.emplace_back(std::make_shared<STransformComponent>(pointLightEntity, EComponentType::TransformComponent));
		pointLightEntity->AddComponent(EComponentType::TransformComponent, 2);
		TransformComponents.back()->Transform.GetMatrix().Translation({ 1.25f, 0.5f, -1.5f });

		PointLightComponents.emplace_back(std::make_shared<SPointLightComponent>(pointLightEntity, EComponentType::PointLightComponent));
		pointLightEntity->AddComponent(EComponentType::PointLightComponent, 0);
		const auto& pointLightComp = PointLightComponents.back();
		pointLightComp->ColorAndIntensity = { 0.0f, 1.0f, 1.0f, 10.0f };
		pointLightComp->Range = 1.0f;

		const SMatrix constantProjectionMatrix = SMatrix::PerspectiveFovLH(UMath::DegToRad(90.0f), 1.0f, 0.0001f, pointLightComp->Range);
		const SVector4 constantPosition = TransformComponents.back()->Transform.GetMatrix().Translation4();

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

		TransformComponents.emplace_back(std::make_shared<STransformComponent>(pendulum, EComponentType::TransformComponent));
		pendulum->AddComponent(EComponentType::TransformComponent, 3);
		auto& transform1 = TransformComponents.back()->Transform;
		transform1.GetMatrix().Translation({1.75f, 0.0f, 0.25f});

		StaticMeshComponents.emplace_back(std::make_shared<SStaticMeshComponent>(pendulum, EComponentType::StaticMeshComponent));
		renderManager->LoadStaticMeshComponent(modelPath1, StaticMeshComponents.back().get());
		pendulum->AddComponent(EComponentType::StaticMeshComponent, 0);

		MaterialComponents.emplace_back(std::make_shared<SMaterialComponent>(pendulum, EComponentType::MaterialComponent));
		renderManager->LoadMaterialComponent(materialNames1, MaterialComponents.back().get());
		pendulum->AddComponent(EComponentType::MaterialComponent, 0);
		// === !Pendulum ===

		// === Bed ===
		newID = Entities.back()->ID + 1;
		Entities.emplace_back(std::make_shared<SEntity>(newID, "Bed"));
		auto bed = Entities.back();

		TransformComponents.emplace_back(std::make_shared<STransformComponent>(bed, EComponentType::TransformComponent));
		bed->AddComponent(EComponentType::TransformComponent, 4);
		auto& transform2 = TransformComponents.back()->Transform;
		transform2.GetMatrix().Translation({ 0.25f, 0.0f, 0.25f });

		StaticMeshComponents.emplace_back(std::make_shared<SStaticMeshComponent>(bed, EComponentType::StaticMeshComponent));
		renderManager->LoadStaticMeshComponent(modelPath2, StaticMeshComponents.back().get());
		bed->AddComponent(EComponentType::StaticMeshComponent, 1);

		MaterialComponents.emplace_back(std::make_shared<SMaterialComponent>(bed, EComponentType::MaterialComponent));
		renderManager->LoadMaterialComponent(materialNames2, MaterialComponents.back().get());
		bed->AddComponent(EComponentType::MaterialComponent, 1);
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

			TransformComponents.emplace_back(std::make_shared<STransformComponent>(floor, EComponentType::TransformComponent));
			floor->AddComponent(EComponentType::TransformComponent, meshStartIndex + i + 3);
			auto& transform3 = TransformComponents.back()->Transform;
			transform3.GetMatrix().Translation(translations[i]);
			transform3.GetMatrix().SetRotation(SMatrix::CreateRotationAroundZ(UMath::DegToRad(-90.0f)));
			//transform3.Rotate({ 0.0f, 0.0f, -90.0f });

			StaticMeshComponents.emplace_back(std::make_shared<SStaticMeshComponent>(floor, EComponentType::StaticMeshComponent));
			renderManager->LoadStaticMeshComponent(modelPath3, StaticMeshComponents.back().get());
			floor->AddComponent(EComponentType::StaticMeshComponent, meshStartIndex + i);

			MaterialComponents.emplace_back(std::make_shared<SMaterialComponent>(floor, EComponentType::MaterialComponent));
			renderManager->LoadMaterialComponent(materialNames3, MaterialComponents.back().get());
			floor->AddComponent(EComponentType::MaterialComponent, meshStartIndex + i);
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

			TransformComponents.emplace_back(std::make_shared<STransformComponent>(floor, EComponentType::TransformComponent));
			floor->AddComponent(EComponentType::TransformComponent, meshStartIndex + i + 3);
			auto& transform3 = TransformComponents.back()->Transform;
			transform3.GetMatrix().Translation(translations[i]);
			transform3.GetMatrix().SetRotation(SMatrix::CreateRotationAroundZ(UMath::DegToRad(-90.0f)) * SMatrix::CreateRotationAroundX(UMath::DegToRad(-90.0f)));
			//transform3.Rotate({ -90.0f, 0.0f, -90.0f });

			StaticMeshComponents.emplace_back(std::make_shared<SStaticMeshComponent>(floor, EComponentType::StaticMeshComponent));
			renderManager->LoadStaticMeshComponent(modelPath3, StaticMeshComponents.back().get());
			floor->AddComponent(EComponentType::StaticMeshComponent, meshStartIndex + i);

			MaterialComponents.emplace_back(std::make_shared<SMaterialComponent>(floor, EComponentType::MaterialComponent));
			renderManager->LoadMaterialComponent(materialNames3, MaterialComponents.back().get());
			floor->AddComponent(EComponentType::MaterialComponent, meshStartIndex + i);
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

			TransformComponents.emplace_back(std::make_shared<STransformComponent>(floor, EComponentType::TransformComponent));
			floor->AddComponent(EComponentType::TransformComponent, meshStartIndex + i + 3);
			auto& transform3 = TransformComponents.back()->Transform;
			transform3.GetMatrix().Translation(translations[i]);
			transform3.GetMatrix().SetRotation(SMatrix::CreateRotationAroundZ(UMath::DegToRad(-90.0f)) * SMatrix::CreateRotationAroundX(UMath::DegToRad(-90.0f)) * SMatrix::CreateRotationAroundY(UMath::DegToRad(-90.0f)));
			//transform3.Rotate({ -90.0f, -90.0f, -90.0f });

			StaticMeshComponents.emplace_back(std::make_shared<SStaticMeshComponent>(floor, EComponentType::StaticMeshComponent));
			renderManager->LoadStaticMeshComponent(modelPath3, StaticMeshComponents.back().get());
			floor->AddComponent(EComponentType::StaticMeshComponent, meshStartIndex + i);

			MaterialComponents.emplace_back(std::make_shared<SMaterialComponent>(floor, EComponentType::MaterialComponent));
			renderManager->LoadMaterialComponent(materialNames3, MaterialComponents.back().get());
			floor->AddComponent(EComponentType::MaterialComponent, meshStartIndex + i);
		}
		// === !Other Wall ===
	}
}
