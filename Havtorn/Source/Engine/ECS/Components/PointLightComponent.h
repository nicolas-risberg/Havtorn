// Copyright 2022 Team Havtorn. All Rights Reserved.

#pragma once
#include "ECS/Component.h"
#include "Graphics/GraphicsStructs.h"

namespace Havtorn
{
	struct SPointLightComponent : public SComponent
	{
		SPointLightComponent(Ref<SEntity> entity, EComponentType type)
			: SComponent(std::move(entity), type)
		{}

		void InspectInEditor() override;

		std::array<SShadowmapViewData, 6> ShadowmapViews = {};
		SVector4 ColorAndIntensity = SVector4(1.0f, 1.0f, 1.0f, 1.0f);
		F32 Range = 1.0f;
	};
}
