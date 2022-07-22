// Copyright 2022 Team Havtorn. All Rights Reserved.

#pragma once

#include "Core/Core.h"

#include "Event/Event.h"
#include "Event/ApplicationEvent.h"

namespace Havtorn
{
	class HAVTORN_API CApplication
	{
	public:
		CApplication();
		virtual ~CApplication() = default;
		
		CApplication(const CApplication& other) = delete;
		CApplication(const CApplication&& other) = delete;
		CApplication& operator=(const CApplication& other) = delete;
		CApplication& operator=(const CApplication&& other) = delete;

		void Run();

	private:
		class CEngine* Engine;
		bool IsRunning = true;
	};

	// To be defined in Client
	CApplication* CreateApplication();
}
