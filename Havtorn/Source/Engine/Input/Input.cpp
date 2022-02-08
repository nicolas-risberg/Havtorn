// Copyright 2022 Team Havtorn. All Rights Reserved.

#include "hvpch.h"
#include "Input.h"

#include <windowsx.h>

#include "imgui.h"
#include "Engine.h"

#include <ranges>

namespace Havtorn
{
	CInput* CInput::GetInstance()
	{
		static auto input = new CInput();
		return input;
	}

	CInput::CInput()
		: MouseX(0)
		, MouseY(0)
		, MouseScreenX(0)
		, MouseScreenY(0)
		, MouseLastX(0)
		, MouseLastY(0)
		, MouseRawDeltaX(0)
		, MouseRawDeltaY(0)
		, MouseWheelDelta(0)
		, Horizontal(0)
		, Vertical(0)
		, HorizontalPressed(false)
		, VerticalPressed(false)
	{

		RAWINPUTDEVICE rid;
		rid.usUsagePage = 0x01; // For mouse
		rid.usUsage = 0x02; // For mouse
		rid.dwFlags = 0;
		rid.hwndTarget = nullptr;
		if (RegisterRawInputDevices(&rid, 1, sizeof(rid)) == FALSE)
		{
			ENGINE_BOOL_POPUP(false, "Mouse could not be registered as Raw Input Device")
		}
	}

	bool CInput::UpdateEvents(UINT message, WPARAM wParam, LPARAM lParam) {

		std::vector<char> rawBuffer;

		switch (message)
		{
		case WM_SYSKEYDOWN:
		case WM_KEYDOWN:
			KeyDown[wParam] = true;

			if (wParam == 0x10 || wParam == 0x11 || wParam == 0x12) // Handle Shift, Ctrl, and Alt input modifiers
			{
				KeyInputModifiers[wParam - 16] = true;
				return true;
			}

			if (KeyInputBuffer.contains(wParam))
			{
				if (KeyInputBuffer[wParam].IsPressed)
				{
					KeyInputBuffer[wParam].IsPressed = false;
					KeyInputBuffer[wParam].IsHeld = true;
				}
				else if (!KeyInputBuffer[wParam].IsHeld)
				{
					KeyInputBuffer[wParam].IsPressed = true;
				}
			}
			else
			{
				KeyInputBuffer.emplace(wParam, SInputPayload());
				KeyInputBuffer[wParam].IsPressed = true;
			}

			KeyPressedInputBuffer.push_back(wParam);
			return true;

		case WM_SYSKEYUP:
		case WM_KEYUP:
			KeyDown[wParam] = false;

			if (wParam == 0x10 || wParam == 0x11 || wParam == 0x12) // Handle Shift, Ctrl, and Alt input modifiers
			{
				KeyInputModifiers[wParam - 16] = false;
				return true;
			}

			KeyInputBuffer[wParam].IsPressed = false;
			KeyInputBuffer[wParam].IsHeld = false;
			KeyInputBuffer[wParam].IsReleased = true;

			return true;

		case WM_MOUSEMOVE:
			MouseX = GET_X_LPARAM(lParam); // Returns x coordiante
			MouseY = GET_Y_LPARAM(lParam); // Returns y coordinate
			return true;

		case WM_MOUSEWHEEL:
			MouseWheelDelta += GET_WHEEL_DELTA_WPARAM(wParam); // Returns difference in mouse wheel position
			return true;

		case WM_LBUTTONDOWN:
			MouseButton[static_cast<U32>(EMouseButton::Left)] = true;
			return true;

		case WM_LBUTTONUP:
			MouseButton[static_cast<U32>(EMouseButton::Left)] = false;
			return true;

		case WM_RBUTTONDOWN:
			MouseButton[static_cast<U32>(EMouseButton::Right)] = true;
			return true;

		case WM_RBUTTONUP:
			MouseButton[static_cast<U32>(EMouseButton::Right)] = false;
			return true;

		case WM_MBUTTONDOWN:
			MouseButton[static_cast<U32>(EMouseButton::Middle)] = true;
			return true;

		case WM_MBUTTONUP:
			MouseButton[static_cast<U32>(EMouseButton::Middle)] = false;
			return true;

		case WM_XBUTTONDOWN:
			if (GET_XBUTTON_WPARAM(wParam) == XBUTTON1) 
			{
				MouseButton[MouseButton[static_cast<U32>(EMouseButton::Mouse4)]] = true;
			}
			else if (GET_XBUTTON_WPARAM(wParam) == XBUTTON2) 
			{
				MouseButton[MouseButton[static_cast<U32>(EMouseButton::Mouse5)]] = true;
			}
			return true;

		case WM_XBUTTONUP:
			if (GET_XBUTTON_WPARAM(wParam) == XBUTTON1)
			{
				MouseButton[MouseButton[static_cast<U32>(EMouseButton::Mouse4)]] = false;
			}
			else if (GET_XBUTTON_WPARAM(wParam) == XBUTTON2)
			{
				MouseButton[MouseButton[static_cast<U32>(EMouseButton::Mouse5)]] = false;
			}
			break;

		// Raw Input
		case WM_INPUT:
		{
			UINT size;
			if (GetRawInputData(
				reinterpret_cast<HRAWINPUT>(lParam),
				RID_INPUT,
				nullptr,
				&size,
				sizeof(RAWINPUTHEADER)) == -1)
			{
				// Error if inputdata == -1
				break;
			}
			rawBuffer.resize(size);

			if (GetRawInputData(
				reinterpret_cast<HRAWINPUT>(lParam),
				RID_INPUT,
				rawBuffer.data(),
				&size,
				sizeof(RAWINPUTHEADER)) != size)
			{
				// Probably an error if the size doesn't match up
				break;
			}

			auto& rawInput = reinterpret_cast<const RAWINPUT&>(*rawBuffer.data());
			if (rawInput.header.dwType == RIM_TYPEMOUSE &&
				(rawInput.data.mouse.lLastX != 0 || rawInput.data.mouse.lLastY != 0))
			{
				// Register raw input
				MouseRawDeltaX = static_cast<U16>(rawInput.data.mouse.lLastX);
				MouseRawDeltaY = static_cast<U16>(rawInput.data.mouse.lLastY);
			}
		}
			break;

		default:
			break;
		}

		return false;
	}

