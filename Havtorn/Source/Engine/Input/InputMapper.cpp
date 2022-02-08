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
		const SInputAction action = { EInputKey::KeyH, EInputContext::Editor };
		MapEvent(EInputActionEvent::CenterCamera, action);

		const SInputAction otherAction = { EInputKey::KeyG, EInputContext::Editor };
		MapEvent(EInputActionEvent::ResetCamera, otherAction);

		const SInputAction thirdAction = { EInputKey::KeyJ, EInputContext::Editor };
		MapEvent(EInputActionEvent::TeleportCamera, thirdAction);

		return true;
	}

	void CInputMapper::Update()
	{
		UpdateKeyboardInput();
		UpdateMouseInput();
		Input->UpdateState();
	}

	CInputDelegate<SInputPayload>& CInputMapper::GetActionDelegate(EInputActionEvent event)
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
				if (val.Has(static_cast<EInputKey>(param.first), context, modifiers))
				{
					val.Delegate.Broadcast(param.second);
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
