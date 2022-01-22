// Copyright 2022 Team Havtorn. All Rights Reserved.

#include "hvpch.h"
#include "CameraSystem.h"
#include "Scene/Scene.h"
#include "ECS/Components/TransformComponent.h"
#include "ECS/Components/CameraComponent.h"

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
	if (GetAsyncKeyState('W'))
		transformComp->Transform.Translate(SVector::Forward * dt);
	if (GetAsyncKeyState('S'))
		transformComp->Transform.Translate(SVector::Backward * dt);
	if (GetAsyncKeyState('A'))
		transformComp->Transform.Translate(SVector::Left * dt);
	if (GetAsyncKeyState('D'))
		transformComp->Transform.Translate(SVector::Right * dt);
	if (GetAsyncKeyState('Q'))
		transformComp->Transform.Translate(SVector::Up * dt);
	if (GetAsyncKeyState('E'))
		transformComp->Transform.Translate(SVector::Down * dt);

	F32 yaw = 0.0f, pitch = 0.0f;
	if (GetAsyncKeyState(VK_UP))
		pitch = UMath::DegToRad(90.0f) * dt;
	if (GetAsyncKeyState(VK_DOWN))
		pitch = UMath::DegToRad(-90.0f) * dt;
	if (GetAsyncKeyState(VK_LEFT))
		yaw = UMath::DegToRad(90.0f) * dt;
	if (GetAsyncKeyState(VK_RIGHT))
		yaw = UMath::DegToRad(-90.0f) * dt;

	transformComp->Transform.Rotate({ pitch, yaw, 0.0f });
}