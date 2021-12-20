#include "hvpch.h"

#include "WindowsWindow.h"
#include "Event/ApplicationEvent.h"
#include "Event/KeyEvent.h"
#include "Event/MouseEvent.h"

namespace Havtorn
{
	static bool FrameworkInitialized = false;

	IWindow * IWindow::Create(const SWindowProperties & properties)
	{
		return new CWindowsWindow(properties);
	}

	CWindowsWindow::CWindowsWindow(const SWindowProperties& properties)
	{
		Init(properties);
	}

	CWindowsWindow::~CWindowsWindow()
	{
		Shutdown();
	}

	void CWindowsWindow::OnUpdate()
	{
	}

	void CWindowsWindow::SetVSync(bool enabled)
	{
		Data.IsVSyncEnabled = enabled;
	}

	bool CWindowsWindow::IsVSync() const
	{
		return false;
	}

	void CWindowsWindow::Init(const SWindowProperties& properties)
	{
		Data.Title = properties.Title;
		Data.Width = properties.Width;
		Data.Height = properties.Height;
		
		SetVSync(false);

		HV_LOG_INFO("Creating window {0} ({1}, {2})", properties.Title, properties.Width, properties.Height);
	}

	void CWindowsWindow::Shutdown()
	{
	}
}
