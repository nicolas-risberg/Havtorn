// Copyright 2022 Team Havtorn. All Rights Reserved.

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

			std::array<Ref<SComponent>, static_cast<size_t>(EComponentType::Count)> components;
			components[static_cast<U8>(EComponentType::TransformComponent)] = transformComp;
			components[static_cast<U8>(EComponentType::CameraComponent)] = cameraComponents[0];
			SRenderCommand command(components, ERenderCommandType::CameraDataStorage);
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

			F32 dt = CTimer::Dt();
			if (GetAsyncKeyState('J'))
				transformComp->Transform.Rotate({ UMath::DegToRad(90.0f) * dt, 0.0f, 0.0f });
			if (GetAsyncKeyState('K'))
				transformComp->Transform.Rotate({ 0.0f, UMath::DegToRad(90.0f) * dt, 0.0f });
			if (GetAsyncKeyState('L'))
				transformComp->Transform.Rotate({ 0.0f, 0.0f, UMath::DegToRad(90.0f) * dt });

			transformComp->Transform.Orbit({ 0.0f, 0.0f, 0.0f }, SMatrix::CreateRotationAroundY(UMath::DegToRad(90.0f) * dt));

			std::array<Ref<SComponent>, static_cast<size_t>(EComponentType::Count)> components;
			components[static_cast<U8>(EComponentType::TransformComponent)] = transformComp;
			components[static_cast<U8>(EComponentType::RenderComponent)] = renderComp;
			SRenderCommand command(components, ERenderCommandType::GBufferData);
			RenderManager->PushRenderCommand(command);
		}
	}
}