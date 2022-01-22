// Copyright 2022 Team Havtorn. All Rights Reserved.

#pragma once
#include "ECS/System.h"

namespace Havtorn
{
	class CRenderManager;

	class CRenderSystem final : public CSystem
	{
	public:
		CRenderSystem(CRenderManager* renderManager);
		~CRenderSystem();

		void Update(CScene* scene) override;
	private:
		CRenderManager* RenderManager;
	};
}