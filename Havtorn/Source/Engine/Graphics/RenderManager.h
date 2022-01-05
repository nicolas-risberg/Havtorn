#pragma once
#include "hvpch.h"
//#include "ForwardRenderer.h"
//#include "DeferredRenderer.h"
//#include "LightRenderer.h"
#include "Renderers/FullscreenRenderer.h"
#include "FullscreenTexture.h"
#include "FullscreenTextureFactory.h"
//#include "ParticleRenderer.h"
//#include "VFXRenderer.h"
#include "RenderStateManager.h"
//#include "SpriteRenderer.h"
//#include "TextRenderer.h"
//#include "ShadowRenderer.h"
//#include "DecalRenderer.h"
//#include "GBuffer.h"
#include <queue>

namespace Havtorn
{
	class CDirectXFramework;
	class CWindowHandler;
	class CScene;
	struct SRenderCommand;

	struct SRenderCommandComparer
	{
		bool operator()(const SRenderCommand& a, const SRenderCommand& b);
	};

	class CRenderManager 
	{
	public:
		CRenderManager();
		~CRenderManager();
		bool Init(CDirectXFramework* framework, CWindowHandler* windowHandler);
		bool ReInit(CDirectXFramework* framework, CWindowHandler* windowHandler);
		void Render(CScene& scene);
		void sRender();

		void Release();

	public:
		const CFullscreenTexture& GetRenderedSceneTexture() const;
		void PushRenderCommand(SRenderCommand& command);
		//void SetBrokenScreen(bool aShouldSetBrokenScreen);

		//const CFullscreenRenderer::SPostProcessingBufferData& GetPostProcessingBufferData() const;
		//void SetPostProcessingBufferData(const CFullscreenRenderer::SPostProcessingBufferData& someBufferData);

	public:
		static U32 NumberOfDrawCallsThisFrame;

	private:
		void Clear(SVector4 clearColor);
		void InitRenderTextures(CWindowHandler* windowHandler);

	private:
		void RenderBloom();
		void RenderWithoutBloom();
		void ToggleRenderPass(bool shouldToggleForwards = true);

	private:
		CRenderStateManager RenderStateManager;
		//CForwardRenderer ForwardRenderer;
		//CDeferredRenderer myDeferredRenderer;
		//CLightRenderer myLightRenderer;
		CFullscreenRenderer FullscreenRenderer;
		//CParticleRenderer myParticleRenderer;
		//CVFXRenderer myVFXRenderer;
		//CSpriteRenderer mySpriteRenderer;
		//CTextRenderer myTextRenderer;
		//CShadowRenderer myShadowRenderer;
		//CDecalRenderer myDecalRenderer;

		CFullscreenTextureFactory FullscreenTextureFactory;
		CFullscreenTexture RenderedScene;
		CFullscreenTexture Backbuffer;
		//CFullscreenTexture myIntermediateTexture;
		//CFullscreenTexture myIntermediateDepth;
		//CFullscreenTexture myEnvironmentShadowDepth;
		//CFullscreenTexture myBoxLightShadowDepth;
		//CFullscreenTexture myDepthCopy;
		//CFullscreenTexture myLuminanceTexture;
		//CFullscreenTexture myHalfSizeTexture;
		//CFullscreenTexture myQuarterSizeTexture;
		//CFullscreenTexture myBlurTexture1;
		//CFullscreenTexture myBlurTexture2;
		//CFullscreenTexture myVignetteTexture;
		//CFullscreenTexture myVignetteOverlayTexture;
		//CFullscreenTexture myDeferredLightingTexture;
		//CFullscreenTexture myVolumetricAccumulationBuffer;
		//CFullscreenTexture myVolumetricBlurTexture;
		//CFullscreenTexture mySSAOBuffer;
		//CFullscreenTexture mySSAOBlurTexture;
		//CFullscreenTexture myDownsampledDepth;
		//CFullscreenTexture myTonemappedTexture;
		//CFullscreenTexture myAntiAliasedTexture;
		//CGBuffer myGBuffer;
		//CGBuffer myGBufferCopy;
		std::priority_queue<SRenderCommand, std::vector<SRenderCommand>, SRenderCommandComparer> RenderCommands;

		SVector4 ClearColor;

		I8 RenderPassIndex;
		// Effectively used to toggle renderpasses and bloom. True == enable bloom, full render. False == disable bloom, isolated render pass
		bool DoFullRender;
		bool UseAntiAliasing;
		bool UseBrokenScreenPass;


	};
}
