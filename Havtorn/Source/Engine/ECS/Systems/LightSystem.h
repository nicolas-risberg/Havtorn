// Copyright 2022 Team Havtorn. All Rights Reserved.

#pragma once
#include "ECS/System.h"

namespace Havtorn
{
	class CRenderManager;

	class CLightSystem final : public CSystem 
	{
	public:
		CLightSystem(CRenderManager* renderManager);
		~CLightSystem() override = default;

		void Update(CScene* scene) override;
	private:
		CRenderManager* RenderManager;
	};
}