	void CInput::UpdateState()
	{
		KeyDownLast = KeyDown;

		MouseLastX = MouseX;
		MouseLastY = MouseY;
		MouseRawDeltaX = 0;
		MouseRawDeltaY = 0;
		MouseWheelDelta = 0;
		MouseButtonLast = MouseButton;

		POINT point;
		if (GetCursorPos(&point)) 
		{
			MouseScreenX = static_cast<U16>(point.x);
			MouseScreenY = static_cast<U16>(point.y);
		}

#ifdef INPUT_AXIS_USES_FALLOFF
		UpdateAxisUsingFallOff();
#else
		UpdateAxisUsingNoFallOff();
#endif

		KeyPressedInputBuffer.clear();

		for (auto& keyInput : KeyInputBuffer | std::views::values)
		{
			if (keyInput.IsPressed)
			{
				keyInput.IsPressed = false;
				keyInput.IsHeld = true;
			}
		}

		for (auto it = KeyInputBuffer.cbegin(); it != KeyInputBuffer.cend();)
		{
			auto& keyInput = it->second;

			if (keyInput.IsReleased)
				it = KeyInputBuffer.erase(it);

			else
				++it;
		}
	}

	const std::vector<WPARAM>& CInput::GetKeyPressedInputBuffer() const
	{
		return KeyPressedInputBuffer;
	}

	const std::vector<WPARAM>& CInput::GetKeyHeldInputBuffer() const
	{
		return KeyHeldInputBuffer;
	}

	const std::vector<WPARAM>& CInput::GetKeyReleasedInputBuffer() const
	{
		return KeyReleasedInputBuffer;
	}

	const std::map<WPARAM, SInputPayload>& CInput::GetKeyInputBuffer() const
	{
		return KeyInputBuffer;
	}

	const std::bitset<3>& CInput::GetKeyInputModifiers() const
	{
		return KeyInputModifiers;
	}

	F32 CInput::GetAxis(const EAxis& axis)
	{
#ifdef INPUT_AXIS_USES_FALLOFF
		return GetAxisUsingFallOff(axis);
#else
		return GetAxisUsingNoFallOff(axis);
#endif
	}

	bool CInput::IsKeyDown(WPARAM wParam) {


		//if (ImguiInput.WantCaptureMouse)
		//{
		//	return false;
		//}

		return KeyDown[wParam];
	}

	bool CInput::IsKeyPressed(WPARAM wParam) const
	{
		return KeyDown[wParam] && (!KeyDownLast[wParam]);
	}

	bool CInput::IsKeyReleased(WPARAM wParam) const
	{
		return (!KeyDown[wParam]) && KeyDownLast[wParam];
	}

	SVector2<F32> CInput::GetAxisRaw()
	{
		POINT p;
		GetCursorPos(&p);
		const SVector2<F32> currentPos = 
		{
			static_cast<F32>(p.x),
			static_cast<F32>(p.y)
		};
		const SVector2<F32> center = CEngine::GetInstance()->GetWindowHandler()->GetCenterPosition();
		SetCursorPos(static_cast<I32>(center.X), static_cast<I32>(center.Y));
		const SVector2<F32> axisRaw = currentPos - center;
		return axisRaw;
	}

	U16 CInput::GetMouseX() const
	{
		return MouseX;
	}

	U16 CInput::GetMouseY() const
	{
		return MouseY;
	}

