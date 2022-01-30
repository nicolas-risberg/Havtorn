// Copyright 2022 Team Havtorn. All Rights Reserved.

#pragma once

namespace Havtorn
{
	class IInputObserver
	{
	public:
		enum class EInputAction
		{
			MouseLeftPressed,
			MouseLeftDown,
			MouseRightDown,
			MouseMiddle,
			KeyEscape,
			KeyEnter,
			KeyShiftDown,
			KeyShiftRelease,
			KeyW,
			KeyA,
			KeyS,
			KeyD,
			KeySpace,
			CTRL,
			KeyF5,
			KeyF8
		};

		enum class EInputEvent
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
			EndSprint
		};

	public:
		IInputObserver() = default;
		virtual ~IInputObserver() = default;
		IInputObserver(const IInputObserver&) = delete;
		IInputObserver(const IInputObserver&&) = delete;
		virtual void ReceiveEvent(const EInputEvent& event) = 0;
	};
}
