// Copyright 2022 Team Havtorn. All Rights Reserved.

#include "InputMapper.h"
#include "Input.h"

namespace Havtorn
{
	CInputMapper* CInputMapper::Instance = nullptr;

	CInputMapper* CInputMapper::GetInstance()
	{
		return Instance;
	}

	CInputMapper::CInputMapper() : Input(CInput::GetInstance())
	{}

	bool CInputMapper::Init()
	{
		Instance = this;

		MapEvent(IInputObserver::EInputAction::MouseLeftDown, IInputObserver::EInputEvent::Push);
		MapEvent(IInputObserver::EInputAction::MouseRightDown, IInputObserver::EInputEvent::Pull);
		MapEvent(IInputObserver::EInputAction::MouseMiddle, IInputObserver::EInputEvent::MiddleMouseMove);
		MapEvent(IInputObserver::EInputAction::KeyShiftDown, IInputObserver::EInputEvent::StandStill);
		MapEvent(IInputObserver::EInputAction::KeyShiftRelease, IInputObserver::EInputEvent::Moving);
		MapEvent(IInputObserver::EInputAction::KeyW, IInputObserver::EInputEvent::MoveForward);
		MapEvent(IInputObserver::EInputAction::KeyA, IInputObserver::EInputEvent::MoveLeft);
		MapEvent(IInputObserver::EInputAction::KeyS, IInputObserver::EInputEvent::MoveBackward);
		MapEvent(IInputObserver::EInputAction::KeyD, IInputObserver::EInputEvent::MoveRight);
		MapEvent(IInputObserver::EInputAction::KeyEscape, IInputObserver::EInputEvent::PauseGame);
		MapEvent(IInputObserver::EInputAction::KeySpace, IInputObserver::EInputEvent::Jump);
		MapEvent(IInputObserver::EInputAction::CTRL, IInputObserver::EInputEvent::Crouch);
		MapEvent(IInputObserver::EInputAction::KeyF5, IInputObserver::EInputEvent::ResetEntities);
		MapEvent(IInputObserver::EInputAction::KeyF8, IInputObserver::EInputEvent::SetResetPointEntities);
		MapEvent(IInputObserver::EInputAction::KeyShiftDown, IInputObserver::EInputEvent::StartSprint);
		MapEvent(IInputObserver::EInputAction::KeyShiftRelease, IInputObserver::EInputEvent::EndSprint);

		if (this == nullptr)
			return false;

		return true;
	}

	void CInputMapper::RunEvent(const IInputObserver::EInputEvent& outputEvent)
	{
		for (int i = 0; i < Observers[outputEvent].size(); ++i)
		{
			Observers[outputEvent][i]->ReceiveEvent(outputEvent);
		}
	}

	void CInputMapper::TranslateActionToEvent(const IInputObserver::EInputAction& action)
	{
		if (const auto eventIterator = Events.find(action); eventIterator != Events.end())
		{
			RunEvent(Events[action]);
		}
	}

	void CInputMapper::UpdateKeyboardInput()
	{
		if (Input->IsKeyPressed(VK_SPACE))
		{
			TranslateActionToEvent(IInputObserver::EInputAction::KeySpace);
		}

		if (Input->IsKeyPressed(VK_ESCAPE))
		{
			TranslateActionToEvent(IInputObserver::EInputAction::KeyEscape);
		}

		if (Input->IsKeyDown(VK_SHIFT))
		{
			TranslateActionToEvent(IInputObserver::EInputAction::KeyShiftDown);
		}

		if (Input->IsKeyReleased(VK_SHIFT))
		{
			TranslateActionToEvent(IInputObserver::EInputAction::KeyShiftRelease);
		}

		if (Input->IsKeyDown('W'))
		{
			TranslateActionToEvent(IInputObserver::EInputAction::KeyW);
		}
		if (Input->IsKeyDown('A'))
		{
			TranslateActionToEvent(IInputObserver::EInputAction::KeyA);
		}
		if (Input->IsKeyDown('S'))
		{
			TranslateActionToEvent(IInputObserver::EInputAction::KeyS);
		}
		if (Input->IsKeyDown('D'))
		{
			TranslateActionToEvent(IInputObserver::EInputAction::KeyD);
		}
		if (Input->IsKeyPressed(VK_CONTROL))
		{
			TranslateActionToEvent(IInputObserver::EInputAction::CTRL);
		}
#ifdef _DEBUG
		if (Input->IsKeyPressed(VK_F5))
		{
			TranslateActionToEvent(IInputObserver::EInputAction::KeyF5);
		}
		if (Input->IsKeyPressed(VK_F8))
		{
			TranslateActionToEvent(IInputObserver::EInputAction::KeyF8);
		}
#endif // DEBUG
	}

	void CInputMapper::UpdateMouseInput()
	{

		if (Input->IsMouseDown(CInput::EMouseButton::Middle))
		{
			TranslateActionToEvent(IInputObserver::EInputAction::MouseMiddle);
		}
		if (Input->IsMousePressed(CInput::EMouseButton::Left))
		{
			TranslateActionToEvent(IInputObserver::EInputAction::MouseLeftPressed);
		}
		if (Input->IsMouseDown(CInput::EMouseButton::Left))
		{
			TranslateActionToEvent(IInputObserver::EInputAction::MouseLeftDown);
		}
		//else if (myInput->IsMouseDown(CInput::EMouseButton::Left))
		//{
		//	TranslateActionToEvent(IInputObserver::EInputAction::MouseLeft);
		//}
		//else if (myInput->IsMouseReleased(CInput::EMouseButton::Left))
		//{
		//	TranslateActionToEvent(IInputObserver::EInputAction::MouseLeft);
		//}
		if (Input->IsMouseDown(CInput::EMouseButton::Right))
		{
			TranslateActionToEvent(IInputObserver::EInputAction::MouseRightDown);
		}
	}

	void CInputMapper::Update()
	{
		UpdateKeyboardInput();
		UpdateMouseInput();
		Input->Update();
	}

	void CInputMapper::MapEvent(const IInputObserver::EInputAction& inputEvent, const IInputObserver::EInputEvent& outputEvent)
	{
		Events[inputEvent] = outputEvent;
	}

	bool CInputMapper::AddObserver(const IInputObserver::EInputEvent& eventToListenFor, IInputObserver* observer)
	{
		ENGINE_ERROR_BOOL_MESSAGE(observer, "InputObsever is nullptr!");

		if (const auto it = std::ranges::find(Observers[eventToListenFor], observer); it == Observers[eventToListenFor].end())
			Observers[eventToListenFor].emplace_back(observer);

		return true;
	}

	bool CInputMapper::RemoveObserver(const IInputObserver::EInputEvent& eventToListenFor, IInputObserver* observer)
	{
		ENGINE_ERROR_BOOL_MESSAGE(observer, "InputObsever is nullptr!");

		if (const auto it = std::ranges::find(Observers[eventToListenFor], observer); it != Observers[eventToListenFor].end())
			Observers[eventToListenFor].erase(it);

		return true;
	}

	bool CInputMapper::HasObserver(const IInputObserver::EInputEvent& eventToListenFor, IInputObserver* observer)
	{
		const auto it = std::ranges::find(Observers[eventToListenFor], observer);
		return it != Observers[eventToListenFor].end();
	}

	void CInputMapper::ClearObserverList(const IInputObserver::EInputEvent& eventToListenFor)
	{
		Observers[eventToListenFor].clear();
	}
}
