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

		const Ref<SEntity> Entity;
		const EComponentType Type;
	};
}
