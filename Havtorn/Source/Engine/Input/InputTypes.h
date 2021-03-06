// Copyright 2022 Team Havtorn. All Rights Reserved.

#pragma once

namespace Havtorn
{
	enum class EInputModifier
	{
		None		= 0,
		Shift		= BIT(0),
		Ctrl		= BIT(1),
		Alt			= BIT(2),
	};

	enum class EInputContext
	{
		Editor		= BIT(0),
		InGame		= BIT(1),
	};

	enum class EInputKey
	{
		None		= 0x00,
		Mouse1		= 0x01, // Left
		Mouse2		= 0x02, // Right
		Mouse3		= 0x04, // Middle
		Mouse4		= 0x05,
		Mouse5		= 0x06,
		Backspace	= 0x08,
		Tab			= 0x09,
		Return		= 0x0D,	// Enter
		Shift		= 0x10,
		Ctrl		= 0x11,
		Alt			= 0x12,
		Pause		= 0x13,
		Caps		= 0x14,	// Caps Lock
		Esc			= 0x1B,	// Escape
		Space		= 0x20,
		PageUp		= 0x21,
		PageDown	= 0x22,
		End			= 0x23,
		Home		= 0x24,
		Left		= 0x25, // Left Arrow
		Up			= 0x26, // Up Arrow
		Right		= 0x27, // Right Arrow
		Down		= 0x28, // Down
		PrtSc		= 0x2C, // Print Screen
		Insert		= 0x2D,
		Delete		= 0x2E, 
		Key0		= 0x30,
		Key1		= 0x31,
		Key2		= 0x32,
		Key3		= 0x33,
		Key4		= 0x34,
		Key5		= 0x35,
		Key6		= 0x36,
		Key7		= 0x37,
		Key8		= 0x38,
		Key9		= 0x39,
		KeyA		= 0x41,
		KeyB		= 0x42,
		KeyC		= 0x43,
		KeyD		= 0x44,
		KeyE		= 0x45,
		KeyF		= 0x46,
		KeyG		= 0x47,
		KeyH		= 0x48,
		KeyI		= 0x49,
		KeyJ		= 0x4A,
		KeyK		= 0x4B,
		KeyL		= 0x4C,
		KeyM		= 0x4D,
		KeyN		= 0x4E,
		KeyO		= 0x4F,
		KeyP		= 0x50,
		KeyQ		= 0x51,
		KeyR		= 0x52,
		KeyS		= 0x53,
		KeyT		= 0x54,
		KeyU		= 0x55,
		KeyV		= 0x56,
		KeyW		= 0x57,
		KeyX		= 0x58,
		KeyY		= 0x59,
		KeyZ		= 0x5A,
		LWin		= 0x5B, // Left Windows key
		RWin		= 0x5C, // Right Windows key
		KeyNum0		= 0x60, // Numeric keypad 0 key
		KeyNum1		= 0x61, // Numeric keypad 1 key
		KeyNum2		= 0x62, // Numeric keypad 2 key
		KeyNum3		= 0x63, // Numeric keypad 3 key
		KeyNum4		= 0x64, // Numeric keypad 4 key
		KeyNum5		= 0x65, // Numeric keypad 5 key
		KeyNum6		= 0x66, // Numeric keypad 6 key
		KeyNum7		= 0x67, // Numeric keypad 7 key
		KeyNum8		= 0x68, // Numeric keypad 8 key
		KeyNum9		= 0x69, // Numeric keypad 9 key
		KeyNumMult	= 0x6A, // Numeric keypad Multiply key
		KeyNumAdd	= 0x6B, // Numeric keypad Add key
		Pipe		= 0x6C, // Separator key
		KeyNumSub	= 0x6D,	// Numeric keypad Subtract key
		KeyNumDec	= 0x6E,	// Numeric keypad Decimal key
		KeyNumDiv	= 0x6F,	// Numeric keypad Divide key
		F1			= 0x70,
		F2			= 0x71,
		F3			= 0x72,
		F4			= 0x73,
		F5			= 0x74,
		F6			= 0x75,
		F7			= 0x76,
		F8			= 0x77,
		F9			= 0x78,
		F10			= 0x79,
		F11			= 0x7A,
		F12			= 0x7B,
		NumLk		= 0x90,	// Num Lock key
		ScrLk		= 0x91,	// Scroll Lock key
	};

	enum class EInputAxis
	{
		Key,
		MouseWheel,
		MouseHorizontal,
		MouseVertical,
		AnalogHorizontal,
		AnalogVertical
	};

	enum class EInputActionEvent
	{
		None,
		CenterCamera,
		ResetCamera,
		TeleportCamera,
		Count
	};

	enum class EInputAxisEvent
	{
		Right,		// X-axis
		Up,			// Y-axis
		Forward,	// Z-axis
		Pitch,		// X-axis
		Yaw,		// Y-axis
		Roll,		// Z-axis
		MouseHorizontal,
		MouseVertical,
		Zoom,
		Count
	};

