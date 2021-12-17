#include "hvpch.h"

#include "Application.h"
#include "Log.h"

namespace Havtorn
{

#define BIND_EVENT_FUNCTION(x) std::bind(&CApplication::x, this, std::placeholders::_1)

	CApplication::CApplication()
	{
		Window = IWindow::Create();
		Window->SetEventCallback(BIND_EVENT_FUNCTION(OnEvent));
	}

	CApplication::~CApplication()
	{
	}

	void CApplication::Run()
	{
		while (IsRunning)
		{
			for (CLayer* layer : LayerStack)
				layer->OnUpdate();

			Window->OnUpdate();
			if (GetAsyncKeyState('V'))
			{
				IsRunning = false;
			}
		}
		CWindowResizeEvent e(1280, 720);
		//HV_LOG_TRACE("{0}", e);
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
