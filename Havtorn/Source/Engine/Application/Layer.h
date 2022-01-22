// Copyright 2022 Team Havtorn. All Rights Reserved.

#pragma once

#include "Core/Core.h"
#include "Event/Event.h"

namespace Havtorn
{
	class HAVTORN_API CLayer
	{
	public:
		CLayer(const std::string& name = "Layer");
		virtual ~CLayer();

		virtual void OnAttach() {}
		virtual void OnDetach() {}
		virtual void OnUpdate() {}
		virtual void OnEvent(CEvent& /*event*/) {}

		inline std::string GetName() const { return DebugName; }

	private:
		const char* DebugName;
	};
}

