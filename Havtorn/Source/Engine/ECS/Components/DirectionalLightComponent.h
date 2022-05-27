// Copyright 2022 Team Havtorn. All Rights Reserved.

#pragma once
#include "ECS/Component.h"
#include "Graphics/GraphicsStructs.h"

namespace Havtorn
{
	struct SDirectionalLightComponent : public SComponent
	{
		SDirectionalLightComponent(Ref<SEntity> entity, EComponentType type)
			: SComponent(std::move(entity), type)
		{}

		SShadowmapViewData ShadowmapView = SShadowmapViewData();
		SVector4 Direction = SVector4::Up;
		SVector4 Color = SVector4(1.0f, 1.0f, 1.0f, 1.0f);
		SVector2<F32> ShadowmapResolution = { 1024.0f, 1024.0f };
		SVector2<F32> ShadowViewSize = { 32.0f, 32.0f };
	};
}