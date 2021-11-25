#pragma once

#include "Application\Window.h"

namespace Havtorn
{
	class CWindowsWindow : public IWindow
	{
	public:
		CWindowsWindow(const SWindowProperties& properties);
		virtual ~CWindowsWindow();

		void OnUpdate() override;

		inline U16 GetWidth() const override { return Data.Width; }
		inline U16 GetHeight() const override { return Data.Height; }

		inline void SetEventCallback(const EventCallbackFunction& callback) override { Data.EventCallback = callback; }
		void SetVSync(bool enabled) override;
		bool IsVSync() const override;

	private:
		virtual void Init(const SWindowProperties& properties);
		virtual void Shutdown();
		
	private:
		struct SWindowData
		{
			std::string Title;
			U16 Width, Height;
			bool IsVSyncEnabled;

			EventCallbackFunction EventCallback;
		};

		SWindowData Data;
	};
}