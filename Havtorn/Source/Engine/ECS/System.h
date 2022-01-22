// Copyright 2022 Team Havtorn. All Rights Reserved.

#pragma once

namespace Havtorn
{
	class CScene;

	class CSystem
	{
	public:
		CSystem() = default;
		virtual ~CSystem() = default;

		virtual void Update(CScene* scene) = 0;
	};
}
