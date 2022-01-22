// Copyright 2022 Team Havtorn. All Rights Reserved.

#pragma once
//#include <initializer_list>
#include "ECS/Component.h"

namespace Havtorn
{
#define GetComponent(x) GetComponentInternal<S##x*>(EComponentType::##x)

	struct SComponent;

	enum class ERenderCommandType
	{
		CameraDataStorage,
		GBufferData,
		GBufferDataInstanced,
		DeferredDecal,
		DeferredLighting,
		ForwardTransparency,
		OutlineMask,
		Outline
	};

	struct SRenderCommand
	{
		inline SRenderCommand(std::array<Ref<SComponent>, static_cast<size_t>(EComponentType::Count)> components, ERenderCommandType type)
			: Components(components)
			, Type(type) 
		{}

		~SRenderCommand() = default;

		template<typename T>
		inline T operator[](EComponentType type)
		{
			return dynamic_cast<T>(Components[static_cast<size_t>(type)].get());
		}

		template<typename T>
		inline T GetComponentInternal(EComponentType type)
		{
			return dynamic_cast<T>(Components[static_cast<size_t>(type)].get());
		}

		std::array<Ref<SComponent>, static_cast<size_t>(EComponentType::Count)> Components;
		//std::vector<Ref<SComponent>> Components;
		ERenderCommandType Type;
	};
}