	struct SInputActionPayload
	{
		EInputKey Key = EInputKey::None;
		bool IsPressed = false;
		bool IsHeld = false;
		bool IsReleased = false;
	};

	struct SInputAxisPayload
	{
		EInputAxisEvent Event = EInputAxisEvent::Count;
		F32 AxisValue = 0.0f;
	};

	struct SInputAction
	{
		SInputAction(EInputKey key, EInputContext context, EInputModifier modifier)
			: Key(key)
			, Contexts(static_cast<U32>(context))
			, Modifiers(static_cast<U32>(modifier))
		{}

		SInputAction(EInputKey key, std::initializer_list<EInputContext> contexts, std::initializer_list<EInputModifier> modifiers = {})
			: Key(key)
			, Contexts(static_cast<U32>(EInputContext::Editor))
			, Modifiers(0)
		{
			SetContexts(contexts);
			SetModifiers(modifiers);
		}

		SInputAction(EInputKey key, EInputContext context)
			: Key(key)
			, Contexts(static_cast<U32>(context))
			, Modifiers(0)
		{}

		// Pass in the number of modifiers the SInputAction should have
		// followed by that number of EInputModifier entries, separated by comma
		void SetModifiers(U32 numberOfModifiers, ...)
		{
			Modifiers = 0;

			va_list args;
			va_start(args, numberOfModifiers);

			for (U32 index = 0; index < numberOfModifiers; index++)
			{
				Modifiers += static_cast<U32>(va_arg(args, EInputModifier));
			}

			va_end(args);
		}

		void SetModifiers(std::initializer_list<EInputModifier> modifiers)
		{
			Modifiers = 0;
			for (auto modifier : modifiers)
				Modifiers += static_cast<U32>(modifier);
		}

		void SetContexts(std::initializer_list<EInputContext> contexts)
		{
			Contexts = 0;
			for (auto context : contexts)
				Contexts += static_cast<U32>(context);
		}

		EInputKey Key = EInputKey::None;
		U32 Contexts = static_cast<U32>(EInputContext::Editor);
		U32 Modifiers = static_cast<U32>(EInputModifier::None);
	};

	struct SInputActionEvent
	{
		SInputActionEvent() = default;

		explicit SInputActionEvent(SInputAction action)
			: Delegate(CMulticastDelegate<const SInputActionPayload>())
		{
			Actions.push_back(action);
		}

		[[nodiscard]] bool HasKey(const EInputKey& key) const
		{
			return std::ranges::any_of(Actions.begin(), Actions.end(),
				[key](const SInputAction& action) {return action.Key == key; });
		}

		[[nodiscard]] bool HasContext(U32 context) const
		{
			return std::ranges::any_of(Actions.begin(), Actions.end(),
				[context](const SInputAction& action) {return (action.Contexts & context) != 0; });
		}

		[[nodiscard]] bool HasModifiers(U32 modifiers) const
		{
			return std::ranges::any_of(Actions.begin(), Actions.end(),
				[modifiers](const SInputAction& action) {return (action.Modifiers ^ modifiers) == 0; });
		}

		[[nodiscard]] bool Has(const EInputKey& key, U32 context, U32 modifiers) const
		{
			return std::ranges::any_of(Actions.begin(), Actions.end(),
				[key, context, modifiers](const SInputAction& action)
				{
					return action.Key == key && (action.Contexts & context) != 0 && (action.Modifiers ^ modifiers) == 0;
				});
		}

		CMulticastDelegate<const SInputActionPayload> Delegate;
		std::vector<SInputAction> Actions;
	};

	struct SInputAxis
	{
		SInputAxis(EInputAxis axis, EInputContext context)
			: Axis(axis)
			, AxisPositiveKey(EInputKey::KeyW)
			, AxisNegativeKey(EInputKey::KeyS)
			, Contexts(static_cast<U32>(context))
			, Modifiers(0)
		{}

		SInputAxis(EInputAxis axis, EInputContext context, EInputModifier modifier)
			: Axis(axis)
			, AxisPositiveKey(EInputKey::KeyW)
			, AxisNegativeKey(EInputKey::KeyS)
			, Contexts(static_cast<U32>(context))
			, Modifiers(static_cast<U32>(modifier))
		{}

		SInputAxis(EInputAxis axis, EInputKey axisPositiveKey, EInputKey axisNegativeKey, EInputContext context)
			: Axis(axis)
			, AxisPositiveKey(axisPositiveKey)
			, AxisNegativeKey(axisNegativeKey)
			, Contexts(static_cast<U32>(context))
			, Modifiers(0)
		{}

