#pragma once
#include "Entity.h"

namespace Havtorn
{
	struct SEntity;

	struct SComponent
	{
		explicit SComponent(SEntity entity, EComponentType type)
			: Entity(entity)
			, Type(type)
		{}

		const SEntity& Entity;
		const EComponentType Type;
	};
}
