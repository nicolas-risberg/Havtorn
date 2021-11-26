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
			for (CLayer* layer : LayerStack)
				layer->OnUpdate();

			Window->OnUpdate();
			if (GetAsyncKeyState('V'))
			{
				IsRunning = false;
			}
		}
		CWindowResizeEvent e(1280, 720);
		HV_LOG_TRACE(e);
	}

	void CApplication::OnEvent(CEvent& e)
	{
		CEventDispatcher dispatcher(e);
		dispatcher.Dispatch<CWindowCloseEvent>(BIND_EVENT_FUNCTION(OnWindowClose));

		HV_LOG_TRACE("{0}", e);

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

	bool CApplication::OnWindowClose(CWindowCloseEvent& e)
	{
		return false;
	}
}
