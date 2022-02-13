// Copyright 2022 Team Havtorn. All Rights Reserved.

#pragma once
#include "ECS/System.h"

#include "Input/InputObserver.h"

namespace Havtorn 
{
	
	class CCameraSystem final : public CSystem {
	public:
		CCameraSystem();
		~CCameraSystem();

		void Update(CScene* scene) override;
		void MoveUp(F32 value);
		void MoveRight(F32 value);
		void MoveForward(F32 value);
		void RotatePitch(F32 value);
		void RotateYaw(F32 value);
		void CenterCamera(const SInputPayload payload);
		void ResetCamera(const SInputPayload payload) const;
		void TeleportCamera(const SInputPayload payload) const;
	private:
		SVector CameraMoveInput;
		SVector CameraRotateInput;
	};
}
