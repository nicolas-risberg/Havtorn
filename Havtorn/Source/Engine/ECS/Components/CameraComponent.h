// Copyright 2022 Team Havtorn. All Rights Reserved.

#pragma once
#include "ECS/Component.h"

namespace Havtorn
{
	struct SCameraComponent : public SComponent
	{
		SCameraComponent(Ref<SEntity> entity, EComponentType type)
			: SComponent(std::move(entity), type)
		{}

		SMatrix ViewMatrix;
		SMatrix ProjectionMatrix;
		F32 FOV = 70.0f;
		F32 AspectRatio = (16.0f / 9.0f);
		F32 NearClip = 0.1f;
		F32 FarClip = 1000.0f;
	};
}
