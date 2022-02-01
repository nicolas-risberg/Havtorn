// Copyright 2022 Team Havtorn. All Rights Reserved.

#pragma once

namespace Havtorn
{
	enum class EInputState
	{
		Pressed		= BIT(0),
		Held		= BIT(1),
		Released	= BIT(2),
	};

	enum class EInputModifier
	{
		None		= BIT(0),
		Alt			= BIT(1),
		Shift		= BIT(2),
		Ctrl		= BIT(3),
	};

	enum class EInputContext
	{
		Editor		= BIT(0),
		InGame		= BIT(1),
	};

	enum class EInputKey
	{
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

	struct SInputAction
	{
		EInputState State;
		EInputModifier Modifier;
		EInputKey Key;
	};

	enum class EInputActionEvent
	{
		PopState,
		Push,
		Pull,
		MoveDown,
		OpenMenuPress,
		LoadLevel,
		PauseGame,
		QuitGame,
		MiddleMouseMove,
		StandStill,
		Moving,
		MoveForward,
		MoveBackward,
		MoveLeft,
		MoveRight,
		Jump,
		Crouch,
		ResetEntities,
		SetResetPointEntities,
		StartSprint,
		EndSprint,
		Count
	};

	enum class EInputAxisEvent
	{
		Horizontal,
		Vertical,
		Count
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
