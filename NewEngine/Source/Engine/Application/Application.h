#pragma once

#include "../Core/Core.h"

namespace NewEngine
{
	class NEW_ENGINE_API CApplication
	{
	public:
		CApplication();
		virtual ~CApplication();

		void Run();
	};

	// To be defined in Client
	CApplication* CreateApplication();
}
