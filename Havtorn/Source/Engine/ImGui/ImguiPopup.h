#pragma once

namespace ImGui
{
	class CPopup
	{
	public:
		virtual ~CPopup() { }
		CPopup(const char* aPopupName);
	public:
		virtual void OnEnable() = 0;
		virtual void OnInspectorGUI() = 0;
		virtual void OnDisable() = 0;

	public:
		inline const char* Name() { return myName; }
		inline void Enable(const bool aEnable) { myIsEnabled = aEnable; }
		inline const bool Enable() const { return myIsEnabled; }

	protected:
		bool* Open() { return &myIsEnabled; }

	private:
		const char* myName;
		bool myIsEnabled;
	};
}