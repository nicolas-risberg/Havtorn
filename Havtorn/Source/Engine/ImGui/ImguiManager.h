#pragma once

//#include "Observer.h"

struct ID3D11Device;
struct ID3D11DeviceContext;

struct ImFontAtlas;
struct ImVec4;

namespace ImGui 
{
	class CWindow;
	class CPopup;
}

namespace Havtorn
{
	enum class EEditorColorTheme
	{
		DefaultDark,
		HavtornDark,
		HavtornRed,
		HavtornGreen,
		Count
	};

	enum class EEditorStyleTheme
	{
		Default,
		Havtorn,
		Count
	};

	class CImguiManager
		//: public IObserver
	{
	public:
		CImguiManager();
		~CImguiManager();

		bool Init(ID3D11Device* device, ID3D11DeviceContext* context, HWND windowHandle);
		void BeginFrame();
		void Update();
		void EndFrame();
		void DebugWindow();

	public://Inherited
	//void Receive(const SMessage& aMessage) override;
		static void SetEditorTheme(EEditorColorTheme colorTheme = EEditorColorTheme::HavtornDark, EEditorStyleTheme styleTheme = EEditorStyleTheme::Havtorn);
		static std::string GetEditorColorThemeName(EEditorColorTheme colorTheme);
		static ::ImVec4 GetEditorColorThemeRepColor(EEditorColorTheme colorTheme);
	private:
		std::vector<Ptr<ImGui::CWindow>> myWindows;
		std::vector<Ptr<ImGui::CPopup>> myPopups;

		const std::string GetSystemMemory();
		const std::string GetDrawCalls();

		bool myIsEnabled;
	};
}
