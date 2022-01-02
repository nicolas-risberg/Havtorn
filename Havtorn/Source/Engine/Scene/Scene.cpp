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

	bool CScene::Init()
	{
		// Setup systems
		Systems.emplace_back(std::make_unique<CRenderSystem>());
		
		// Create entities
		Entities.emplace_back(std::make_unique<SEntity>(1, "Camera"));

		// Setup entities (create components)
		TransformComponents.emplace_back(std::make_unique<STransformComponent>(*Entities.back(), EComponentType::TransformComponent));
		Entities.back()->AddComponent(EComponentType::TransformComponent, 0);
		
		RenderComponents.emplace_back(std::make_unique<SRenderComponent>(*Entities.back(), EComponentType::RenderComponent));
		Entities.back()->AddComponent(EComponentType::RenderComponent, 0);
		
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
