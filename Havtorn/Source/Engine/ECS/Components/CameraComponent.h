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

		void InspectInEditor() override;

		SMatrix ViewMatrix;
		SMatrix ProjectionMatrix;
	};
}
