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
		void CenterCamera(SInputPayload payload);
		void ResetCamera(SInputPayload payload) const;
		void TeleportCamera(SInputPayload payload) const;
	private:

	};

}