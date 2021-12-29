#include "hvpch.h"
#include "RenderManager.h"
#include "GraphicsFramework.h"
//#include "Scene.h"
//#include "LineInstance.h"
//#include "ModelFactory.h"
//#include "GameObject.h"
//#include "TransformComponent.h"
//#include "CameraComponent.h"
//#include "ModelComponent.h"
//#include "InstancedModelComponent.h"
//#include "MainSingleton.h"
//#include "PopupTextService.h"
//#include "DialogueSystem.h"
//#include "Canvas.h"

#include "Engine.h"

//#include "BoxLightComponent.h"
//#include "BoxLight.h"

#include <algorithm>

namespace Havtorn
{
	unsigned int CRenderManager::NumberOfDrawCallsThisFrame = 0;

	CRenderManager::CRenderManager()
		: DoFullRender(true)
		, UseAntiAliasing(true)
		, UseBrokenScreenPass(false)
		, ClearColor(0.5f, 0.5f, 0.5f, 1.0f)
		, RenderPassIndex(0)
	{
	}

	CRenderManager::~CRenderManager()
	{
		Release();
	}

	bool CRenderManager::Init(CDirectXFramework* framework, CWindowHandler* windowHandler)
	{
		//ENGINE_ERROR_BOOL_MESSAGE(ForwardRenderer.Init(aFramework), "Failed to Init Forward Renderer.");
		//ENGINE_ERROR_BOOL_MESSAGE(myLightRenderer.Init(aFramework), "Failed to Init Light Renderer.");
		//ENGINE_ERROR_BOOL_MESSAGE(myDeferredRenderer.Init(aFramework, &CMainSingleton::MaterialHandler()), "Failed to Init Deferred Renderer.");
		//ENGINE_ERROR_BOOL_MESSAGE(myFullscreenRenderer.Init(aFramework), "Failed to Init Fullscreen Renderer.");
		ENGINE_ERROR_BOOL_MESSAGE(FullscreenTextureFactory.Init(framework), "Failed to Init Fullscreen Texture Factory.");
		//ENGINE_ERROR_BOOL_MESSAGE(myParticleRenderer.Init(aFramework), "Failed to Init Particle Renderer.");
		ENGINE_ERROR_BOOL_MESSAGE(RenderStateManager.Init(framework), "Failed to Init Render State Manager.");
		//ENGINE_ERROR_BOOL_MESSAGE(myVFXRenderer.Init(aFramework), "Failed to Init VFX Renderer.");
		//ENGINE_ERROR_BOOL_MESSAGE(mySpriteRenderer.Init(aFramework), "Failed to Init Sprite Renderer.");
		//ENGINE_ERROR_BOOL_MESSAGE(myTextRenderer.Init(aFramework), "Failed to Init Text Renderer.");
		//ENGINE_ERROR_BOOL_MESSAGE(myShadowRenderer.Init(aFramework), "Failed to Init Shadow Renderer.");
		//ENGINE_ERROR_BOOL_MESSAGE(myDecalRenderer.Init(aFramework), "Failed to Init Decal Renderer.");

		ID3D11Texture2D* backbufferTexture = framework->GetBackbufferTexture();
		ENGINE_ERROR_BOOL_MESSAGE(backbufferTexture, "Backbuffer Texture is null.");

		RenderedScene = FullscreenTextureFactory.CreateTexture(backbufferTexture);
		Backbuffer = FullscreenTextureFactory.CreateTexture(backbufferTexture);
		InitRenderTextures(windowHandler);

		return true;
	}

	bool CRenderManager::ReInit(CDirectXFramework* framework, CWindowHandler* windowHandler)
	{
		ENGINE_ERROR_BOOL_MESSAGE(RenderStateManager.Init(framework), "Failed to Init Render State Manager.");

		ID3D11Texture2D* backbufferTexture = framework->GetBackbufferTexture();
		ENGINE_ERROR_BOOL_MESSAGE(backbufferTexture, "Backbuffer Texture is null.");

		//RenderedScene = FullscreenTextureFactory.CreateTexture(backbufferTexture);
		Backbuffer = FullscreenTextureFactory.CreateTexture(backbufferTexture);
		InitRenderTextures(windowHandler);

		return true;
	}

