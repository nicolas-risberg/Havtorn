// Copyright 2022 Team Havtorn. All Rights Reserved.

#pragma once
#include "Entity.h"

namespace Havtorn
{
	struct SEntity;

	struct SComponent
	{
		explicit SComponent(Ref<SEntity> entity, EComponentType type)
			: Entity(entity)
			, Type(type)
		{}
		virtual ~SComponent() = default;

		const Ref<SEntity> Entity;
		const EComponentType Type;
	};
}
