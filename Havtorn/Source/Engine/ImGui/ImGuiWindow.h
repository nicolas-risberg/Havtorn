// Copyright 2022 Team Havtorn. All Rights Reserved.

#pragma once

namespace Havtorn
{
	class CImguiManager;
}

namespace ImGui
{
	class CWindow
	{
	public:
		virtual ~CWindow() { }
		CWindow(const char* displayName, Havtorn::CImguiManager* manager);
	public:
		virtual void OnEnable() = 0;
		virtual void OnInspectorGUI() = 0;
		virtual void OnDisable() = 0;

	public:
		inline const char* Name() { return DisplayName; }
		inline void Enable(const bool enable) { IsEnabled = enable; }
		inline const bool Enable() const { return IsEnabled; }

	protected:
		bool* Open() { return &IsEnabled; }

	protected:
		Havtorn::Ref<Havtorn::CImguiManager> Manager;

	private:
		const char* DisplayName;
		bool IsEnabled;
	};
}
