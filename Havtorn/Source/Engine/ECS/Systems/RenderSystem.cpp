#include "RenderSystem.h"
#include "Scene/Scene.h"
#include "ECS/Entity.h"
#include "ECS/Components/TransformComponent.h"
#include "ECS/Components/RenderComponent.h"
#include "ECS/Components/CameraComponent.h"
#include "Graphics/RenderManager.h"
#include "Graphics/RenderCommand.h"

namespace Havtorn
{
	CRenderSystem::CRenderSystem(CRenderManager* renderManager)
		: CSystem()
		, RenderManager(renderManager)
	{
	}

	CRenderSystem::~CRenderSystem()
	{
	}

	void CRenderSystem::Update(CScene* scene)
	{
		auto& renderComponents = scene->GetRenderComponents();
		auto& transformComponents = scene->GetTransformComponents();
		auto& cameraComponents = scene->GetCameraComponents();

		if (!cameraComponents.empty())
		{
			I64 transformCompIndex = cameraComponents[0]->Entity->GetComponentIndex(EComponentType::TransformComponent);
			auto& transformComp = transformComponents[transformCompIndex];
			
			SRenderCommand command({ transformComp, cameraComponents[0] }, ERenderCommandType::CameraDataStorage);
			RenderManager->PushRenderCommand(command);
		}
		else
			return;

		for (auto& renderComp : renderComponents)
		{
			if (!renderComp->Entity->HasComponent(EComponentType::TransformComponent))
				continue;

			I64 transformCompIndex = renderComp->Entity->GetComponentIndex(EComponentType::TransformComponent);
			auto& transformComp = transformComponents[transformCompIndex];

			SRenderCommand command({ transformComp, renderComp }, ERenderCommandType::GBufferData);
			RenderManager->PushRenderCommand(command);
		}
	}
}