// Copyright 2022 Team Havtorn. All Rights Reserved.

#include "hvpch.h"

#include "Application.h"
#include "Engine.h"
#include "WindowHandler.h"

namespace Havtorn
{
#define BIND_EVENT_FUNCTION(x) std::bind(&CApplication::x, this, std::placeholders::_1)

	CApplication::CApplication()
		: Engine(new CEngine())
	{
		CWindowHandler::SWindowData windowData;
		windowData.X = 100;
		windowData.Y = 100;
		windowData.Width = 1280;
		windowData.Height = 720;

		IsRunning = Engine->Init(windowData);
	}

	void CApplication::Run()
	{
		MSG windowMessage = { 0 };
		while (IsRunning)
		{
			while (PeekMessage(&windowMessage, 0, 0, 0, PM_REMOVE))
			{
				TranslateMessage(&windowMessage);
				DispatchMessage(&windowMessage);

				if (windowMessage.message == WM_QUIT)
					IsRunning = false;
			}

			Engine->BeginFrame();
			Engine->Update();
			Engine->RenderFrame();
			Engine->EndFrame();
		}
	}

	void CApplication::OnEvent(CEvent& e)
	{
		CEventDispatcher dispatcher(e);
		dispatcher.Dispatch<CWindowCloseEvent>(BIND_EVENT_FUNCTION(OnWindowClose));

		//HV_LOG_TRACE("{0}", e);

		for (auto it = LayerStack.end(); it != LayerStack.begin(); )
		{
			(*--it)->OnEvent(e);
			if (e.IsHandled)
				break;
		}
	}

	void CApplication::PushLayer(CLayer* layer)
	{
		LayerStack.PushLayer(layer);
	}

	void CApplication::PushOverlay(CLayer* overlay)
	{
		LayerStack.PushOverlay(overlay);
	}

	bool CApplication::OnWindowClose(CWindowCloseEvent& /*e*/)
	{
		IsRunning = false;
		return true;
	} 
}
