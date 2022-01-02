#pragma once
#include "ECS/Component.h"

namespace Havtorn
{
	struct STransformComponent : public SComponent
	{
		STransformComponent(SEntity entity, EComponentType type)
			: SComponent(entity, type) {}

		SMatrix Transform;
	};
}