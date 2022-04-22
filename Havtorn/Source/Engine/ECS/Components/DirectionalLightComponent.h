// Copyright 2022 Team Havtorn. All Rights Reserved.

#pragma once
#include "ECS/Component.h"

namespace Havtorn
{
	struct SDirectionalLightComponent : public SComponent
	{
		SDirectionalLightComponent(Ref<SEntity> entity, EComponentType type)
			: SComponent(std::move(entity), type)
		{}

		SMatrix ShadowViewMatrix = SMatrix::Identity;
		SMatrix ShadowProjectionMatrix = SMatrix::Identity;
		SVector4 ShadowPosition = SVector4::Zero;
		SVector4 Direction = SVector4::Up;
		SVector4 Color = SVector4(1.0f, 1.0f, 1.0f, 1.0f);
	};
}