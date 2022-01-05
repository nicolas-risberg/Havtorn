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
		Systems.emplace_back(std::make_unique<CRenderSystem>(renderManager));
		
		// Create entities
		Entities.emplace_back(std::make_shared<SEntity>(1, "Camera"));
		auto cameraEntity = Entities[0];
		Entities.emplace_back(std::make_shared<SEntity>(2, "Cube"));
		auto cubeEntity = Entities.back();

		// Setup entities (create components)
		TransformComponents.emplace_back(std::make_shared<STransformComponent>(cameraEntity, EComponentType::TransformComponent));
		cameraEntity->AddComponent(EComponentType::TransformComponent, 0);
		
		TransformComponents.emplace_back(std::make_shared<STransformComponent>(cubeEntity, EComponentType::TransformComponent));
		cubeEntity->AddComponent(EComponentType::TransformComponent, 1);

		CameraComponents.emplace_back(std::make_shared<SCameraComponent>(cameraEntity, EComponentType::CameraComponent));
		cameraEntity->AddComponent(EComponentType::CameraComponent, 0);
		CameraComponents.back()->ProjectionMatrix = SMatrix::PerspectiveFovLH(UMath::DegToRad(70.0f), (16.0f / 9.0f), 0.1f, 1000.0f);
		CameraComponents.back()->ViewMatrix = SMatrix::LookAtLH(SVector::Zero, SVector::Forward, SVector::Up);

		RenderComponents.emplace_back(std::make_shared<SRenderComponent>(cubeEntity, EComponentType::RenderComponent));
		cubeEntity->AddComponent(EComponentType::RenderComponent, 0);
		RenderComponents.back()->MaterialRef = 0;
	
		auto id = CameraComponents[0]->Entity->ID;
		id = 1;

		return true;
	}

	void CScene::Update()
	{
		for (auto& system : Systems)
		{
			system->Update(this);
		}
	}
}
