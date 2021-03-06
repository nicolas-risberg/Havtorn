// Copyright 2022 Team Havtorn. All Rights Reserved.

#include "RenderSystem.h"
#include "Scene/Scene.h"
#include "ECS/ECSInclude.h"
#include "Graphics/RenderManager.h"
#include "Graphics/RenderCommand.h"
#include "Input/Input.h"

namespace Havtorn
{
	CRenderSystem::CRenderSystem(CRenderManager* renderManager)
		: CSystem()
		, RenderManager(renderManager)
	{
	}

	void CRenderSystem::Update(CScene* scene)
	{
		const auto& staticMeshComponents = scene->GetStaticMeshComponents();
		const auto& transformComponents = scene->GetTransformComponents();
		const auto& cameraComponents = scene->GetCameraComponents();
		const auto& materialComponents = scene->GetMaterialComponents();
		const auto& directionalLightComponents = scene->GetDirectionalLightComponents();
		const auto& pointLightComponents = scene->GetPointLightComponents();
		const auto& spotLightComponents = scene->GetSpotLightComponents();

		if (!cameraComponents.empty())
		{
			const I64 transformCompIndex = cameraComponents[0]->Entity->GetComponentIndex(EComponentType::TransformComponent);
			auto& transformComp = transformComponents[transformCompIndex];

			std::array<Ref<SComponent>, static_cast<size_t>(EComponentType::Count)> components;
			components[static_cast<U8>(EComponentType::TransformComponent)] = transformComp;
			components[static_cast<U8>(EComponentType::CameraComponent)] = cameraComponents[0];
			SRenderCommand command(components, ERenderCommandType::CameraDataStorage);
			RenderManager->PushRenderCommand(command);
		}
		else
			return;

		for (auto& staticMeshComponent : staticMeshComponents)
		{
			if (!staticMeshComponent->Entity->HasComponent(EComponentType::TransformComponent))
				continue;

			if (!staticMeshComponent->Entity->HasComponent(EComponentType::MaterialComponent))
				continue;

			const I64 transformCompIndex = staticMeshComponent->Entity->GetComponentIndex(EComponentType::TransformComponent);
			auto& transformComp = transformComponents[transformCompIndex];

			const I64 materialCompIndex = staticMeshComponent->Entity->GetComponentIndex(EComponentType::MaterialComponent);
			auto& materialComp = materialComponents[materialCompIndex];

			const F32 dt = CTimer::Dt();
			if (CInput::GetInstance()->IsKeyPressed('J'))
				transformComp->Transform.Rotate({ UMath::DegToRad(90.0f) * dt, 0.0f, 0.0f });
			if (CInput::GetInstance()->IsKeyPressed('K'))
				transformComp->Transform.Rotate({ 0.0f, UMath::DegToRad(90.0f) * dt, 0.0f });
			if (CInput::GetInstance()->IsKeyPressed('L'))
				transformComp->Transform.Rotate({ 0.0f, 0.0f, UMath::DegToRad(90.0f) * dt });

			//transformComp->Transform.Orbit({ 0.0f, 0.0f, 0.0f }, SMatrix::CreateRotationAroundY(UMath::DegToRad(90.0f) * dt));

			if (!directionalLightComponents.empty())
			{
				std::array<Ref<SComponent>, static_cast<size_t>(EComponentType::Count)> components;
				components[static_cast<U8>(EComponentType::TransformComponent)] = transformComp;
				components[static_cast<U8>(EComponentType::StaticMeshComponent)] = staticMeshComponent;
				components[static_cast<U8>(EComponentType::DirectionalLightComponent)] = directionalLightComponents[0];
				SRenderCommand command(components, ERenderCommandType::ShadowAtlasPrePassDirectional);
				RenderManager->PushRenderCommand(command);
			}

			if (!pointLightComponents.empty())
			{
				std::array<Ref<SComponent>, static_cast<size_t>(EComponentType::Count)> components;
				components[static_cast<U8>(EComponentType::TransformComponent)] = transformComp;
				components[static_cast<U8>(EComponentType::StaticMeshComponent)] = staticMeshComponent;
				components[static_cast<U8>(EComponentType::PointLightComponent)] = pointLightComponents[0];
				SRenderCommand command(components, ERenderCommandType::ShadowAtlasPrePassPoint);
				RenderManager->PushRenderCommand(command);
			}

			if (!spotLightComponents.empty())
			{
				std::array<Ref<SComponent>, static_cast<size_t>(EComponentType::Count)> components;
				components[static_cast<U8>(EComponentType::TransformComponent)] = transformComp;
				components[static_cast<U8>(EComponentType::StaticMeshComponent)] = staticMeshComponent;
				components[static_cast<U8>(EComponentType::SpotLightComponent)] = spotLightComponents[0];
				SRenderCommand command(components, ERenderCommandType::ShadowAtlasPrePassSpot);
				RenderManager->PushRenderCommand(command);
			}

			std::array<Ref<SComponent>, static_cast<size_t>(EComponentType::Count)> components;
			components[static_cast<U8>(EComponentType::TransformComponent)] = transformComp;
			components[static_cast<U8>(EComponentType::StaticMeshComponent)] = staticMeshComponent;
			components[static_cast<U8>(EComponentType::MaterialComponent)] = materialComp;
			SRenderCommand command(components, ERenderCommandType::GBufferData);
			RenderManager->PushRenderCommand(command);
		}

		{
			SRenderCommand command(std::array<Ref<SComponent>, static_cast<size_t>(EComponentType::Count)>{}, ERenderCommandType::PreLightingPass);
			RenderManager->PushRenderCommand(command);
		}

		if (!directionalLightComponents.empty())
		{
			const I64 transformCompIndex = directionalLightComponents[0]->Entity->GetComponentIndex(EComponentType::TransformComponent);
			auto& transformComp = transformComponents[transformCompIndex];

			std::array<Ref<SComponent>, static_cast<size_t>(EComponentType::Count)> components;
			components[static_cast<U8>(EComponentType::TransformComponent)] = transformComp;
			components[static_cast<U8>(EComponentType::DirectionalLightComponent)] = directionalLightComponents[0];
			SRenderCommand command(components, ERenderCommandType::DeferredLightingDirectional);
			RenderManager->PushRenderCommand(command);
		}

		if (!pointLightComponents.empty())
		{
			const I64 transformCompIndex = pointLightComponents[0]->Entity->GetComponentIndex(EComponentType::TransformComponent);
			auto& transformComp = transformComponents[transformCompIndex];

			std::array<Ref<SComponent>, static_cast<size_t>(EComponentType::Count)> components;
			components[static_cast<U8>(EComponentType::TransformComponent)] = transformComp;
			components[static_cast<U8>(EComponentType::PointLightComponent)] = pointLightComponents[0];
			SRenderCommand command(components, ERenderCommandType::DeferredLightingPoint);
			RenderManager->PushRenderCommand(command);
		}

		if (!spotLightComponents.empty())
		{
			const I64 transformCompIndex = spotLightComponents[0]->Entity->GetComponentIndex(EComponentType::TransformComponent);
			auto& transformComp = transformComponents[transformCompIndex];

			std::array<Ref<SComponent>, static_cast<size_t>(EComponentType::Count)> components;
			components[static_cast<U8>(EComponentType::TransformComponent)] = transformComp;
			components[static_cast<U8>(EComponentType::SpotLightComponent)] = spotLightComponents[0];
			SRenderCommand command(components, ERenderCommandType::DeferredLightingSpot);
			RenderManager->PushRenderCommand(command);
		}
	}
}
