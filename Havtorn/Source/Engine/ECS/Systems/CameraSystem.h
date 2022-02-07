// Copyright 2022 Team Havtorn. All Rights Reserved.

#pragma once
#include "ECS/System.h"

namespace Havtorn 
{
	
	class CCameraSystem final : public CSystem {
	public:
		CCameraSystem();
		~CCameraSystem();

		void Update(CScene* scene) override;
		void CenterCamera(F32 axisValue);
		void ResetCamera(F32 axisValue) const;
		void TeleportCamera(F32 axisValue) const;
	private:

	};

}