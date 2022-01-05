#pragma once
#include <initializer_list>
#include "ECS/Component.h"

namespace Havtorn
{
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
		inline SRenderCommand(std::initializer_list<Ref<SComponent>> components, ERenderCommandType type)
			: Components(components)
			, Type(type) 
		{}

		~SRenderCommand() = default;

		std::vector<Ref<SComponent>> Components;
		ERenderCommandType Type;
	};
}