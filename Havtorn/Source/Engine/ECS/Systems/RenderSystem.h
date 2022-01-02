#pragma once
#include "ECS/System.h"

namespace Havtorn
{
	class CRenderSystem final : public CSystem
	{
	public:
		CRenderSystem();
		~CRenderSystem();

		void Update(CScene* scene) override;
	private:

	};
}