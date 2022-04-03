// Copyright 2022 Team Havtorn. All Rights Reserved.

#pragma once
#include "ECS/Component.h"
#include "Graphics/GraphicsStructs.h"

namespace Havtorn
{
	struct SStaticMeshComponent : public SComponent
	{
		SStaticMeshComponent(Ref<SEntity> entity, EComponentType type)
			: SComponent(std::move(entity), type)
		{}

		// Asset Data
		std::vector<SDrawCallData> DrawCallData;

		// Render Data
		U8 VertexShaderIndex = 0;
		U8 InputLayoutIndex = 0;
		U8 PixelShaderIndex = 0;
		U8 SamplerIndex = 0;
		U8 TopologyIndex = 0;
	};
}