		SInputAxis(EInputAxis axis, std::initializer_list<EInputContext> contexts, std::initializer_list<EInputModifier> modifiers = {})
			: Axis(axis)
			, AxisPositiveKey(EInputKey::KeyW)
			, AxisNegativeKey(EInputKey::KeyS)
			, Contexts(static_cast<U32>(EInputContext::Editor))
			, Modifiers(0)
		{
			SetContexts(contexts);
			SetModifiers(modifiers);
		}

		SInputAxis(EInputAxis axis, EInputKey axisPositiveKey, EInputKey axisNegativeKey, std::initializer_list<EInputContext> contexts, std::initializer_list<EInputModifier> modifiers = {})
			: Axis(axis)
			, AxisPositiveKey(axisPositiveKey)
			, AxisNegativeKey(axisNegativeKey)
			, Contexts(static_cast<U32>(EInputContext::Editor))
			, Modifiers(0)
		{
			SetContexts(contexts);
			SetModifiers(modifiers);
		}

		// Pass in the number of modifiers the SInputAction should have
		// followed by that number of EInputModifier entries, separated by comma
		void SetModifiers(U32 numberOfModifiers, ...)
		{
			Modifiers = 0;

			va_list args;
			va_start(args, numberOfModifiers);

			for (U32 index = 0; index < numberOfModifiers; index++)
			{
				Modifiers += static_cast<U32>(va_arg(args, EInputModifier));
			}

			va_end(args);
		}

		void SetModifiers(std::initializer_list<EInputModifier> modifiers)
		{
			Modifiers = 0;
			for (auto modifier : modifiers)
				Modifiers += static_cast<U32>(modifier);
		}

		void SetContexts(std::initializer_list<EInputContext> contexts)
		{
			Contexts = 0;
			for (auto context : contexts)
				Contexts += static_cast<U32>(context);
		}

		[[nodiscard]] F32 GetAxisValue(const EInputKey& key) const
		{
			if (AxisPositiveKey == key)
				return 1.0;

			if (AxisNegativeKey == key)
				return -1.0f;

			return 0.0f;
		}

		[[nodiscard]] F32 GetAxisValue(const F32 rawValue) const
		{
			switch (Axis)
			{
				// Mouse Wheel scroll threshold is capped at 120
				case EInputAxis::MouseWheel:
					return rawValue / 120.0f;

				// Return raw delta for now
				case EInputAxis::MouseHorizontal: 
				case EInputAxis::MouseVertical: 
					return rawValue;

				case EInputAxis::AnalogHorizontal: 

				case EInputAxis::AnalogVertical: 

				// Do not handle this here, call GetAxisValue(const EInputKey&) instead
				case EInputAxis::Key:
				default:
					return 0.0f;
			}
		}

		EInputAxis Axis = EInputAxis::Key;
		EInputKey AxisPositiveKey = EInputKey::None; // Optional
		EInputKey AxisNegativeKey = EInputKey::None; // Optional
		U32 Contexts = static_cast<U32>(EInputContext::Editor);
		U32 Modifiers = static_cast<U32>(EInputModifier::None);
	};

	struct SInputAxisEvent
	{
		SInputAxisEvent() = default;

		explicit SInputAxisEvent(SInputAxis axis)
			: Delegate(CMulticastDelegate<const SInputAxisPayload>())
		{
			Axes.push_back(axis);
		}

		[[nodiscard]] bool HasKeyAxis() const
		{
			return std::ranges::any_of(Axes.begin(), Axes.end(),
				[](const SInputAxis& axis) {return axis.Axis == EInputAxis::Key; });
		}

		[[nodiscard]] bool Has(const EInputKey& key, U32 context, U32 modifiers, F32& outAxisValue) const
		{
			return std::ranges::any_of(Axes.begin(), Axes.end(),
				[key, context, modifiers, &outAxisValue](const SInputAxis& axisAction)
				{
					if ((axisAction.AxisPositiveKey == key || axisAction.AxisNegativeKey == key)
						&& (axisAction.Contexts & context) != 0 && (axisAction.Modifiers ^ modifiers) == 0)
					{
						outAxisValue = axisAction.GetAxisValue(key);
						return true;
					}
					return false;
				});
		}

		[[nodiscard]] bool Has(const EInputAxis& axis, F32& outAxisValue) const
		{
			return std::ranges::any_of(Axes.begin(), Axes.end(),
				[axis, &outAxisValue](const SInputAxis& axisAction)
				{
					if (axisAction.Axis == axis)
					{
						outAxisValue = axisAction.GetAxisValue(outAxisValue);
						return true;
					}

					return false;
				});
		}

		CMulticastDelegate<const SInputAxisPayload> Delegate;
		std::vector<SInputAxis> Axes;
	};

	class IInputObserver
	{
	public:
		IInputObserver() = default;
		virtual ~IInputObserver() = default;
		IInputObserver(const IInputObserver&) = delete;
		IInputObserver(const IInputObserver&&) = delete;

		virtual void ReceiveEvent(const EInputActionEvent& event) = 0;
	};
}
