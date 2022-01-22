// Copyright 2022 Team Havtorn. All Rights Reserved.

#pragma once

#include "Event.h"
#include "Core/EngineTypes.h"

namespace Havtorn
{
	class HAVTORN_API CMouseMovedEvent : public CEvent
	{
	public:
		CMouseMovedEvent(F32 x, F32 y)
			: MouseX(x), MouseY(y) {}

		inline F32 GetX() const { return MouseX; }
		inline F32 GetY() const { return MouseY; }

		inline SVector2<F32> GetMousePosition() const { return SVector2(MouseX, MouseY); }

		std::string ToString() const override
		{
			std::stringstream ss;
			SVector2<F32> mousePos = SVector2(MouseX, MouseY);
			ss << "MouseMovedEvent: " << mousePos.ToString();
			return ss.str();
		}

		EVENT_CLASS_TYPE(MouseMoved)
		EVENT_CLASS_CATEGORY((U16)EEventCategory::Mouse | (U16)EEventCategory::Input)

	private:
		F32 MouseX, MouseY;
	};

	class HAVTORN_API CMouseScrolledEvent : public CEvent
	{
	public:
		CMouseScrolledEvent(F32 xOffset, F32 yOffset)
			: XOffset(xOffset), YOffset(yOffset) {}

		inline F32 GetX() const { return XOffset; }
		inline F32 GetY() const { return YOffset; }

		inline SVector2<F32> GetMousePosition() const { return SVector2(XOffset, YOffset); }

		std::string ToString() const override
		{
			std::stringstream ss;
			SVector2<F32> scrollOffsets = SVector2(XOffset, YOffset);
			ss << "MouseScrolledEvent: " << scrollOffsets.ToString();
			return ss.str();
		}

		EVENT_CLASS_TYPE(MouseScrolled)
		EVENT_CLASS_CATEGORY((U16)EEventCategory::Mouse | (U16)EEventCategory::Input)

	private:
		F32 XOffset, YOffset;
	};

	class HAVTORN_API CMouseButtonEvent : public CEvent
	{
	public:
		inline I8 GetMouseButton() const { return Button; }

		EVENT_CLASS_CATEGORY((U16)EEventCategory::Mouse | (U16)EEventCategory::Input)

	protected:
		CMouseButtonEvent(I8 button)
			: Button(button) {}

		I8 Button;
	};

	class HAVTORN_API CMouseButtonPressedEvent : public CMouseButtonEvent
	{
	public:
		CMouseButtonPressedEvent(I8 button)
			: CMouseButtonEvent(button) {}

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "MouseButtonPressedEvent: " << Button;
			return ss.str();
		}

		EVENT_CLASS_TYPE(MouseButtonPressed)
	};

	class HAVTORN_API CMouseButtonReleasedEvent : public CMouseButtonEvent
	{
	public:
		CMouseButtonReleasedEvent(I8 button)
			: CMouseButtonEvent(button) {}

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "MouseButtonReleasedEvent: " << Button;
			return ss.str();
		}

		EVENT_CLASS_TYPE(MouseButtonReleased)
	};
}