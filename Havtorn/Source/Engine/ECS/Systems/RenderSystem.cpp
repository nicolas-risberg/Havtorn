#include "RenderSystem.h"
#include "Scene/Scene.h"
#include "ECS/Entity.h"
#include "ECS/Components/TransformComponent.h"
#include "ECS/Components/RenderComponent.h"

namespace Havtorn
{
	CRenderSystem::CRenderSystem()
		: CSystem()
	{
	}

	CRenderSystem::~CRenderSystem()
	{
	}

	void CRenderSystem::Update(CScene* scene)
	{
		//auto& transformComponents = scene->GetTransformComponents();
		auto& renderComponents = scene->GetRenderComponents();

		for (auto& renderComp : renderComponents)
		{
			if (!renderComp->Entity.HasComponent(EComponentType::TransformComponent))
				continue;

			//I64 transformCompIndex = renderComp.Entity.GetComponentIndex(EComponentType::TransformComponent);
			//auto& transformComp = transformComponents[transformCompIndex];
		}
	}
}