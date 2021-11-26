#pragma once

#include "Core/WindowsInclude.h"

namespace Havtorn
{
	class CWindowHandler
	{
		friend class CEngine;

	public:
		struct SWindowData
		{
			U16 myX;
			U16 myY;
			U16 myWidth;
			U16 myHeight;
		};

		static LRESULT CALLBACK WinProc(_In_ HWND hwnd, _In_ UINT uMsg, _In_ WPARAM wParam, _In_ LPARAM lParam);

		CWindowHandler();
		~CWindowHandler();

		const HWND GetWindowHandle() const;
		SVector2<F32> GetCenterPosition();
		SVector2<F32> GetResolution();
		const float GetResolutionScale() const;
		void SetWindowTitle(std::string aString);

		const bool CursorLocked() const;
		void LockCursor(bool aShouldLock);
		void HidLockCursor(bool aShouldLock);

		void HideAndLockCursor(const bool& anIsInEditorMode = false);
		void ShowAndUnlockCursor(const bool& anIsInEditorMode = true);

	private:
		bool Init(CWindowHandler::SWindowData someWindowData);
		void SetInternalResolution();
		void SetResolution(SVector2<F32> aResolution);

	private:
		CWindowHandler::SWindowData myWindowData;
		HWND myWindowHandle;
		SVector2<F32>* myResolution;
		F32 myResolutionScale;
		U16 myMaxResX = 1920;
		U16 myMaxResY = 1080;
		bool myCursorIsLocked;
		bool myWindowIsInEditingMode;
	};
}
