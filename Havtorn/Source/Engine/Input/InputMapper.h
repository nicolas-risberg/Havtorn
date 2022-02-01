// Copyright 2022 Team Havtorn. All Rights Reserved.

#pragma once
#include "InputObserver.h"
#include <unordered_map>

namespace Havtorn
{
	class CInput;

	class CInputMapper
	{
		friend class CEngine;
	public:
		static CInputMapper* GetInstance();

		void Update();
		void MapEvent(const EInputKey& inputEvent, const EInputActionEvent& outputEvent);
		bool AddObserver(const EInputActionEvent& eventToListenFor, IInputObserver* observer);
		bool RemoveObserver(const EInputActionEvent& eventToListenFor, IInputObserver* observer);
		bool HasObserver(const EInputActionEvent& eventToListenFor, IInputObserver* observer);

		void ClearObserverList(const EInputActionEvent& eventToListenFor);

	public:
		~CInputMapper() = default;
		CInputMapper(const CInputMapper&) = delete;
		CInputMapper(const CInputMapper&&) = delete;
		CInputMapper operator=(const CInputMapper&) = delete;
		CInputMapper operator=(const CInputMapper&&) = delete;

	private:
		static CInputMapper* Instance;

		CInputMapper();
		bool Init();

		void RunEvent(const EInputActionEvent& outputEvent);
		void TranslateActionToEvent(const EInputKey& action);
		void UpdateKeyboardInput();
		void UpdateMouseInput();

		std::unordered_map<EInputActionEvent, std::vector<IInputObserver*>> Observers;
		std::unordered_map<EInputKey, EInputActionEvent> Events;
		CInput* Input;
	};
}
