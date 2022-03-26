// Copyright 2022 Team Havtorn. All Rights Reserved.

#pragma once
#include "ECS/Component.h"

namespace Havtorn
{
	struct SStaticMeshComponent : public SComponent
	{
		SStaticMeshComponent(Ref<SEntity> entity, EComponentType type)
			: SComponent(std::move(entity), type)
		{}

		// Asset Data
		U32 IndexCount = 0;

		U16 VertexBufferIndex = 0;
		U16 IndexBufferIndex = 0;
		U16 VertexStrideIndex = 0;
		U16 VertexOffsetIndex = 0;

		// Render Data
		U8 VertexShaderIndex = 0;
		U8 InputLayoutIndex = 0;
		U8 PixelShaderIndex = 0;
		U8 SamplerIndex = 0;
		U8 TopologyIndex = 0;
	};
}