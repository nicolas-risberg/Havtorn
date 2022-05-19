// Copyright 2022 Team Havtorn. All Rights Reserved.

#pragma once
#include "ECS/System.h"
#include "Input/InputTypes.h"

namespace Havtorn 
{
	class CCameraSystem final : public CSystem {
	public:
		CCameraSystem();
		~CCameraSystem() override;

		void Update(CScene* scene) override;
		void HandleAxisInput(const SInputAxisPayload payload);

	private:
		SVector CameraMoveInput = SVector::Zero;
		SVector CameraRotateInput = SVector::Zero;
	};
}
