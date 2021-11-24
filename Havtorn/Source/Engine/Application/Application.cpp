#include "hvpch.h"

#include "Application.h"
#include "Event\ApplicationEvent.h"
#include "Log.h"

namespace Havtorn
{
	CApplication::CApplication()
	{
	}

	CApplication::~CApplication()
	{
	}
	
	void CApplication::Run()
	{
		CWindowResizeEvent e(1280, 720);
		HV_LOG_TRACE(e);
	}
}
