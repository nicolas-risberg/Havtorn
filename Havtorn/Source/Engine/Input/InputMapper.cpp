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
		// TODO.NR: Make Input Axis events broadcast payloads as well,
		// make payloads general so we don't need so many functions binding to the delegates.

		const SInputAxis forwardAxis = { EInputAxis::Key, EInputKey::KeyW, EInputKey::KeyS, EInputContext::Editor };
		MapEvent(EInputAxisEvent::Forward, forwardAxis);

		const SInputAxis rightAxis = { EInputAxis::Key, EInputKey::KeyD, EInputKey::KeyA, EInputContext::Editor };
		MapEvent(EInputAxisEvent::Right, rightAxis);

		const SInputAxis upAxis = { EInputAxis::Key, EInputKey::KeyQ, EInputKey::KeyE, EInputContext::Editor };
		MapEvent(EInputAxisEvent::Up, upAxis);

		const SInputAxis pitchAxis = { EInputAxis::Key, EInputKey::Up, EInputKey::Down, EInputContext::Editor };
		MapEvent(EInputAxisEvent::Pitch, pitchAxis);

		const SInputAxis yawAxis = { EInputAxis::Key, EInputKey::Left, EInputKey::Right, EInputContext::Editor };
		MapEvent(EInputAxisEvent::Yaw, yawAxis);

		return true;
	}

	void CInputMapper::Update()
	{
		UpdateKeyboardInput();
		UpdateMouseInput();
		Input->UpdateState();
	}

	CMulticastDelegate<const SInputActionPayload>& CInputMapper::GetActionDelegate(EInputActionEvent event)
	{
		HV_ASSERT(BoundActionEvents.contains(event), "There is no such Input Action Event bound!");
		return BoundActionEvents[event].Delegate;
	}

	CMulticastDelegate<const SInputAxisPayload>& CInputMapper::GetAxisDelegate(EInputAxisEvent event)
	{
		HV_ASSERT(BoundAxisEvents.contains(event), "There is no such Input Axis Event bound!");
		return BoundAxisEvents[event].Delegate;
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

	void CInputMapper::MapEvent(EInputAxisEvent event, SInputAxis axisAction)
	{
		if (!BoundAxisEvents.contains(event))
			BoundAxisEvents.emplace(event, SInputAxisEvent(axisAction));

		else
			BoundAxisEvents[event].Axes.push_back(axisAction);
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

			// Key Axes
			for (auto& val : BoundAxisEvents)
			{
				if (val.second.HasKeyAxis())
				{
					F32 axisValue = 0.0f;
					if (val.second.Has(static_cast<EInputKey>(param.first), context, modifiers, axisValue))
					{
						const SInputAxisPayload payload = { val.first, axisValue };
						val.second.Delegate.Broadcast(payload);
					}
				}
			}
		}

		if (Input->GetMouseWheelDelta() != 0)
			HV_LOG_WARN("Wheel Delta: %i", Input->GetMouseWheelDelta());
		//HV_LOG_WARN("Axis Raw: %s", Input->GetAxisRaw().ToString().c_str());
		//HV_LOG_WARN("Raw Delta X: %f, Y: %f", Input->GetMouseRawDeltaX(), Input->GetMouseRawDeltaY());
	}

	void CInputMapper::UpdateMouseInput()
	{

	}
}
