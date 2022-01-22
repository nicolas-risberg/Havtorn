// Copyright 2022 Team Havtorn. All Rights Reserved.

#pragma once

#include "Core/Core.h"

#include "Window.h"
#include "Event/Event.h"
#include "Event/ApplicationEvent.h"
#include "LayerStack.h"

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

		void OnEvent(CEvent& e);

		void PushLayer(CLayer* layer);
		void PushOverlay(CLayer* overlay);
		
	private:
		bool OnWindowClose(CWindowCloseEvent& e);

	private:
		class CEngine* Engine;
		IWindow* Window;
		bool IsRunning = true;
		CLayerStack LayerStack;
	};

	// To be defined in Client
	CApplication* CreateApplication();
}
