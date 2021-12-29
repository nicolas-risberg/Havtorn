#include "hvpch.h"
#include "ViewportWindow.h"
#include <imgui.h>
#include "Imgui/ImguiManager.h"
#include "Graphics/RenderManager.h"
#include "Graphics/FullscreenTexture.h"

namespace ImGui
{
	CViewportWindow::CViewportWindow(const char* aName, Havtorn::CImguiManager* manager)
		: CWindow(aName, manager)
		, RenderedSceneTextureReference(nullptr)
	{
	}

	CViewportWindow::~CViewportWindow()
	{
	}

	void CViewportWindow::OnEnable()
	{
	}

	void CViewportWindow::OnInspectorGUI()
	{
		const Havtorn::SEditorLayout& layout = Manager->GetEditorLayout();

		const ImGuiViewport* mainViewport = ImGui::GetMainViewport();
		ImGui::SetNextWindowPos(ImVec2(mainViewport->WorkPos.x + layout.ViewportPosition.X, mainViewport->WorkPos.y + layout.ViewportPosition.Y));
		ImGui::SetNextWindowSize(ImVec2(layout.ViewportSize.X, layout.ViewportSize.Y));

		if (ImGui::Begin(Name(), nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse))
		{
			RenderedSceneTextureReference = &(Manager->GetRenderManager()->GetRenderedSceneTexture());
			
			if (RenderedSceneTextureReference)
			{
				auto underlyingTexture = RenderedSceneTextureReference->GetTexture();
				D3D11_TEXTURE2D_DESC textureDesc;
				underlyingTexture->GetDesc(&textureDesc);
				Havtorn::F32 width = static_cast<Havtorn::F32>(textureDesc.Width);
				Havtorn::F32 height = static_cast<Havtorn::F32>(textureDesc.Height);
				ImGui::Image((void*)RenderedSceneTextureReference->GetShaderResourceView(), ImVec2(width, height));
			}
		}
		ImGui::End();
	}

	void CViewportWindow::OnDisable()
	{
	}
}