#pragma once

#include "Core/Core.h"
#include "Event/Event.h"
#include "Window.h"

namespace Havtorn
{
	class HAVTORN_API CApplication
	{
	public:
		CApplication();
		virtual ~CApplication();
		
		CApplication(const CApplication& other) = delete;
		CApplication(const CApplication&& other) = delete;
		CApplication& operator=(const CApplication& other) = delete;
		CApplication& operator=(const CApplication&& other) = delete;

		void Run();
		
	private:
		Ptr<IWindow> Window;

		bool IsRunning = true;
	};

	// To be defined in Client
	CApplication* CreateApplication();
}
