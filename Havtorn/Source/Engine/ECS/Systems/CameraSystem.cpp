// Copyright 2022 Team Havtorn. All Rights Reserved.

#include "hvpch.h"
#include "CameraSystem.h"
#include "Engine.h"
#include "Scene/Scene.h"
#include "ECS/Components/TransformComponent.h"
#include "ECS/Components/CameraComponent.h"
#include "Input/InputMapper.h"

namespace Havtorn
{
	CCameraSystem::CCameraSystem()
		: CSystem()
		, CameraMoveInput(SVector::Zero)
		, CameraRotateInput(SVector::Zero)
	{
		CEngine::GetInstance()->GetInput()->GetAxisDelegate(EInputAxisEvent::Up).AddMember(this, &CCameraSystem::HandleAxisInput);
		CEngine::GetInstance()->GetInput()->GetAxisDelegate(EInputAxisEvent::Right).AddMember(this, &CCameraSystem::HandleAxisInput);
		CEngine::GetInstance()->GetInput()->GetAxisDelegate(EInputAxisEvent::Forward).AddMember(this, &CCameraSystem::HandleAxisInput);
		CEngine::GetInstance()->GetInput()->GetAxisDelegate(EInputAxisEvent::Pitch).AddMember(this, &CCameraSystem::HandleAxisInput);
		CEngine::GetInstance()->GetInput()->GetAxisDelegate(EInputAxisEvent::Yaw).AddMember(this, &CCameraSystem::HandleAxisInput);
	}

	CCameraSystem::~CCameraSystem()
	{
	}

	void CCameraSystem::Update(CScene* scene)
	{
		auto& transformComponents = scene->GetTransformComponents();
		auto& cameraComponents = scene->GetCameraComponents();
		if (cameraComponents.empty())
			return;

		I64 transformCompIndex = cameraComponents[0]->Entity->GetComponentIndex(EComponentType::TransformComponent);
		auto& transformComp = transformComponents[transformCompIndex];

		const F32 dt = CTimer::Dt();
		transformComp->Transform.Translate(CameraMoveInput * dt);
		transformComp->Transform.Rotate(CameraRotateInput * dt);

		CameraMoveInput = SVector::Zero;
		CameraRotateInput = SVector::Zero;
	}

	void CCameraSystem::HandleAxisInput(const SInputAxisPayload payload)
	{
		switch (payload.Event)
		{
			case EInputAxisEvent::Right: 
				CameraMoveInput += SVector::Right * payload.AxisValue * 10.0f;
				return;
			case EInputAxisEvent::Up:
				CameraMoveInput += SVector::Up * payload.AxisValue * 10.0f;
				return;
			case EInputAxisEvent::Forward:
				CameraMoveInput += SVector::Forward * payload.AxisValue * 10.0f;
				return;
			case EInputAxisEvent::Pitch:
				CameraRotateInput.X += UMath::DegToRad(90.0f) * payload.AxisValue;
				return;
			case EInputAxisEvent::Yaw:
				CameraRotateInput.Y += UMath::DegToRad(90.0f) * payload.AxisValue;
				return;
			default: 
				return;
		}
	}
}
