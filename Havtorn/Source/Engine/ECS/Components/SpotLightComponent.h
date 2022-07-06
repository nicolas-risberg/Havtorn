// Copyright 2022 Team Havtorn. All Rights Reserved.

#pragma once
#include "ECS/Component.h"
#include "Graphics/GraphicsStructs.h"

namespace Havtorn
{
	struct SSpotLightComponent : public SComponent
	{
		SSpotLightComponent(Ref<SEntity> entity, EComponentType type)
			: SComponent(std::move(entity), type)
		{}

		SShadowmapViewData ShadowmapView = {};
		SVector4 ColorAndIntensity = SVector4(1.0f, 1.0f, 1.0f, 1.0f);
		SVector4 Direction = SVector4::Forward;
		SVector4 DirectionNormal1 = SVector4::Right;
		SVector4 DirectionNormal2 = SVector4::Up;
		SVector2<F32> ShadowmapResolution = { 8192.0f, 8192.0f };
		SVector2<F32> ShadowViewSize = { 8.0f, 8.0f };
		F32 OuterAngle = 90.0f;
		F32 InnerAngle = 45.0f;
		F32 AngleExponent = 1.0f;
		F32 Range = 1.0f;
	};
}
