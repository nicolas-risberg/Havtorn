// Copyright 2022 Team Havtorn. All Rights Reserved.

#pragma once
#include "ECS/Component.h"

namespace Havtorn
{
	struct SPointLightComponent : public SComponent
	{
		SPointLightComponent(Ref<SEntity> entity, EComponentType type)
			: SComponent(std::move(entity), type)
		{}


	};
}
