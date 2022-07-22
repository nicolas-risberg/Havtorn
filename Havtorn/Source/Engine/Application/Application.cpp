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

	/*
	CProcess
		Init - Runs once
		BeginFrame()
		PreUpdate
		Update
		PostUpdate
		EndFrame


	Engine: CProcess

	Application.Processes[]

	Application.Run()
		while(isRunning)
			foreach Process
				p.BeginFrame
			foreach Process
				p.PreUpdate
			foreach Process
				p.Update
			...


	
	*/

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

}
