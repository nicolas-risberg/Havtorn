// Copyright 2022 Team Havtorn. All Rights Reserved.

#pragma once
#include "ECS/Component.h"

namespace Havtorn
{
	struct SRenderComponent : public SComponent
	{
		SRenderComponent(Ref<SEntity> entity, EComponentType type)
			: SComponent(std::move(entity), type) 
			, MaterialRef(0)
		{}

		void InspectInEditor() override;

		U64 MaterialRef;
	};
}