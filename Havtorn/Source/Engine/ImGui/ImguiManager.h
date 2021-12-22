#pragma once

//#include "Observer.h"

struct ID3D11Device;
struct ID3D11DeviceContext;

struct ImFontAtlas;

namespace ImGui {
	class CWindow;
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
	private:
		void SetEditorStyle(EEditorColorTheme colorTheme = EEditorColorTheme::DefaultDark);

	private:
		std::vector<Ptr<ImGui::CWindow>> myWindows;

		const std::string GetSystemMemory();
		const std::string GetDrawCalls();

		bool myIsEnabled;
	};
}
