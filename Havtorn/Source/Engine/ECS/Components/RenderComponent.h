#pragma once
#include "ECS/Component.h"

namespace Havtorn
{
	struct SRenderComponent : public SComponent
	{
		SRenderComponent(Ref<SEntity> entity, EComponentType type)
			: SComponent(entity, type) 
		{}

		U64 MaterialRef;
	};
}