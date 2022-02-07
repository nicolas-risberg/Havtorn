// Copyright 2022 Team Havtorn. All Rights Reserved.

#include "InputMapper.h"

#include <ranges>

#include "Input.h"

namespace Havtorn
{
	CInputMapper::CInputMapper() : Input(CInput::GetInstance())
	{}

	bool CInputMapper::Init()
	{
		const SInputAction action = { EInputState::Pressed, EInputModifier::Ctrl, EInputKey::KeyG };
		MapEvent(EInputActionEvent::CenterCamera, action, EInputContext::Editor);

		SInputAction otherAction = { EInputState::Pressed, EInputKey::KeyK };
		otherAction.SetModifiers(3, EInputModifier::Shift, EInputModifier::Ctrl, EInputModifier::Alt);
		MapEvent(EInputActionEvent::ResetCamera, otherAction, EInputContext::Editor);

		SInputAction thirdAction = { EInputState::Pressed, EInputKey::KeyJ };
		thirdAction.SetModifiers(2, EInputModifier::Shift, EInputModifier::Alt);
		MapEvent(EInputActionEvent::TeleportCamera, thirdAction, EInputContext::Editor);

		return true;
	}

	void CInputMapper::Update()
	{
		UpdateKeyboardInput();
		UpdateMouseInput();
		Input->Update();
	}

	CInputDelegate<F32>& CInputMapper::GetActionDelegate(EInputActionEvent event)
	{
		HV_ASSERT(BoundActionEvents.contains(event), "There is no such Input Action Event bound!");
		return BoundActionEvents[event].Delegate;
	}

	void CInputMapper::MapEvent(EInputActionEvent event, SInputAction action, const EInputContext& /*context*/)
	{
		if (!BoundActionEvents.contains(event))
			BoundActionEvents.emplace(event, SInputActionEvent(action));

		else
			BoundActionEvents[event].Actions.push_back(action);
	}

	void CInputMapper::UpdateKeyboardInput()
	{
		const auto& modifiers = Input->GetKeyInputModifiers();
		for (const auto& param : Input->GetKeyInputBuffer())
		{
			for (auto& val : BoundActionEvents | std::views::values)
			{
				if (val.HasKey(static_cast<EInputKey>(param)) && val.HasModifiers(modifiers.to_ulong()))
				{
					val.Delegate.Broadcast(1.0f);
				}
			}
		}
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
}
