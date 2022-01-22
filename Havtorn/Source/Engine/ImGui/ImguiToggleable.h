// Copyright 2022 Team Havtorn. All Rights Reserved.

#pragma once

namespace Havtorn
{
	class CImguiManager;
}

namespace ImGui
{
	class CToggleable
	{
	public:
		virtual ~CToggleable() = default;
		CToggleable(const char* displayName, Havtorn::CImguiManager* manager);
	public:
		virtual void OnEnable() = 0;
		virtual void OnInspectorGUI() = 0;
		virtual void OnDisable() = 0;

	public:
		[[nodiscard]] inline const char* Name() const { return DisplayName; }
		inline void Enable(const bool enable) { IsEnabled = enable; }
		[[nodiscard]] inline bool Enable() const { return IsEnabled; }

	protected:
		bool* Open() { return &IsEnabled; }

	protected:
		Havtorn::Ref<Havtorn::CImguiManager> Manager;
	
	private:
		const char* DisplayName;
		bool IsEnabled;
	};
}