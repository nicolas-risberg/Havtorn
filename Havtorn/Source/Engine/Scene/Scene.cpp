// Copyright 2022 Team Havtorn. All Rights Reserved.

#include "Scene.h"

#include "ECS/ECSInclude.h"

namespace Havtorn
{
	//CScene::CScene()
	//	: Entities()
	//	, TransformComponents()
	//	, RenderComponents()
	//	, Systems()
	//{}

	CScene::~CScene()
	{
	}

	bool CScene::Init(CRenderManager* renderManager)
	{
		// Setup systems
		Systems.emplace_back(std::make_unique<CCameraSystem>());
		Systems.emplace_back(std::make_unique<CRenderSystem>(renderManager));

		// Create entities
		Entities.emplace_back(std::make_shared<SEntity>(1, "Camera"));
		auto cameraEntity = Entities[0];

		// Setup entities (create components)
		TransformComponents.emplace_back(std::make_shared<STransformComponent>(cameraEntity, EComponentType::TransformComponent));
		cameraEntity->AddComponent(EComponentType::TransformComponent, 0);
		TransformComponents.back()->Transform.Translation({ 0.0f, 0.0f, -5.0f });

		CameraComponents.emplace_back(std::make_shared<SCameraComponent>(cameraEntity, EComponentType::CameraComponent));
		cameraEntity->AddComponent(EComponentType::CameraComponent, 0);
		CameraComponents.back()->ProjectionMatrix = SMatrix::PerspectiveFovLH(UMath::DegToRad(70.0f), (16.0f / 9.0f), 0.1f, 1000.0f);
		CameraComponents.back()->ViewMatrix = SMatrix::LookAtLH(SVector::Zero, SVector::Forward, SVector::Up);

		InitDemoScene();

		return true;
	}

	void CScene::Update()
	{
		for (auto& system : Systems)
		{
			system->Update(this);
		}
	}

	// TODO.NR: Make primitive class containing verts (static getters for bindables?)
	void CScene::InitDemoScene()
	{
		constexpr U8 cubeNumber = 20;

		const SVector minTranslation = { -4.0f, -4.0f, -4.0f };
		const SVector maxTranslation = { 4.0f, 4.0f, 4.0f };
		const SVector minEulerRotation = { UMath::DegToRad(-90.0f), UMath::DegToRad(-90.0f), UMath::DegToRad(-90.0f) };
		const SVector maxEulerRotation = { UMath::DegToRad(90.0f), UMath::DegToRad(90.0f), UMath::DegToRad(90.0f) };

		for (U8 i = 0; i <= cubeNumber; ++i)
		{
			U64 newID = Entities.back()->ID + 1;
			Entities.emplace_back(std::make_shared<SEntity>(newID, "Cube"));
			auto cubeEntity = Entities.back();

			TransformComponents.emplace_back(std::make_shared<STransformComponent>(cubeEntity, EComponentType::TransformComponent));
			cubeEntity->AddComponent(EComponentType::TransformComponent, i + 1);
			auto& transform = TransformComponents.back()->Transform;
			transform.Translation(SVector::Random(minTranslation, maxTranslation));
			transform.Rotate(SVector::Random(minEulerRotation, maxEulerRotation));

			RenderComponents.emplace_back(std::make_shared<SRenderComponent>(cubeEntity, EComponentType::RenderComponent));
			cubeEntity->AddComponent(EComponentType::RenderComponent, i);
			RenderComponents.back()->MaterialRef = 0;
		}
	}
}
