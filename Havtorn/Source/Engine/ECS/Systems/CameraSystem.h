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

	private:

	};

}