	void CRenderManager::InitRenderTextures(CWindowHandler* windowHandler)
	{
		RenderedScene = FullscreenTextureFactory.CreateTexture(windowHandler->GetResolution(), DXGI_FORMAT_R16G16B16A16_FLOAT);
		//myIntermediateDepth = FullscreenTextureFactory.CreateDepth(aWindowHandler->GetResolution(), DXGI_FORMAT_R24G8_TYPELESS);
		//myEnvironmentShadowDepth = FullscreenTextureFactory.CreateDepth({ 2048.0f * 1.0f, 2048.0f * 1.0f }, DXGI_FORMAT_R32_TYPELESS);
		//myBoxLightShadowDepth = FullscreenTextureFactory.CreateDepth(aWindowHandler->GetResolution(), DXGI_FORMAT_R32_TYPELESS);
		//myDepthCopy = FullscreenTextureFactory.CreateTexture(aWindowHandler->GetResolution(), DXGI_FORMAT_R32_FLOAT);
		//myDownsampledDepth = FullscreenTextureFactory.CreateTexture(aWindowHandler->GetResolution() / 2.0f, DXGI_FORMAT_R32_FLOAT);

		//myIntermediateTexture = FullscreenTextureFactory.CreateTexture({ 2048.0f * 1.0f, 2048.0f * 1.0f }, DXGI_FORMAT_R16G16B16A16_FLOAT);
		//myLuminanceTexture = FullscreenTextureFactory.CreateTexture(aWindowHandler->GetResolution(), DXGI_FORMAT_R16G16B16A16_FLOAT);
		//myHalfSizeTexture = FullscreenTextureFactory.CreateTexture(aWindowHandler->GetResolution() / 2.0f, DXGI_FORMAT_R16G16B16A16_FLOAT);
		//myQuarterSizeTexture = FullscreenTextureFactory.CreateTexture(aWindowHandler->GetResolution() / 4.0f, DXGI_FORMAT_R16G16B16A16_FLOAT);
		//myBlurTexture1 = FullscreenTextureFactory.CreateTexture(aWindowHandler->GetResolution(), DXGI_FORMAT_R16G16B16A16_FLOAT);
		//myBlurTexture2 = FullscreenTextureFactory.CreateTexture(aWindowHandler->GetResolution(), DXGI_FORMAT_R16G16B16A16_FLOAT);
		//myVignetteTexture = FullscreenTextureFactory.CreateTexture(aWindowHandler->GetResolution(), DXGI_FORMAT_R16G16B16A16_FLOAT);
		//myVignetteOverlayTexture = FullscreenTextureFactory.CreateTexture(aWindowHandler->GetResolution(), DXGI_FORMAT_R16G16B16A16_FLOAT, ASSETPATH("Assets/IronWrought/UI/Misc/UI_VignetteTexture.dds"));

		//myDeferredLightingTexture = FullscreenTextureFactory.CreateTexture(aWindowHandler->GetResolution(), DXGI_FORMAT_R16G16B16A16_FLOAT);

		//myVolumetricAccumulationBuffer = FullscreenTextureFactory.CreateTexture(aWindowHandler->GetResolution() / 2.0f, DXGI_FORMAT_R16G16B16A16_FLOAT);
		//myVolumetricBlurTexture = FullscreenTextureFactory.CreateTexture(aWindowHandler->GetResolution() / 2.0f, DXGI_FORMAT_R16G16B16A16_FLOAT);

		//mySSAOBuffer = FullscreenTextureFactory.CreateTexture(aWindowHandler->GetResolution() / 2.0f, DXGI_FORMAT_R16G16B16A16_FLOAT);
		//mySSAOBlurTexture = FullscreenTextureFactory.CreateTexture(aWindowHandler->GetResolution() / 2.0f, DXGI_FORMAT_R16G16B16A16_FLOAT);

		//myTonemappedTexture = FullscreenTextureFactory.CreateTexture(aWindowHandler->GetResolution(), DXGI_FORMAT_R16G16B16A16_FLOAT);
		//myAntiAliasedTexture = FullscreenTextureFactory.CreateTexture(aWindowHandler->GetResolution(), DXGI_FORMAT_R16G16B16A16_FLOAT);
		//myGBuffer = FullscreenTextureFactory.CreateGBuffer(aWindowHandler->GetResolution());
		//myGBufferCopy = FullscreenTextureFactory.CreateGBuffer(aWindowHandler->GetResolution());
	}