	U16 CInput::GetMouseScreenX() const
	{
		return MouseScreenX;
	}

	U16 CInput::GetMouseScreenY() const
	{
		return MouseScreenY;
	}

	I16 CInput::GetMouseDeltaX() const
	{
		return static_cast<I16>(MouseX - MouseLastX);
	}

	I16 CInput::GetMouseDeltaY() const
	{
		return static_cast<I16>(MouseY - MouseLastY);
	}

	I16 CInput::GetMouseRawDeltaX() const
	{
		return static_cast<I16>(MouseRawDeltaX);
	}

	I16 CInput::GetMouseRawDeltaY() const
	{
		return static_cast<I16>(MouseRawDeltaY);
	}

	I16 CInput::GetMouseWheelDelta() const
	{
		return MouseWheelDelta;
	}

	bool CInput::IsMouseDown(EMouseButton mouseButton) const
	{
		return MouseButton[static_cast<U32>(mouseButton)];
	}

	bool CInput::IsMousePressed(EMouseButton mouseButton) const
	{
		return MouseButton[static_cast<U32>(mouseButton)] && (!MouseButtonLast[static_cast<U32>(mouseButton)]);
	}

	bool CInput::IsMouseReleased(EMouseButton mouseButton) const
	{
		return (!MouseButton[static_cast<U32>(mouseButton)]) && MouseButtonLast[static_cast<U32>(mouseButton)];
	}

	void CInput::SetMouseScreenPosition(U16 x, U16 y)
	{
		SetCursorPos(x, y);
	}

	void CInput::UpdateAxisUsingFallOff()
	{
		if (!HorizontalPressed) 
		{
			if (Horizontal >= (0.0f + CTimer::FixedDt())) 
			{
				Horizontal -= CTimer::FixedDt();
			}
			else if (Horizontal <= (0.0f - CTimer::FixedDt())) 
			{
				Horizontal += CTimer::FixedDt();
			}
			else 
			{
				Horizontal = 0.0f;
			}
		}
		if (VerticalPressed == false) 
		{
			if (Vertical >= (0.0f + CTimer::FixedDt())) 
			{
				Vertical -= CTimer::FixedDt();
			}
			else if (Vertical <= (0.0f - CTimer::FixedDt())) 
			{
				Vertical += CTimer::FixedDt();
			}
			else {
				Vertical = 0.0f;
			}
		}
	}

	void CInput::UpdateAxisUsingNoFallOff()
	{
		if (!HorizontalPressed)
		{
			Horizontal = 0.0f;
		}
		if (!VerticalPressed) 
		{
			Vertical = 0.0f;
		}
	}

	F32 CInput::GetAxisUsingFallOff(const EAxis& axis)
	{
		if (axis == EAxis::Horizontal) 
		{
			HorizontalPressed = false;
			if (IsKeyDown('A')) 
			{
				HorizontalPressed = true;
				Horizontal += CTimer::FixedDt(); // For falloff/deceleration
				if (Horizontal >= 1.0f)
				{
					Horizontal = 1.0f;
				}
			}
			if (IsKeyDown('D')) 
			{
				HorizontalPressed = true;
				Horizontal -= CTimer::FixedDt(); // For falloff/deceleration
				if (Horizontal <= -1.0f)
				{
					Horizontal = -1.0f;
				}
			}
			return Horizontal;
		}
		if (axis == EAxis::Vertical) 
		{
			VerticalPressed = false;
			if (IsKeyDown('W')) 
			{
				VerticalPressed = true;
				Vertical += CTimer::FixedDt(); // For falloff/deceleration
				if (Vertical >= 1.0f) 
				{
					Vertical = 1.0f;
				}
			}
			if (IsKeyDown('S')) 
			{
				VerticalPressed = true;
				Vertical -= CTimer::FixedDt(); // For falloff/deceleration
				if (Vertical <= -1.0f)
				{
					Vertical = -1.0f;
				}
			}
			return Vertical;
		}
		return 0.0f;
	}

	F32 CInput::GetAxisUsingNoFallOff(const EAxis& axis)
	{
		if (axis == EAxis::Horizontal) 
		{
			HorizontalPressed = false;
			if (IsKeyDown('A')) 
			{
				HorizontalPressed = true;
				Horizontal = 1.0f;
			}
			if (IsKeyDown('D')) 
			{
				HorizontalPressed = true;
				Horizontal = -1.f;
			}
			return Horizontal;
		}
		if (axis == EAxis::Vertical) 
		{
			VerticalPressed = false;
			if (IsKeyDown('W'))
			{
				VerticalPressed = true;
				Vertical = 1.f;
			}
			if (IsKeyDown('S')) 
			{
				VerticalPressed = true;
				Vertical = -1.f;

			}
			return Vertical;
		}
		return 0.0f;
	}
}
