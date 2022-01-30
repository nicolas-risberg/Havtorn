// Copyright 2022 Team Havtorn. All Rights Reserved.

#pragma once
#include <bitset>

//#define INPUT_AXIS_USES_FALLOFF

namespace Havtorn
{
	class CInput
	{
	public:

		static CInput* GetInstance();

		enum class EMouseButton
		{
			Left = 0,
			Right = 1,
			Middle = 2,
			Mouse4 = 3,
			Mouse5 = 4
		};

		enum class EAxis
		{
			Horizontal = 0,
			Vertical = 1
		};

		CInput();

		bool UpdateEvents(UINT message, WPARAM wParam, LPARAM lParam);
		void Update();

		//bool MoveLeft();
		//bool MoveRight();
		//bool MoveUp();
		//bool MoveDown();

		static SVector2<F32> GetAxisRaw();

		F32 GetAxis(const EAxis& axis);
		bool IsKeyDown(WPARAM wParam);
		[[nodiscard]] bool IsKeyPressed(WPARAM wParam) const;
		[[nodiscard]] bool IsKeyReleased(WPARAM wParam) const;
		
		[[nodiscard]] U16 GetMouseX() const; // X coordiantes in application window
		[[nodiscard]] U16 GetMouseY() const; // Y coordiantes in application window
		[[nodiscard]] U16 GetMouseScreenX() const;
		[[nodiscard]] U16 GetMouseScreenY() const;
		[[nodiscard]] I16 GetMouseDeltaX() const;
		[[nodiscard]] I16 GetMouseDeltaY() const;
		[[nodiscard]] I16 GetMouseRawDeltaX() const;
		[[nodiscard]] I16 GetMouseRawDeltaY() const;
		[[nodiscard]] I16 GetMouseWheelDelta() const; // Positive = away from user, negative = towards user
		[[nodiscard]] bool IsMouseDown(EMouseButton mouseButton) const;
		[[nodiscard]] bool IsMousePressed(EMouseButton mouseButton) const;
		[[nodiscard]] bool IsMouseReleased(EMouseButton mouseButton) const;

		static void SetMouseScreenPosition(U16 x, U16 y);

	private:
		void UpdateAxisUsingFallOff();
		void UpdateAxisUsingNoFallOff();
		F32 GetAxisUsingFallOff(const EAxis& axis);
		F32 GetAxisUsingNoFallOff(const EAxis& axis);

	private:
		std::bitset<5> MouseButtonLast;
		std::bitset<5> MouseButton;

		std::bitset<256> KeyDownLast;
		std::bitset<256> KeyDown;

		U16 MouseX;
		U16 MouseY;
		U16 MouseScreenX;
		U16 MouseScreenY;
		U16 MouseLastX;
		U16 MouseLastY;
		U16 MouseRawDeltaX;
		U16 MouseRawDeltaY;
		I16 MouseWheelDelta;

		F32 Horizontal;
		F32 Vertical;
		bool HorizontalPressed;
		bool VerticalPressed;
	};
}