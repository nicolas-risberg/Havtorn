// Copyright 2022 Team Havtorn. All Rights Reserved.

#include "LightSystem.h"
#include "Scene/Scene.h"
#include "ECS/Components/DirectionalLightComponent.h"
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
		directionalLightComp->ShadowmapView.ShadowPosition.Y = 2.0f;
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

		directionalLightComp->ShadowmapView.ShadowProjectionMatrix = SMatrix::OrthographicLH(directionalLightComp->ShadowViewSize.X, directionalLightComp->ShadowViewSize.Y, -10.0f, 10.0f);
	}
}
