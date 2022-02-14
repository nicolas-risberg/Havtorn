// Copyright 2022 Team Havtorn. All Rights Reserved.

#pragma once
#include "InputTypes.h"

namespace Havtorn
{
	class CInput;

	class CInputMapper
	{
	public:
		CInputMapper();
		~CInputMapper() = default;
		CInputMapper(const CInputMapper&) = delete;
		CInputMapper(CInputMapper&&) = delete;
		CInputMapper operator=(const CInputMapper&) = delete;
		CInputMapper operator=(CInputMapper&&) = delete;

		bool Init();
		void Update();

		[[nodiscard]] CMulticastDelegate<const SInputActionPayload>& GetActionDelegate(EInputActionEvent event);
		[[nodiscard]] CMulticastDelegate<const SInputAxisPayload>& GetAxisDelegate(EInputAxisEvent event);

		void SetInputContext(EInputContext context);

	private:
		void MapEvent(EInputActionEvent event, SInputAction action);
		void MapEvent(EInputAxisEvent event, SInputAxis axisAction);
		void UpdateKeyboardInput();
		void UpdateMouseInput();

		std::map<EInputActionEvent, SInputActionEvent> BoundActionEvents;
		std::map<EInputAxisEvent, SInputAxisEvent> BoundAxisEvents;
		CInput* Input;

		EInputContext CurrentInputContext;
	};
}
