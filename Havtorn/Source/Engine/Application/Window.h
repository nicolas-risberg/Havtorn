// Copyright 2022 Team Havtorn. All Rights Reserved.

#pragma once

#include "hvpch.h"

#include "Core\Core.h"
#include "Event\Event.h"

namespace Havtorn
{
	struct SWindowProperties
	{
		std::string Title;
		U16 Width;
		U16 Height;

		SWindowProperties(const std::string& title = "Havtorn Editor", U16 width = 1280, U16 height = 720)
			: Title(title), Width(width), Height(height) {}
	};

	// Interface representing a desktop system based window
	class HAVTORN_API IWindow
	{
	public:
		using EventCallbackFunction = std::function<void(CEvent&)>;

		virtual ~IWindow() {}

		virtual void OnUpdate() = 0;

		virtual U16 GetWidth() const = 0;
		virtual U16 GetHeight() const = 0;

		virtual void SetEventCallback(const EventCallbackFunction& callback) = 0;
		virtual void SetVSync(bool enabled) = 0;
		virtual bool IsVSync() const = 0;

		static IWindow* Create(const SWindowProperties& props = SWindowProperties());
	};
}
