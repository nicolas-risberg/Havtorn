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
		void MapEvent(const IInputObserver::EInputAction& inputEvent, const IInputObserver::EInputEvent& outputEvent);
		bool AddObserver(const IInputObserver::EInputEvent& eventToListenFor, IInputObserver* observer);
		bool RemoveObserver(const IInputObserver::EInputEvent& eventToListenFor, IInputObserver* observer);
		bool HasObserver(const IInputObserver::EInputEvent& eventToListenFor, IInputObserver* observer);

		void ClearObserverList(const IInputObserver::EInputEvent& eventToListenFor);

	public:
		~CInputMapper() = delete;

	private:
		static CInputMapper* Instance;

		CInputMapper();
		bool Init();

		void RunEvent(const IInputObserver::EInputEvent& outputEvent);
		void TranslateActionToEvent(const IInputObserver::EInputAction& action);
		void UpdateKeyboardInput();
		void UpdateMouseInput();

		std::unordered_map<IInputObserver::EInputEvent, std::vector<IInputObserver*>> Observers;
		std::unordered_map<IInputObserver::EInputAction, IInputObserver::EInputEvent> Events;
		CInput* Input;
	};
}
