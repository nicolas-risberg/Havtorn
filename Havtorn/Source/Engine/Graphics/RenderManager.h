#pragma once
#include "hvpch.h"
//#include "ForwardRenderer.h"
//#include "DeferredRenderer.h"
//#include "LightRenderer.h"
//#include "FullscreenRenderer.h"
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

namespace Havtorn
{
	class CDirectXFramework;
	class CWindowHandler;
	class CScene;

	class CRenderManager {
	public:
		CRenderManager();
		~CRenderManager();
		bool Init(CDirectXFramework* aFramework, CWindowHandler* aWindowHandler);
		bool ReInit(CDirectXFramework* aFramework, CWindowHandler* aWindowHandler);
		void Render(CScene& aScene);
		void Render();

		void Release();

	public:
		//void SetBrokenScreen(bool aShouldSetBrokenScreen);

		//const CFullscreenRenderer::SPostProcessingBufferData& GetPostProcessingBufferData() const;
		//void SetPostProcessingBufferData(const CFullscreenRenderer::SPostProcessingBufferData& someBufferData);

	public:
		static unsigned int myNumberOfDrawCallsThisFrame;

	private:
		void Clear(SVector4 clearColor);
		void InitRenderTextures(CWindowHandler* aWindowHandler);

	private:
		void RenderBloom();
		void RenderWithoutBloom();
		void ToggleRenderPass(bool aShouldToggleForwards = true);

	private:
		CRenderStateManager myRenderStateManager;
		//CForwardRenderer myForwardRenderer;
		//CDeferredRenderer myDeferredRenderer;
		//CLightRenderer myLightRenderer;
		//CFullscreenRenderer myFullscreenRenderer;
		//CParticleRenderer myParticleRenderer;
		//CVFXRenderer myVFXRenderer;
		//CSpriteRenderer mySpriteRenderer;
		//CTextRenderer myTextRenderer;
		//CShadowRenderer myShadowRenderer;
		//CDecalRenderer myDecalRenderer;

		CFullscreenTextureFactory myFullscreenTextureFactory;
		CFullscreenTexture myBackbuffer;
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

		SVector4 myClearColor;

		int myRenderPassIndex;
		// Effectively used to toggle renderpasses and bloom. True == enable bloom, full render. False == disable bloom, isolated render pass
		bool myDoFullRender;
		bool myUseAntiAliasing;
		bool myUseBrokenScreenPass;
	};
}
