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