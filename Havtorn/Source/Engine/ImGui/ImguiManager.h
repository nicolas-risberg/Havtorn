#pragma once

#ifdef _DEBUG
//#include "Observer.h"
#endif // DEBUG

struct ID3D11Device;
struct ID3D11DeviceContext;

struct ImFontAtlas;

namespace ImGui {
	class CWindow;
}

namespace Havtorn
{

	class CImguiManager
#ifdef _DEBUG
		//: public IObserver
#endif
	{
	public:
		CImguiManager();
		~CImguiManager();

		bool Init(ID3D11Device* device, ID3D11DeviceContext* context, HWND windowHandle);
		void Update();
		void DebugWindow();

	public://Inherited
#ifdef _DEBUG
	//void Receive(const SMessage& aMessage) override;
#endif
	private:

#ifdef _DEBUG
		std::vector<std::unique_ptr<ImGui::CWindow>> myWindows;
#endif

		const std::string GetSystemMemory();
		const std::string GetDrawCalls();

		bool myIsEnabled;
	};
}
