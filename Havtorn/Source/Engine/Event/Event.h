// Copyright 2022 Team Havtorn. All Rights Reserved.

#pragma once

#include "Core/Core.h"
#include "Core/CoreTypes.h"

namespace Havtorn
{
	enum class EEventType
	{
		None = 0,
		WindowClose,
		WindowResize,
		WindowFocus,
		WindowLostFocus,
		WindowMoved,
		AppTick,
		AppUpdate,
		AppRender,
		KeyPressed,
		KeyReleased,
		MouseButtonPressed,
		MouseButtonReleased,
		MouseMoved,
		MouseScrolled
	};

	enum class EEventCategory
	{
		None = 0,
		Application		= BIT(0),
		Input			= BIT(1),
		Keyboard		= BIT(2),
		Mouse			= BIT(3),
		MouseButton		= BIT(4)

	};

#define EVENT_CLASS_TYPE(type) static EEventType GetStaticType() { return EEventType::##type; }\
							   virtual EEventType GetEventType() const override { return GetStaticType(); }\
							   virtual const char* GetName() const override { return #type; }

#define EVENT_CLASS_CATEGORY(category) virtual U16 GetCategoryFlags() const override { return category; }

	class HAVTORN_API CEvent
	{
		friend class CEventDispatcher;

	public:
		virtual EEventType GetEventType() const = 0;
		virtual const char* GetName() const = 0;
		virtual U16 GetCategoryFlags() const = 0;
		virtual std::string ToString() const { return GetName(); }

		inline bool IsInCategory(EEventCategory category)
		{
			return GetCategoryFlags() & (U16)category;
		}

		bool IsHandled = false;
	//protected:
	};

	class CEventDispatcher
	{
		template<typename T>
		using EventFunction = std::function<bool(T&)>;

	public:
		CEventDispatcher(CEvent& eventToDispatch)
			: Event(eventToDispatch) {}

		template<typename T>
		bool Dispatch(EventFunction<T> func)
		{
			if (Event.GetEventType() == T::GetStaticType())
			{
				Event.IsHandled = func(*(T*)&Event);
				return true;
			}
			return false;
		}

	private:
		CEvent& Event;
	};

	//// Allows Logger to print events using HV_LOG_TRACE(e)
	//inline std::ostream& operator<<(std::ostream& os, const CEvent& e)
	//{
	//	return os << e.ToString();
	//}
}