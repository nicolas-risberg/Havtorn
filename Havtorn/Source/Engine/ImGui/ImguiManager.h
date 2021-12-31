#pragma once
//#include "Observer.h"

struct ID3D11Device;
struct ID3D11DeviceContext;

struct ImFontAtlas;
struct ImVec4;

namespace ImGui 
{
	class CWindow;
	class CToggleable;
}

namespace Havtorn
{
	class CDirectXFramework;
	class CWindowHandler;
	class CRenderManager;

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

	struct SEditorLayout
	{
		SVector2<I16> ViewportPosition		= SVector2<I16>::Zero;
		SVector2<U16> ViewportSize			= SVector2<U16>::Zero;
		SVector2<I16> AssetBrowserPosition	= SVector2<I16>::Zero;
		SVector2<U16> AssetBrowserSize		= SVector2<U16>::Zero;
		SVector2<I16> HierarchyViewPosition	= SVector2<I16>::Zero;
		SVector2<U16> HierarchyViewSize		= SVector2<U16>::Zero;
		SVector2<I16> InspectorPosition		= SVector2<I16>::Zero;
		SVector2<U16> InspectorSize			= SVector2<U16>::Zero;
	};

	struct SEditorColorProfile
	{
		SVector4 BackgroundBase = SVector4::Zero;
		SVector4 BackgroundMid = SVector4::Zero;
		SVector4 ElementBackground = SVector4::Zero;
		SVector4 ElementHovered = SVector4::Zero;
		SVector4 ElementActive = SVector4::Zero;
		SVector4 ElementHighlight = SVector4::Zero;
	};

	class CImguiManager
		//: public IObserver
	{
	public:
		CImguiManager();
		~CImguiManager();

		bool Init(CDirectXFramework* framework, CWindowHandler* windowHandler, const CRenderManager* renderManager);
		void BeginFrame();
		void Update();
		void EndFrame();
		void DebugWindow();

	public://Inherited
	//void Receive(const SMessage& aMessage) override;
		void SetEditorTheme(EEditorColorTheme colorTheme = EEditorColorTheme::HavtornDark, EEditorStyleTheme styleTheme = EEditorStyleTheme::Havtorn);
		std::string GetEditorColorThemeName(EEditorColorTheme colorTheme);
		::ImVec4 GetEditorColorThemeRepColor(EEditorColorTheme colorTheme);
		const SEditorLayout& GetEditorLayout() const;

		F32 GetViewportPadding() const;
		void SetViewportPadding(F32 padding);
	
		const CRenderManager* GetRenderManager() const;

	private:
		void InitEditorLayout(); 
		void SetEditorColorProfile(const SEditorColorProfile& colorProfile);

		const std::string GetFrameRate();
		const std::string GetSystemMemory();
		const std::string GetDrawCalls();

	private:
		const CRenderManager* RenderManager;

		std::vector<Ptr<ImGui::CWindow>> Windows;
		std::vector<Ptr<ImGui::CToggleable>> MenuElements;

		SEditorLayout EditorLayout;
		SEditorColorProfile EditorColorProfile;

		F32 ViewportPadding;
		bool IsEnabled;
	};
}
