#include "hvpch.h"

#include "Application.h"
#include "Event/ApplicationEvent.h"
#include "Log.h"

namespace Havtorn
{
	CApplication::CApplication()
	{
		Window = Ptr<IWindow>(IWindow::Create());
	}

	CApplication::~CApplication()
	{
	}
	
	void CApplication::Run()
	{
		while (IsRunning)
		{
			Window->OnUpdate();
			if (GetAsyncKeyState('V'))
			{
				IsRunning = false;
			}
		}
		CWindowResizeEvent e(1280, 720);
		HV_LOG_TRACE(e);
	}
}
