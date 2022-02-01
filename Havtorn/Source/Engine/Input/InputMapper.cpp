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

		/// <summary>
		/// Ideas:
		///	Store delegates in CInputMapper
		///	fill up buffer in CInput
		///	use bitsets to define InputActions?
		/// typedef static delegates, dont template
		///	start on proper delegate system!
		///
		/// System -> CInputMapper->AddInputAxisDelegate<void, SVector2>("CameraTranslation", &CameraSystem::Move)
		/// System -> CInputMapper->AddInputActionDelegate<void, float>("CameraForward", &CameraSystem::MoveForward)
		///
		///	CameraSystem::Move(float)
		///	{
		///		
		///	}
		/// </summary>
		/// <returns></returns>

		//MapEvent(EInputKey::MouseLeftDown, EInputActionEvent::Push);
		//MapEvent(EInputKey::MouseRightDown, EInputActionEvent::Pull);
		//MapEvent(EInputKey::MouseMiddle, EInputActionEvent::MiddleMouseMove);
		//MapEvent(EInputKey::KeyShiftDown, EInputActionEvent::StandStill);
		//MapEvent(EInputKey::KeyShiftRelease, EInputActionEvent::Moving);
		//MapEvent(EInputKey::KeyW, EInputActionEvent::MoveForward);
		//MapEvent(EInputKey::KeyA, EInputActionEvent::MoveLeft);
		//MapEvent(EInputKey::KeyS, EInputActionEvent::MoveBackward);
		//MapEvent(EInputKey::KeyD, EInputActionEvent::MoveRight);
		//MapEvent(EInputKey::KeyEscape, EInputActionEvent::PauseGame);
		//MapEvent(EInputKey::KeySpace, EInputActionEvent::Jump);
		//MapEvent(EInputKey::CTRL, EInputActionEvent::Crouch);
		//MapEvent(EInputKey::KeyF5, EInputActionEvent::ResetEntities);
		//MapEvent(EInputKey::KeyF8, EInputActionEvent::SetResetPointEntities);
		//MapEvent(EInputKey::KeyShiftDown, EInputActionEvent::StartSprint);
		//MapEvent(EInputKey::KeyShiftRelease, EInputActionEvent::EndSprint);

		if (this == nullptr)
			return false;

		return true;
	}

	void CInputMapper::RunEvent(const EInputActionEvent& outputEvent)
	{
		for (int i = 0; i < Observers[outputEvent].size(); ++i)
		{
			Observers[outputEvent][i]->ReceiveEvent(outputEvent);
		}
	}

	void CInputMapper::TranslateActionToEvent(const EInputKey& action)
	{
		if (const auto eventIterator = Events.find(action); eventIterator != Events.end())
		{
			RunEvent(Events[action]);
		}
	}

	void CInputMapper::UpdateKeyboardInput()
	{
//		if (Input->IsKeyPressed(VK_SPACE))
//		{
//			TranslateActionToEvent(EInputKey::KeySpace);
//		}
//
//		if (Input->IsKeyPressed(VK_ESCAPE))
//		{
//			TranslateActionToEvent(EInputKey::KeyEscape);
//		}
//
//		if (Input->IsKeyDown(VK_SHIFT))
//		{
//			TranslateActionToEvent(EInputKey::KeyShiftDown);
//		}
//
//		if (Input->IsKeyReleased(VK_SHIFT))
//		{
//			TranslateActionToEvent(EInputKey::KeyShiftRelease);
//		}
//
//		if (Input->IsKeyDown('W'))
//		{
//			TranslateActionToEvent(EInputKey::KeyW);
//		}
//		if (Input->IsKeyDown('A'))
//		{
//			TranslateActionToEvent(EInputKey::KeyA);
//		}
//		if (Input->IsKeyDown('S'))
//		{
//			TranslateActionToEvent(EInputKey::KeyS);
//		}
//		if (Input->IsKeyDown('D'))
//		{
//			TranslateActionToEvent(EInputKey::KeyD);
//		}
//		if (Input->IsKeyPressed(VK_CONTROL))
//		{
//			TranslateActionToEvent(EInputKey::CTRL);
//		}
//#ifdef _DEBUG
//		if (Input->IsKeyPressed(VK_F5))
//		{
//			TranslateActionToEvent(EInputKey::KeyF5);
//		}
//		if (Input->IsKeyPressed(VK_F8))
//		{
//			TranslateActionToEvent(EInputKey::KeyF8);
//		}
//#endif // DEBUG
	}

	void CInputMapper::UpdateMouseInput()
	{
		//if (Input->IsMouseDown(CInput::EMouseButton::Middle))
		//{
		//	TranslateActionToEvent(EInputKey::MouseMiddle);
		//}
		//if (Input->IsMousePressed(CInput::EMouseButton::Left))
		//{
		//	TranslateActionToEvent(EInputKey::MouseLeftPressed);
		//}
		//if (Input->IsMouseDown(CInput::EMouseButton::Left))
		//{
		//	TranslateActionToEvent(EInputKey::MouseLeftDown);
		//}
		////else if (myInput->IsMouseDown(CInput::EMouseButton::Left))
		////{
		////	TranslateActionToEvent(EInputKey::MouseLeft);
		////}
		////else if (myInput->IsMouseReleased(CInput::EMouseButton::Left))
		////{
		////	TranslateActionToEvent(EInputKey::MouseLeft);
		////}
		//if (Input->IsMouseDown(CInput::EMouseButton::Right))
		//{
		//	TranslateActionToEvent(EInputKey::MouseRightDown);
		//}
	}

	void CInputMapper::Update()
	{
		UpdateKeyboardInput();
		UpdateMouseInput();
		Input->Update();
	}

	void CInputMapper::MapEvent(const EInputKey& inputEvent, const EInputActionEvent& outputEvent)
	{
		Events[inputEvent] = outputEvent;
	}

	bool CInputMapper::AddObserver(const EInputActionEvent& eventToListenFor, IInputObserver* observer)
	{
		ENGINE_ERROR_BOOL_MESSAGE(observer, "InputObsever is nullptr!");

		if (const auto it = std::ranges::find(Observers[eventToListenFor], observer); it == Observers[eventToListenFor].end())
			Observers[eventToListenFor].emplace_back(observer);

		return true;
	}

	bool CInputMapper::RemoveObserver(const EInputActionEvent& eventToListenFor, IInputObserver* observer)
	{
		ENGINE_ERROR_BOOL_MESSAGE(observer, "InputObsever is nullptr!");

		if (const auto it = std::ranges::find(Observers[eventToListenFor], observer); it != Observers[eventToListenFor].end())
			Observers[eventToListenFor].erase(it);

		return true;
	}

	bool CInputMapper::HasObserver(const EInputActionEvent& eventToListenFor, IInputObserver* observer)
	{
		const auto it = std::ranges::find(Observers[eventToListenFor], observer);
		return it != Observers[eventToListenFor].end();
	}

	void CInputMapper::ClearObserverList(const EInputActionEvent& eventToListenFor)
	{
		Observers[eventToListenFor].clear();
	}
}
