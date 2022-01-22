// Copyright 2022 Team Havtorn. All Rights Reserved.

#pragma once 

#include "Event.h"

namespace Havtorn
{
	class HAVTORN_API CKeyEvent : public CEvent
	{
	public:
		inline int GetKeyCode() const { return KeyCode; }

		EVENT_CLASS_CATEGORY((U16)EEventCategory::Keyboard | (U16)EEventCategory::Input)
	
	protected:
		CKeyEvent(U8 keyCode)
			: KeyCode(keyCode) {}

		U8 KeyCode = 0;
	};

	class HAVTORN_API CKeyPressedEvent : public CKeyEvent
	{
	public:
		CKeyPressedEvent(U8 keyCode, I32 repeatCount)
			: CKeyEvent(keyCode), RepeatCount(repeatCount) {}

		inline I32 GetRepeatCount() const { return RepeatCount; }

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "KeyPressedEvent: " << KeyCode << " (" << RepeatCount << " repeats)";
			return ss.str();
		}

		EVENT_CLASS_TYPE(KeyPressed)

	private:
		I32 RepeatCount = 0;
	};

	class HAVTORN_API CKeyReleasedEvent : public CKeyEvent
	{
	public:
		CKeyReleasedEvent(U8 keyCode)
			: CKeyEvent(keyCode) {}

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "KeyReleasedEvent: " << KeyCode;
			return ss.str();
		}

		EVENT_CLASS_TYPE(KeyReleased)
	};
}