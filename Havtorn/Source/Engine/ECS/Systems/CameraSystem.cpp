// Copyright 2022 Team Havtorn. All Rights Reserved.

#include "hvpch.h"
#include "CameraSystem.h"
#include "Scene/Scene.h"
#include "ECS/Components/TransformComponent.h"
#include "ECS/Components/CameraComponent.h"
#include "Input/Input.h"
#include "Input/InputMapper.h"
#include "Engine.h"

namespace Havtorn
{
	CCameraSystem::CCameraSystem()
		: CSystem()
		, CameraMoveInput(SVector::Zero)
		, CameraRotateInput(SVector::Zero)
	{
		// TODO.NR: Gather these together when the axis delegate broadcasts a payload with all necessary info
		CEngine::GetInstance()->GetInput()->GetAxisDelegate(EInputAxisEvent::Up).AddMember(this, &CCameraSystem::MoveUp);
		CEngine::GetInstance()->GetInput()->GetAxisDelegate(EInputAxisEvent::Right).AddMember(this, &CCameraSystem::MoveRight);
		CEngine::GetInstance()->GetInput()->GetAxisDelegate(EInputAxisEvent::Forward).AddMember(this, &CCameraSystem::MoveForward);
		CEngine::GetInstance()->GetInput()->GetAxisDelegate(EInputAxisEvent::Pitch).AddMember(this, &CCameraSystem::RotatePitch);
		CEngine::GetInstance()->GetInput()->GetAxisDelegate(EInputAxisEvent::Yaw).AddMember(this, &CCameraSystem::RotateYaw);
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

	void CCameraSystem::MoveUp(F32 value)
	{
		CameraMoveInput += SVector::Up * value * 10.0f;
	}

	void CCameraSystem::MoveRight(F32 value)
	{
		CameraMoveInput += SVector::Right * value * 10.0f;
	}

	void CCameraSystem::MoveForward(F32 value)
	{
		CameraMoveInput += SVector::Forward * value * 10.0f;
	}

	void CCameraSystem::RotatePitch(F32 value)
	{
		CameraRotateInput.X += UMath::DegToRad(90.0f) * value;
	}

	void CCameraSystem::RotateYaw(F32 value)
	{
		CameraRotateInput.Y += UMath::DegToRad(90.0f) * value;
	}
}