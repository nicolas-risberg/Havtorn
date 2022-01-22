// Copyright 2022 Team Havtorn. All Rights Reserved.

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
		void Render();

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
		template<class T>
		void BindBuffer(ID3D11Buffer* buffer, T& bufferData, std::string bufferType)
		{
			D3D11_MAPPED_SUBRESOURCE localBufferData;
			ZeroMemory(&localBufferData, sizeof(D3D11_MAPPED_SUBRESOURCE));
			std::string errorMessage = bufferType + " could not be bound.";
			ENGINE_HR_MESSAGE(Context->Map(buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &localBufferData), errorMessage.c_str());

			memcpy(localBufferData.pData, &bufferData, sizeof(T));
			Context->Unmap(buffer, 0);
		}

	private:
		struct SFrameBufferData
		{
			SMatrix ToCameraFromWorld;
			SMatrix ToWorldFromCamera;
			SMatrix ToProjectionFromCamera;
			SMatrix ToCameraFromProjection;
			SVector4 CameraPosition;
		} FrameBufferData;
		HV_ASSERT_BUFFER(SFrameBufferData)

		struct SObjectBufferData
		{
			SMatrix ToWorldFromObject;
		} ObjectBufferData;
		HV_ASSERT_BUFFER(SObjectBufferData)

	private:
		ID3D11DeviceContext* Context;
		ID3D11Buffer* FrameBuffer;
		ID3D11Buffer* ObjectBuffer;
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
		CFullscreenTexture IntermediateDepth;
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

		ID3D11VertexShader* DemoVertexShader;
		ID3D11PixelShader* DemoPixelShader;
		ID3D11SamplerState* SamplerState;
		ID3D11Buffer* DemoVertexBuffer;
		ID3D11Buffer* DemoIndexBuffer;
		ID3D11InputLayout* DemoInputLayout;
		U32 DemoNumberOfVertices;
		U32 DemoNumberOfIndices;
		U32 DemoStride;
		U32 DemoOffset;
	};
}
