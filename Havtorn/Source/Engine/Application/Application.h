#pragma once

#include "Core/Core.h"
#include "Event/Event.h"

namespace Havtorn
{
	class HAVTORN_API CApplication
	{
	public:
		CApplication();
		virtual ~CApplication();

		void Run();
	};

	// To be defined in Client
	CApplication* CreateApplication();
}