	void CRenderManager::Render(CScene& /*scene*/)
	{
		CRenderManager::NumberOfDrawCallsThisFrame = 0;

//#ifndef EXCELSIOR_BUILD
//		if (Input::GetInstance()->IsKeyPressed(VK_F6))
//		{
//			ToggleRenderPass();
//			ForwardRenderer.ToggleRenderPass(RenderPassIndex);
//		}
//		if (Input::GetInstance()->IsKeyPressed(VK_F7))
//		{
//			ToggleRenderPass(false);
//			ForwardRenderer.ToggleRenderPass(RenderPassIndex);
//		}
//#endif
//
		RenderStateManager.SetAllDefault();
		Backbuffer.ClearTexture(ClearColor);
//		myIntermediateTexture.ClearTexture(ClearColor);
//		myIntermediateDepth.ClearDepth();
//		myEnvironmentShadowDepth.ClearDepth();
//		myGBuffer.ClearTextures(ClearColor);
//		myDeferredLightingTexture.ClearTexture();
//		myVolumetricAccumulationBuffer.ClearTexture();
//		mySSAOBuffer.ClearTexture();
//
//		CEnvironmentLight* environmentlight = aScene.EnvironmentLight();
//		CCameraComponent* maincamera = aScene.MainCamera();
//
//		if (maincamera == nullptr)
//			return;
//
//		std::vector<CGameObject*> gameObjects /*= aScene.CullGameObjects(maincamera)*/;
//		std::vector<CGameObject*> instancedGameObjects;
//		std::vector<CGameObject*> instancedGameObjectsWithAlpha;
//		std::vector<CGameObject*> gameObjectsWithAlpha;
//		std::vector<int> indicesOfOutlineModels;
//		std::vector<int> indicesOfAlphaGameObjects;
//
//		aScene.CullGameObjects(maincamera, gameObjects, instancedGameObjects);
//
//		//for (unsigned int i = 0; i < gameObjects.size(); ++i)
//		//{
//		//	auto instance = gameObjects[i];
//		//	//for (auto gameObjectToOutline : aScene.ModelsToOutline()) {
//		//	//	if (instance == gameObjectToOutline) {
//		//	//		indicesOfOutlineModels.emplace_back(i);
//		//	//	}
//		//	//}
//
//		//	if (instance->GetComponent<CInstancedModelComponent>()) 
//		//	{
//		//		//if (instance->GetComponent<CInstancedModelComponent>()->RenderWithAlpha())
//		//		//{
//		//		//	instancedGameObjectsWithAlpha.emplace_back(instance);
//		//		//	indicesOfAlphaGameObjects.emplace_back(i);
//		//		//	continue;
//		//		//}
//		//		instancedGameObjects.emplace_back(instance);
//		//		std::swap(gameObjects[i], gameObjects.back());
//		//		gameObjects.pop_back();
//		//	}
//
//		//	// All relevant objects are run in deferred now
//		//	//else if (instance->GetComponent<CModelComponent>()) 
//		//	//{
//		//	//	//if (instance->GetComponent<CModelComponent>()->RenderWithAlpha())
//		//	//	//{
//		//	//	//	gameObjectsWithAlpha.emplace_back(instance);
//		//	//	//	indicesOfAlphaGameObjects.emplace_back(i);
//		//	//	//	continue;
//		//	//	//}
//		//	//}
//		//}
//
//		std::sort(indicesOfAlphaGameObjects.begin(), indicesOfAlphaGameObjects.end(), [](UINT a, UINT b) { return a > b; });
//		for (auto index : indicesOfAlphaGameObjects)
//		{
//			std::swap(gameObjects[index], gameObjects.back());
//			gameObjects.pop_back();
//		}
//
//		// GBuffer
//		myGBuffer.SetAsActiveTarget(&myIntermediateDepth);
//		myDeferredRenderer.GenerateGBuffer(maincamera, gameObjects, instancedGameObjects);
//
//		// Shadows
//		myEnvironmentShadowDepth.SetAsDepthTarget(&myIntermediateTexture);
//
//		// If no shadowmap, don't do this
//		//myShadowRenderer.Render(environmentlight, gameObjects, instancedGameObjects);
//
//		// All relevant objects are run in deferred now
//		//myShadowRenderer.Render(environmentlight, gameObjectsWithAlpha, instancedGameObjectsWithAlpha);
//
//		// Decals
//		myDepthCopy.SetAsActiveTarget();
//		myIntermediateDepth.SetAsResourceOnSlot(0);
//		myFullscreenRenderer.Render(CFullscreenRenderer::FullscreenShader::CopyDepth);
//
//		RenderStateManager.SetDepthStencilState(CRenderStateManager::DepthStencilStates::DEPTHSTENCILSTATE_ONLYREAD);
//		RenderStateManager.SetBlendState(CRenderStateManager::BlendStates::BLENDSTATE_ALPHABLEND);
//		myGBuffer.SetAsActiveTarget(&myIntermediateDepth);
//		myDepthCopy.SetAsResourceOnSlot(21);
//		myDecalRenderer.Render(maincamera, gameObjects);
//
//		// SSAO
//		mySSAOBuffer.SetAsActiveTarget();
//		myGBuffer.SetAsResourceOnSlot(CGBuffer::EGBufferTextures::NORMAL, 2);
//		myIntermediateDepth.SetAsResourceOnSlot(21);
//		myFullscreenRenderer.Render(CFullscreenRenderer::FullscreenShader::SSAO);
//		RenderStateManager.SetBlendState(CRenderStateManager::BlendStates::BLENDSTATE_DISABLE);
//
//		mySSAOBlurTexture.SetAsActiveTarget();
//		mySSAOBuffer.SetAsResourceOnSlot(0);
//		myFullscreenRenderer.Render(CFullscreenRenderer::FullscreenShader::SSAOBlur);
//
//		// Lighting
//		myDeferredLightingTexture.SetAsActiveTarget();
//		myGBuffer.SetAllAsResources();
//		myDepthCopy.SetAsResourceOnSlot(21);
//		RenderStateManager.SetBlendState(CRenderStateManager::BlendStates::BLENDSTATE_ADDITIVEBLEND);
//		std::vector<CPointLight*> onlyPointLights;
//		onlyPointLights = aScene.CullPointLights(gameObjects);
//		std::vector<CSpotLight*> onlySpotLights;
//		onlySpotLights = aScene.CullSpotLights(gameObjects);
//		std::vector<CBoxLight*> onlyBoxLights;
//		//onlyBoxLights = aScene.CullBoxLights(&maincamera->GameObject());
//		std::vector<CEnvironmentLight*> onlySecondaryEnvironmentLights;
//		onlySecondaryEnvironmentLights = aScene.CullSecondaryEnvironmentLights(&maincamera->GameObject());
//
//		if (RenderPassIndex == 0)
//		{
//			myEnvironmentShadowDepth.SetAsResourceOnSlot(22);
//			mySSAOBlurTexture.SetAsResourceOnSlot(23);
//			myLightRenderer.Render(maincamera, environmentlight);
//
//			RenderStateManager.SetRasterizerState(CRenderStateManager::RasterizerStates::RASTERIZERSTATE_FRONTFACECULLING);
//
//			myLightRenderer.Render(maincamera, onlySpotLights);
//			myLightRenderer.Render(maincamera, onlyPointLights);
//			//myLightRenderer.Render(maincamera, onlyBoxLights);
//		}
//
//#pragma region Deferred Render Passes
//		switch (RenderPassIndex)
//		{
//		case 1:
//			myFullscreenRenderer.Render(CFullscreenRenderer::FullscreenShader::DeferredAlbedo);
//			break;
//		case 2:
//			myFullscreenRenderer.Render(CFullscreenRenderer::FullscreenShader::DeferredNormals);
//			break;
//		case 3:
//			myFullscreenRenderer.Render(CFullscreenRenderer::FullscreenShader::DeferredRoughness);
//			break;
//		case 4:
//			myFullscreenRenderer.Render(CFullscreenRenderer::FullscreenShader::DeferredMetalness);
//			break;
//		case 5:
//			myFullscreenRenderer.Render(CFullscreenRenderer::FullscreenShader::DeferredAmbientOcclusion);
//			break;
//		case 6:
//			myFullscreenRenderer.Render(CFullscreenRenderer::FullscreenShader::DeferredEmissive);
//			break;
//		default:
//			break;
//		}
//#pragma endregion
//
//		// Skybox
//		RenderStateManager.SetBlendState(CRenderStateManager::BlendStates::BLENDSTATE_DISABLE);
//		myDeferredLightingTexture.SetAsActiveTarget(&myIntermediateDepth);
//
//		RenderStateManager.SetDepthStencilState(CRenderStateManager::DepthStencilStates::DEPTHSTENCILSTATE_DEPTHFIRST);
//		RenderStateManager.SetRasterizerState(CRenderStateManager::RasterizerStates::RASTERIZERSTATE_FRONTFACECULLING);
//		myDeferredRenderer.RenderSkybox(maincamera, environmentlight);
//		RenderStateManager.SetDepthStencilState(CRenderStateManager::DepthStencilStates::DEPTHSTENCILSTATE_DEFAULT);
//		RenderStateManager.SetRasterizerState(CRenderStateManager::RasterizerStates::RASTERIZERSTATE_DEFAULT);
//
//		// Render Lines
//		const std::vector<CLineInstance*>& lineInstances = aScene.CullLineInstances();
//		const std::vector<SLineTime>& lines = aScene.CullLines();
//		ForwardRenderer.RenderLines(maincamera, lines);
//		ForwardRenderer.RenderLineInstances(maincamera, lineInstances);
//
//		// All relevant objects are moved to deferred now
//
//		//// Alpha stage for objects in World 3D space
//		////RenderStateManager.SetBlendState(CRenderStateManager::BlendStates::BLENDSTATE_ALPHABLEND);
//		//RenderStateManager.SetBlendState(CRenderStateManager::BlendStates::BLENDSTATE_DISABLE); // Alpha clipped
//		//RenderStateManager.SetDepthStencilState(CRenderStateManager::DepthStencilStates::DEPTHSTENCILSTATE_DEFAULT);
//		////RenderStateManager.SetDepthStencilState(CRenderStateManager::DepthStencilStates::DEPTHSTENCILSTATE_ONLYREAD);
//
//		//std::vector<LightPair> pointlights;
//		//std::vector<LightPair> pointLightsInstanced;
//
//		//for (unsigned int i = 0; i < instancedGameObjectsWithAlpha.size(); ++i)
//		//{
//		//	pointLightsInstanced.emplace_back(aScene.CullLightInstanced(instancedGameObjectsWithAlpha[i]->GetComponent<CInstancedModelComponent>()));
//		//}
//		//for (unsigned int i = 0; i < gameObjectsWithAlpha.size(); ++i)
//		//{
//		//	pointlights.emplace_back(aScene.CullLights(gameObjectsWithAlpha[i]));
//		//}
//
//		//myEnvironmentShadowDepth.SetAsResourceOnSlot(22);
//		//ForwardRenderer.InstancedRender(environmentlight, pointLightsInstanced, maincamera, instancedGameObjectsWithAlpha);
//		//ForwardRenderer.Render(environmentlight, pointlights, maincamera, gameObjectsWithAlpha);
//
//	//#pragma region Volumetric Lighting
//	//	if (RenderPassIndex == 0 || RenderPassIndex == 7)
//	//	{
//	//		// Depth Copy
//	//		myDepthCopy.SetAsActiveTarget();
//	//		myIntermediateDepth.SetAsResourceOnSlot(0);
//	//		myFullscreenRenderer.Render(CFullscreenRenderer::FullscreenShader::CopyDepth);
//	//
//	//		// Volumetric Lighting
//	//		myVolumetricAccumulationBuffer.SetAsActiveTarget();
//	//		myDepthCopy.SetAsResourceOnSlot(21);
//	//		RenderStateManager.SetBlendState(CRenderStateManager::BlendStates::BLENDSTATE_ADDITIVEBLEND);
//	//		RenderStateManager.SetRasterizerState(CRenderStateManager::RasterizerStates::RASTERIZERSTATE_NOFACECULLING);
//	//
//	//		myLightRenderer.RenderVolumetric(maincamera, onlyPointLights);
//	//		myLightRenderer.RenderVolumetric(maincamera, onlySpotLights);
//	//		myBoxLightShadowDepth.SetAsResourceOnSlot(22);
//	//		myLightRenderer.RenderVolumetric(maincamera, onlyBoxLights);
//	//		RenderStateManager.SetRasterizerState(CRenderStateManager::RasterizerStates::RASTERIZERSTATE_DEFAULT);
//	//		myEnvironmentShadowDepth.SetAsResourceOnSlot(22);
//	//		myLightRenderer.RenderVolumetric(maincamera, environmentlight);
//	//		myLightRenderer.RenderVolumetric(maincamera, onlySecondaryEnvironmentLights);
//	//
//	//		// Downsampling and Blur
//	//		RenderStateManager.SetBlendState(CRenderStateManager::BlendStates::BLENDSTATE_DISABLE);
//	//		myDownsampledDepth.SetAsActiveTarget();
//	//		myIntermediateDepth.SetAsResourceOnSlot(0);
//	//		myFullscreenRenderer.Render(CFullscreenRenderer::FullscreenShader::DownsampleDepth);
//	//
//	//		// Blur
//	//		myVolumetricBlurTexture.SetAsActiveTarget();
//	//		myVolumetricAccumulationBuffer.SetAsResourceOnSlot(0);
//	//		myFullscreenRenderer.Render(CFullscreenRenderer::FullscreenShader::BilateralHorizontal);
//	//
//	//		myVolumetricAccumulationBuffer.SetAsActiveTarget();
//	//		myVolumetricBlurTexture.SetAsResourceOnSlot(0);
//	//		myFullscreenRenderer.Render(CFullscreenRenderer::FullscreenShader::BilateralVertical);
//	//
//	//		myVolumetricBlurTexture.SetAsActiveTarget();
//	//		myVolumetricAccumulationBuffer.SetAsResourceOnSlot(0);
//	//		myFullscreenRenderer.Render(CFullscreenRenderer::FullscreenShader::BilateralHorizontal);
//	//
//	//		myVolumetricAccumulationBuffer.SetAsActiveTarget();
//	//		myVolumetricBlurTexture.SetAsResourceOnSlot(0);
//	//		myFullscreenRenderer.Render(CFullscreenRenderer::FullscreenShader::BilateralVertical);
//	//
//	//		myVolumetricBlurTexture.SetAsActiveTarget();
//	//		myVolumetricAccumulationBuffer.SetAsResourceOnSlot(0);
//	//		myFullscreenRenderer.Render(CFullscreenRenderer::FullscreenShader::BilateralHorizontal);
//	//
//	//		myVolumetricAccumulationBuffer.SetAsActiveTarget();
//	//		myVolumetricBlurTexture.SetAsResourceOnSlot(0);
//	//		myFullscreenRenderer.Render(CFullscreenRenderer::FullscreenShader::BilateralVertical);
//	//
//	//		myVolumetricBlurTexture.SetAsActiveTarget();
//	//		myVolumetricAccumulationBuffer.SetAsResourceOnSlot(0);
//	//		myFullscreenRenderer.Render(CFullscreenRenderer::FullscreenShader::BilateralHorizontal);
//	//
//	//		myVolumetricAccumulationBuffer.SetAsActiveTarget();
//	//		myVolumetricBlurTexture.SetAsResourceOnSlot(0);
//	//		myFullscreenRenderer.Render(CFullscreenRenderer::FullscreenShader::BilateralVertical);
//	//
//	//		// Upsampling
//	//		RenderStateManager.SetBlendState(CRenderStateManager::BlendStates::BLENDSTATE_ADDITIVEBLEND);
//	//		myDeferredLightingTexture.SetAsActiveTarget();
//	//		myVolumetricAccumulationBuffer.SetAsResourceOnSlot(0);
//	//		myDownsampledDepth.SetAsResourceOnSlot(1);
//	//		myIntermediateDepth.SetAsResourceOnSlot(2);
//	//		myFullscreenRenderer.Render(CFullscreenRenderer::FullscreenShader::DepthAwareUpsampling);
//	//	}
//	//#pragma endregion
//
//		//VFX
//		myDeferredLightingTexture.SetAsActiveTarget(&myIntermediateDepth);
//		RenderStateManager.SetBlendState(CRenderStateManager::BlendStates::BLENDSTATE_ADDITIVEBLEND);
//		RenderStateManager.SetDepthStencilState(CRenderStateManager::DepthStencilStates::DEPTHSTENCILSTATE_ONLYREAD);
//		RenderStateManager.SetRasterizerState(CRenderStateManager::RasterizerStates::RASTERIZERSTATE_NOFACECULLING);
//		myVFXRenderer.Render(maincamera, gameObjects);
//		RenderStateManager.SetRasterizerState(CRenderStateManager::RasterizerStates::RASTERIZERSTATE_DEFAULT);
//
//		myParticleRenderer.Render(maincamera, gameObjects);
//
//		RenderStateManager.SetBlendState(CRenderStateManager::BlendStates::BLENDSTATE_DISABLE);
//		//RenderStateManager.SetDepthStencilState(CRenderStateManager::DepthStencilStates::DEPTHSTENCILSTATE_DEFAULT);
//
//		// Bloom
//		RenderBloom();
//
//		// Tonemapping
//		myTonemappedTexture.SetAsActiveTarget();
//		myDeferredLightingTexture.SetAsResourceOnSlot(0);
//		myFullscreenRenderer.Render(CFullscreenRenderer::FullscreenShader::Tonemap);
//
//		// Anti-aliasing
//		myAntiAliasedTexture.SetAsActiveTarget();
//		myTonemappedTexture.SetAsResourceOnSlot(0);
//		myFullscreenRenderer.Render(CFullscreenRenderer::FullscreenShader::FXAA);
//
//		// Broken Screen
//		if (UseBrokenScreenPass)
//		{
//			myAntiAliasedTexture.SetAsActiveTarget();
//			myTonemappedTexture.SetAsResourceOnSlot(0);
//			myFullscreenRenderer.Render(CFullscreenRenderer::FullscreenShader::BrokenScreenEffect);
//			Backbuffer.SetAsActiveTarget();
//			myAntiAliasedTexture.SetAsResourceOnSlot(0);
//		}
//
//		// Gamma correction
//		myVignetteTexture.SetAsActiveTarget(); // For vignetting
//		myAntiAliasedTexture.SetAsResourceOnSlot(0);
//
//		if (RenderPassIndex == 7)
//		{
//			myVolumetricAccumulationBuffer.SetAsResourceOnSlot(0);
//		}
//
//		if (RenderPassIndex == 8)
//		{
//			mySSAOBlurTexture.SetAsResourceOnSlot(0);
//			//mySSAOBuffer.SetAsResourceOnSlot(0);
//		}
//
//		if (RenderPassIndex < 2)
//		{
//			myFullscreenRenderer.Render(CFullscreenRenderer::FullscreenShader::GammaCorrection);
//		}
//		else
//		{
//			myFullscreenRenderer.Render(CFullscreenRenderer::FullscreenShader::GammaCorrectionRenderPass);
//		}
//
//		// Vignette
//		Backbuffer.SetAsActiveTarget();
//		myVignetteTexture.SetAsResourceOnSlot(0);
//		myVignetteOverlayTexture.SetAsResourceOnSlot(1);
//		myFullscreenRenderer.Render(CFullscreenRenderer::FullscreenShader::Vignette);
//
//		//Backbuffer.SetAsActiveTarget();
//
//		RenderStateManager.SetBlendState(CRenderStateManager::BlendStates::BLENDSTATE_ALPHABLEND);
//		RenderStateManager.SetDepthStencilState(CRenderStateManager::DepthStencilStates::DEPTHSTENCILSTATE_ONLYREAD);
//
//		std::vector<CSpriteInstance*> sprites;
//		std::vector<CSpriteInstance*> animatedUIFrames;
//		std::vector<CTextInstance*> textsToRender;
//		std::vector<CAnimatedUIElement*> animatedUIElements;
//
//		const CCanvas* canvas = aScene.Canvas();
//		if (canvas)
//		{
//			canvas->EmplaceSprites(sprites);
//			animatedUIElements = canvas->EmplaceAnimatedUI(animatedUIFrames);
//			canvas->EmplaceTexts(textsToRender);
//		}
//
//		// Sprites
//		CMainSingleton::PopupTextService().EmplaceSprites(sprites);
//		CMainSingleton::DialogueSystem().EmplaceSprites(sprites);
//		CEngine::GetInstance()->GetActiveScene().MainCamera()->EmplaceSprites(animatedUIFrames);
//		mySpriteRenderer.Render(sprites);
//		mySpriteRenderer.Render(animatedUIElements);
//		mySpriteRenderer.Render(animatedUIFrames);
//
//		// Text
//		CMainSingleton::PopupTextService().EmplaceTexts(textsToRender);
//		CMainSingleton::DialogueSystem().EmplaceTexts(textsToRender);
//		RenderStateManager.SetBlendState(CRenderStateManager::BlendStates::BLENDSTATE_DISABLE);
//		RenderStateManager.SetDepthStencilState(CRenderStateManager::DepthStencilStates::DEPTHSTENCILSTATE_DEFAULT);
//		myTextRenderer.Render(textsToRender);
	}

