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
		U32 VertexCount = 0;
		U32 IndexCount = 36;

		U8 VertexBufferIndex = 0;
		U8 InputLayoutIndex = 0;
		U8 IndexBufferIndex = 0;

		// Render Data
		U8 TopologyIndex = 0;
		U8 VertexStrideIndex = 0;
		U8 VertexOffsetIndex = 0;
		U8 VertexShaderIndex = 0;
		U8 PixelShaderIndex = 0;
		U8 SamplerIndex = 0;

	};
}