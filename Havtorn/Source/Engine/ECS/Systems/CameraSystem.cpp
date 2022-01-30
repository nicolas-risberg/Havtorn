// Copyright 2022 Team Havtorn. All Rights Reserved.

#include "hvpch.h"
#include "CameraSystem.h"
#include "Scene/Scene.h"
#include "ECS/Components/TransformComponent.h"
#include "ECS/Components/CameraComponent.h"
#include "Input/Input.h"

Havtorn::CCameraSystem::CCameraSystem()
	: CSystem()
{
}

Havtorn::CCameraSystem::~CCameraSystem()
{
}

void Havtorn::CCameraSystem::Update(CScene* scene)
{
	auto& transformComponents = scene->GetTransformComponents();
	auto& cameraComponents = scene->GetCameraComponents();
	if (cameraComponents.empty())
		return;

	I64 transformCompIndex = cameraComponents[0]->Entity->GetComponentIndex(EComponentType::TransformComponent);
	auto& transformComp = transformComponents[transformCompIndex];

	F32 dt = CTimer::Dt();
	if (CInput::GetInstance()->IsKeyPressed('W'))
		transformComp->Transform.Translate(SVector::Forward * dt);
	if (CInput::GetInstance()->IsKeyPressed('S'))
		transformComp->Transform.Translate(SVector::Backward * dt);
	if (CInput::GetInstance()->IsKeyPressed('A'))
		transformComp->Transform.Translate(SVector::Left * dt);
	if (CInput::GetInstance()->IsKeyPressed('D'))
		transformComp->Transform.Translate(SVector::Right * dt);
	if (CInput::GetInstance()->IsKeyPressed('Q'))
		transformComp->Transform.Translate(SVector::Up * dt);
	if (CInput::GetInstance()->IsKeyPressed('E'))
		transformComp->Transform.Translate(SVector::Down * dt);

	F32 yaw = 0.0f, pitch = 0.0f;
	if (CInput::GetInstance()->IsKeyPressed(VK_UP))
		pitch = UMath::DegToRad(90.0f) * dt;
	if (CInput::GetInstance()->IsKeyPressed(VK_DOWN))
		pitch = UMath::DegToRad(-90.0f) * dt;
	if (CInput::GetInstance()->IsKeyPressed(VK_LEFT))
		yaw = UMath::DegToRad(90.0f) * dt;
	if (CInput::GetInstance()->IsKeyPressed(VK_RIGHT))
		yaw = UMath::DegToRad(-90.0f) * dt;

	transformComp->Transform.Rotate({ pitch, yaw, 0.0f });
}