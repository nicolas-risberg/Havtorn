// Copyright 2022 Team Havtorn. All Rights Reserved.

#pragma once
#include "InputObserver.h"

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

		[[nodiscard]] CInputDelegate<SInputPayload>& GetActionDelegate(EInputActionEvent event);

		void SetInputContext(EInputContext context);

	private:
		void MapEvent(EInputActionEvent event, SInputAction action);
		void UpdateKeyboardInput();
		void UpdateMouseInput();

		std::map<EInputActionEvent, SInputActionEvent> BoundActionEvents;
		CInput* Input;

		EInputContext CurrentInputContext;
	};
}
