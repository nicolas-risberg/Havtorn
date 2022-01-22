// Copyright 2022 Team Havtorn. All Rights Reserved.

#include "hvpch.h"

#include "Application.h"
#include "Log.h"
#include "Engine.h"
#include "WindowHandler.h"

namespace Havtorn
{

#define BIND_EVENT_FUNCTION(x) std::bind(&CApplication::x, this, std::placeholders::_1)

	CApplication::CApplication()
	{
		//Window = IWindow::Create();
		//Window->SetEventCallback(BIND_EVENT_FUNCTION(OnEvent));

		CWindowHandler::SWindowData windowData;
		windowData.myX = 100;
		windowData.myY = 100;
		windowData.myWidth = 1280;
		windowData.myHeight = 720;

		Engine = new CEngine();

		IsRunning = Engine->Init(windowData);
	}

	CApplication::~CApplication()
	{
	}

	void CApplication::Run()
	{
		//while (IsRunning)
		//{
		//	//for (CLayer* layer : LayerStack)
		//	//	layer->OnUpdate();

		//	//Window->OnUpdate();
		//	//if (GetAsyncKeyState('V'))
		//	//{
		//	//	IsRunning = false;
		//	//}
		//}
		//CWindowResizeEvent e(1280, 720);
		//HV_LOG_TRACE("{0}", e);

		MSG windowMessage = { 0 };
		while (IsRunning)
		{
			while (PeekMessage(&windowMessage, 0, 0, 0, PM_REMOVE))
			{
				TranslateMessage(&windowMessage);
				DispatchMessage(&windowMessage);

				if (windowMessage.message == WM_QUIT)
				{
					IsRunning = false;
				}
			}

			//PrintMemoryUsage();
			Engine->BeginFrame();
			//if (!game.Update())
			//	break;
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
