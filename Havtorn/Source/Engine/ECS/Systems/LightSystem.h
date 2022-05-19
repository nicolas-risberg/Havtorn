// Copyright 2022 Team Havtorn. All Rights Reserved.

#pragma once
#include "ECS/System.h"

namespace Havtorn
{

	class CLightSystem final : public CSystem {
	public:
		CLightSystem();
		~CLightSystem() override = default;

		void Update(CScene* scene) override;
	};
}
