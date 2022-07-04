// Copyright 2022 Team Havtorn. All Rights Reserved.

#include "LightSystem.h"
#include "Scene/Scene.h"
#include "ECS/Components/DirectionalLightComponent.h"
#include "ECS/Components/PointLightComponent.h"
#include "ECS/Components/SpotLightComponent.h"
#include "ECS/Components/TransformComponent.h"
#include "ECS/Components/CameraComponent.h"

namespace Havtorn
{
	CLightSystem::CLightSystem()
		: CSystem()
	{}

	void CLightSystem::Update(CScene* scene)
	{
		const auto& cameraComponents = scene->GetCameraComponents();
		const auto& transformComponents = scene->GetTransformComponents();
		const auto& directionalLightComponents = scene->GetDirectionalLightComponents();

		if (directionalLightComponents.empty())
			return;

		auto& directionalLightComp = directionalLightComponents[0];
		auto& mainCameraComp = cameraComponents[0];
		const I64 transformCompIndex = mainCameraComp->Entity->GetComponentIndex(EComponentType::TransformComponent);
		auto& transformComp = transformComponents[transformCompIndex];

		directionalLightComp->ShadowmapView.ShadowPosition = transformComp->Transform.GetMatrix().Translation4();
		directionalLightComp->ShadowmapView.ShadowPosition.Y = 4.0f;
		static float counter = 0.0f;
		counter += CTimer::Dt();
		directionalLightComp->Direction = { -1.0f/*UMath::Sin(counter)*/, 1.0f, -1.0f, 0.0f };

		// Round to pixel positions
		SVector position = { directionalLightComp->ShadowmapView.ShadowPosition.X, directionalLightComp->ShadowmapView.ShadowPosition.Y, directionalLightComp->ShadowmapView.ShadowPosition.Z };
		const SVector2<F32> unitsPerPixel = directionalLightComp->ShadowViewSize / directionalLightComp->ShadowmapResolution;

		auto shadowTransform = SMatrix();
		const F32 radiansY = atan2(-directionalLightComp->Direction.X, -directionalLightComp->Direction.Z);
		const F32 l = sqrt(directionalLightComp->Direction.Z * directionalLightComp->Direction.Z + directionalLightComp->Direction.X * directionalLightComp->Direction.X);
		const F32 radiansX = atan2(directionalLightComp->Direction.Y, l);
		shadowTransform *= SMatrix::CreateRotationAroundY(radiansY);
		shadowTransform *= SMatrix::CreateRotationFromAxisAngle(shadowTransform.Right(), radiansX);

		F32 rightStep = position.Dot(shadowTransform.Right());
		position -= rightStep * shadowTransform.Right();
		rightStep = floor(rightStep / unitsPerPixel.X) * unitsPerPixel.X;
		position += rightStep * shadowTransform.Right();

		F32 upStep = position.Dot(shadowTransform.Up());
		position -= upStep * shadowTransform.Up();
		upStep = floor(upStep / unitsPerPixel.Y) * unitsPerPixel.Y;
		position += upStep * shadowTransform.Up();

		directionalLightComp->ShadowmapView.ShadowPosition = SVector4(position.X, position.Y, position.Z, 1.0f);

		const SVector shadowDirection = { directionalLightComp->Direction.X, directionalLightComp->Direction.Y, directionalLightComp->Direction.Z };
		directionalLightComp->ShadowmapView.ShadowViewMatrix = SMatrix::LookAtLH(position, position - shadowDirection, SVector::Up);

		static F32 counter2 = 0.001f;
		const auto& pointLightComp = scene->GetPointLightComponents()[0];
		if (GetAsyncKeyState('O'))
			counter2 -= CTimer::Dt();
		if (GetAsyncKeyState('L'))
			counter2 += CTimer::Dt();

		SVector4 move = SVector4::Zero;
		F32 moveSpeed = 2.0f;
		if (GetAsyncKeyState('Y'))
			move += moveSpeed * SVector4::Forward * CTimer::Dt();
		if (GetAsyncKeyState('G'))
			move += moveSpeed * SVector4::Left * CTimer::Dt();
		if (GetAsyncKeyState('H'))
			move += moveSpeed * SVector4::Backward * CTimer::Dt();
		if (GetAsyncKeyState('J'))
			move += moveSpeed * SVector4::Right * CTimer::Dt();
		if (GetAsyncKeyState('I'))
			move += moveSpeed * SVector4::Up * CTimer::Dt();
		if (GetAsyncKeyState('K'))
			move += moveSpeed * SVector4::Down * CTimer::Dt();

		counter2 = UMath::Clamp(counter2, 0.001f);

		transformComponents[pointLightComp->Entity->GetComponentIndex(EComponentType::TransformComponent)]->Transform.Translate(move);
		SVector4 constantPosition = transformComponents[pointLightComp->Entity->GetComponentIndex(EComponentType::TransformComponent)]->Transform.GetMatrix().Translation4();

		const SMatrix constantProjectionMatrix = SMatrix::PerspectiveFovLH(UMath::DegToRad(90.0f), 1.0f, UMath::Abs(UMath::Sin(counter2)), pointLightComp->Range);

		// Forward
		SShadowmapViewData& view1 = pointLightComp->ShadowmapViews[0];
		view1.ShadowPosition = constantPosition;
		view1.ShadowmapViewportIndex = 1;
		view1.ShadowViewMatrix = SMatrix::LookAtLH(constantPosition.ToVector3(), (constantPosition + SVector4::Forward).ToVector3(), SVector::Up);
		view1.ShadowProjectionMatrix = constantProjectionMatrix;

		// Right
		SShadowmapViewData& view2 = pointLightComp->ShadowmapViews[1];
		view2.ShadowPosition = constantPosition;
		view2.ShadowmapViewportIndex = 2;
		view2.ShadowViewMatrix = SMatrix::LookAtLH(constantPosition.ToVector3(), (constantPosition + SVector4::Right).ToVector3(), SVector::Up);
		view2.ShadowProjectionMatrix = constantProjectionMatrix;

		// Backward
		SShadowmapViewData& view3 = pointLightComp->ShadowmapViews[2];
		view3.ShadowPosition = constantPosition;
		view3.ShadowmapViewportIndex = 3;
		view3.ShadowViewMatrix = SMatrix::LookAtLH(constantPosition.ToVector3(), (constantPosition + SVector4::Backward).ToVector3(), SVector::Up);
		view3.ShadowProjectionMatrix = constantProjectionMatrix;

		// Left
		SShadowmapViewData& view4 = pointLightComp->ShadowmapViews[3];
		view4.ShadowPosition = constantPosition;
		view4.ShadowmapViewportIndex = 4;
		view4.ShadowViewMatrix = SMatrix::LookAtLH(constantPosition.ToVector3(), (constantPosition + SVector4::Left).ToVector3(), SVector::Up);
		view4.ShadowProjectionMatrix = constantProjectionMatrix;

		// Up
		SShadowmapViewData& view5 = pointLightComp->ShadowmapViews[4];
		view5.ShadowPosition = constantPosition;
		view5.ShadowmapViewportIndex = 5;
		view5.ShadowViewMatrix = SMatrix::LookAtLH(constantPosition.ToVector3(), (constantPosition + SVector4::Up).ToVector3(), SVector::Backward);
		view5.ShadowProjectionMatrix = constantProjectionMatrix;

		// Down
		SShadowmapViewData& view6 = pointLightComp->ShadowmapViews[5];
		view6.ShadowPosition = constantPosition;
		view6.ShadowmapViewportIndex = 6;
		view6.ShadowViewMatrix = SMatrix::LookAtLH(constantPosition.ToVector3(), (constantPosition + SVector4::Down).ToVector3(), SVector::Forward);
		view6.ShadowProjectionMatrix = constantProjectionMatrix;
	}
}
