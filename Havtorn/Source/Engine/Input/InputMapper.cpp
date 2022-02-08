// Copyright 2022 Team Havtorn. All Rights Reserved.

#include "InputMapper.h"

#include <ranges>

#include "Input.h"

namespace Havtorn
{
	CInputMapper::CInputMapper()
		: Input(CInput::GetInstance())
		, CurrentInputContext(EInputContext::Editor)
	{}

	bool CInputMapper::Init()
	{
		const SInputAction action = { EInputState::Pressed, EInputKey::KeyG, EInputContext::Editor, EInputModifier::Ctrl };
		MapEvent(EInputActionEvent::CenterCamera, action);

		SInputAction otherAction = { EInputState::Pressed, EInputKey::KeyK, EInputContext::Editor };
		otherAction.SetModifiers(3, EInputModifier::Shift, EInputModifier::Ctrl, EInputModifier::Alt);
		MapEvent(EInputActionEvent::ResetCamera, otherAction);

		SInputAction thirdAction = { EInputState::Pressed, EInputKey::KeyJ, EInputContext::Editor };
		thirdAction.SetModifiers(2, EInputModifier::Shift, EInputModifier::Alt);
		MapEvent(EInputActionEvent::TeleportCamera, thirdAction);

		const SInputAction fourthAction = {EInputState::Pressed, EInputKey::KeyH, EInputContext::Editor, EInputModifier::Shift};
		MapEvent(EInputActionEvent::TeleportCamera, fourthAction);

		const SInputAction fifthAction = { EInputState::Pressed, EInputKey::KeyL, {EInputContext::Editor, EInputContext::InGame}, {EInputModifier::Shift} };
		MapEvent(EInputActionEvent::CenterCamera, fifthAction);

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

	void CInputMapper::SetInputContext(EInputContext context)
	{
		CurrentInputContext = context;
	}

	void CInputMapper::MapEvent(EInputActionEvent event, SInputAction action)
	{
		if (!BoundActionEvents.contains(event))
			BoundActionEvents.emplace(event, SInputActionEvent(action));

		else
			BoundActionEvents[event].Actions.push_back(action);
	}

	void CInputMapper::UpdateKeyboardInput()
	{
		const auto& modifiers = Input->GetKeyInputModifiers().to_ulong();
		const auto& context = static_cast<U32>(CurrentInputContext);

		for (const auto& param : Input->GetKeyInputBuffer())
		{
			for (auto& val : BoundActionEvents | std::views::values)
			{
				if (val.Has(static_cast<EInputKey>(param), context, modifiers))
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