	void CRenderManager::Render()
	{
		CRenderManager::NumberOfDrawCallsThisFrame = 0;
		RenderStateManager.SetAllDefault();
		RenderedScene.ClearTexture(ClearColor);
		//RenderedScene.SetAsActiveTarget();
		Backbuffer.ClearTexture(ClearColor);
		Backbuffer.SetAsActiveTarget();
	}

	void CRenderManager::Release()
	{
		Clear(ClearColor);
		CEngine::GetInstance()->Framework->GetContext()->OMSetRenderTargets(0, 0, 0);
		CEngine::GetInstance()->Framework->GetContext()->OMGetDepthStencilState(0, 0);
		CEngine::GetInstance()->Framework->GetContext()->ClearState();

		//Backbuffer.ReleaseTexture();
		//myIntermediateTexture.ReleaseTexture();
		//myIntermediateDepth.ReleaseDepth();
		//myLuminanceTexture.ReleaseTexture();
		//myHalfSizeTexture.ReleaseTexture();
		//myQuarterSizeTexture.ReleaseTexture();
		//myBlurTexture1.ReleaseTexture();
		//myBlurTexture2.ReleaseTexture();
		//myVignetteTexture.ReleaseTexture();
		//myVignetteOverlayTexture.ReleaseTexture();
		//myDeferredLightingTexture.ReleaseTexture();

		//myEnvironmentShadowDepth.ReleaseDepth();
		//myBoxLightShadowDepth.ReleaseDepth();
		//myDepthCopy.ReleaseDepth();
		//myDownsampledDepth.ReleaseDepth();

		//myVolumetricAccumulationBuffer.ReleaseTexture();
		//myVolumetricBlurTexture.ReleaseTexture();
		//myTonemappedTexture.ReleaseTexture();
		//myAntiAliasedTexture.ReleaseTexture();

		//myGBuffer.ReleaseResources();
		//myGBufferCopy.ReleaseResources();
	}

