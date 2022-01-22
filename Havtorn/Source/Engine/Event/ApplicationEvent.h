// Copyright 2022 Team Havtorn. All Rights Reserved.

#pragma once

#include "Event.h"

namespace Havtorn
{
	class HAVTORN_API CWindowResizeEvent : public CEvent
	{
	public:
		CWindowResizeEvent(U16 width, U16 height)
			: Width(width), Height(height) {}

		inline U16 GetWidth() const { return Width; }
		inline U16 GetHeight() const { return Height; }

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "WindowResizeEvent: " << Width << ", " << Height;
			return ss.str();
		}

		EVENT_CLASS_TYPE(WindowResize)
		EVENT_CLASS_CATEGORY((U16)EEventCategory::Application)

	private:
		U16 Width, Height;
	};

	class HAVTORN_API CWindowCloseEvent : public CEvent
	{
	public:
		CWindowCloseEvent() {}

		EVENT_CLASS_TYPE(WindowClose)
		EVENT_CLASS_CATEGORY((U16)EEventCategory::Application)
	};

	class HAVTORN_API CAppTickEvent : public CEvent
	{
	public:
		CAppTickEvent() {}

		EVENT_CLASS_TYPE(AppTick)
		EVENT_CLASS_CATEGORY((U16)EEventCategory::Application)
	};

	class HAVTORN_API CAppUpdateEvent : public CEvent
	{
	public:
		CAppUpdateEvent() {}

		EVENT_CLASS_TYPE(AppUpdate)
		EVENT_CLASS_CATEGORY((U16)EEventCategory::Application)
	};

	class HAVTORN_API CAppRenderEvent : public CEvent
	{
	public:
		CAppRenderEvent() {}

		EVENT_CLASS_TYPE(AppRender)
			EVENT_CLASS_CATEGORY((U16)EEventCategory::Application)
	};
}