	const CFullscreenTexture& CRenderManager::GetRenderedSceneTexture() const
	{
		return Backbuffer;
	}

	//void CRenderManager::SetBrokenScreen(bool aShouldSetBrokenScreen)
	//{
	//	UseBrokenScreenPass = aShouldSetBrokenScreen;
	//}

	//const CFullscreenRenderer::SPostProcessingBufferData& CRenderManager::GetPostProcessingBufferData() const
	//{
	//	return myFullscreenRenderer.myPostProcessingBufferData;
	//}

	//void CRenderManager::SetPostProcessingBufferData(const CFullscreenRenderer::SPostProcessingBufferData& someBufferData)
	//{
	//	myFullscreenRenderer.myPostProcessingBufferData = someBufferData;
	//}

	void CRenderManager::Clear(SVector4 /*clearColor*/)
	{
		//Backbuffer.ClearTexture(clearColor);
		//myIntermediateDepth.ClearDepth();
	}

	void CRenderManager::RenderBloom()
	{
		//myHalfSizeTexture.SetAsActiveTarget();
		//myDeferredLightingTexture.SetAsResourceOnSlot(0);
		//myFullscreenRenderer.Render(CFullscreenRenderer::FullscreenShader::Copy);

		//myQuarterSizeTexture.SetAsActiveTarget();
		//myHalfSizeTexture.SetAsResourceOnSlot(0);
		//myFullscreenRenderer.Render(CFullscreenRenderer::FullscreenShader::Copy);

		//myBlurTexture1.SetAsActiveTarget();
		//myQuarterSizeTexture.SetAsResourceOnSlot(0);
		//myFullscreenRenderer.Render(CFullscreenRenderer::FullscreenShader::Copy);

		//myBlurTexture2.SetAsActiveTarget();
		//myBlurTexture1.SetAsResourceOnSlot(0);
		//myFullscreenRenderer.Render(CFullscreenRenderer::FullscreenShader::GaussianHorizontal);

		//myBlurTexture1.SetAsActiveTarget();
		//myBlurTexture2.SetAsResourceOnSlot(0);
		//myFullscreenRenderer.Render(CFullscreenRenderer::FullscreenShader::GaussianVertical);

		//myBlurTexture2.SetAsActiveTarget();
		//myBlurTexture1.SetAsResourceOnSlot(0);
		//myFullscreenRenderer.Render(CFullscreenRenderer::FullscreenShader::GaussianHorizontal);

		//myBlurTexture1.SetAsActiveTarget();
		//myBlurTexture2.SetAsResourceOnSlot(0);
		//myFullscreenRenderer.Render(CFullscreenRenderer::FullscreenShader::GaussianVertical);

		//myQuarterSizeTexture.SetAsActiveTarget();
		//myBlurTexture1.SetAsResourceOnSlot(0);
		//myFullscreenRenderer.Render(CFullscreenRenderer::FullscreenShader::Copy);

		//myHalfSizeTexture.SetAsActiveTarget();
		//myQuarterSizeTexture.SetAsResourceOnSlot(0);
		//myFullscreenRenderer.Render(CFullscreenRenderer::FullscreenShader::Copy);

		//myVignetteTexture.SetAsActiveTarget();
		//myDeferredLightingTexture.SetAsResourceOnSlot(0);
		//myFullscreenRenderer.Render(CFullscreenRenderer::FullscreenShader::Copy); // No vignette at this step

		//myDeferredLightingTexture.SetAsActiveTarget();
		//myVignetteTexture.SetAsResourceOnSlot(0);
		//myHalfSizeTexture.SetAsResourceOnSlot(1);
		//myFullscreenRenderer.Render(CFullscreenRenderer::FullscreenShader::Bloom);
	}

	void CRenderManager::RenderWithoutBloom()
	{
		//Backbuffer.SetAsActiveTarget();
		//myIntermediateTexture.SetAsResourceOnSlot(0);
		//myFullscreenRenderer.Render(CFullscreenRenderer::FullscreenShader::Vignette);
	}

	void CRenderManager::ToggleRenderPass(bool shouldToggleForwards)
	{
		if (!shouldToggleForwards)
		{
			--RenderPassIndex;
			if (RenderPassIndex < 0) {
				RenderPassIndex = 8;
			}
			return;
		}

		++RenderPassIndex;
		if (RenderPassIndex > 8)
		{
			RenderPassIndex = 0;
		}
	}
}
