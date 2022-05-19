// Copyright 2022 Team Havtorn. All Rights Reserved.

#include "hvpch.h"
#include "RenderManager.h"
#include "GraphicsUtilities.h"
#include "RenderCommand.h"
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
#include "ECS/ECSInclude.h"
#include "GraphicsStructs.h"
#include "FileSystem/FileHeaderDeclarations.h"

#include <algorithm>
#include <future>

#include "FileSystem/FileSystem.h"
#include "Threading/ThreadManager.h"
#include "MaterialHandler.h"

#include "ModelImporter.h"

#include <DirectXTex/DirectXTex.h>

namespace Havtorn
{
	unsigned int CRenderManager::NumberOfDrawCallsThisFrame = 0;

	CRenderManager::CRenderManager()
		: Framework(nullptr)
	    , Context(nullptr)
	    , FrameBuffer(nullptr)
	    , ObjectBuffer(nullptr)
	    , LightBuffer(nullptr)
	    , PushToCommands(&RenderCommandsA)
	    , PopFromCommands(&RenderCommandsB)
	    , ClearColor(0.5f, 0.5f, 0.5f, 1.0f)
	    , RenderPassIndex(0)
	    , DoFullRender(true)
	    , UseAntiAliasing(true)
	    , UseBrokenScreenPass(false)
	{
	}

	CRenderManager::~CRenderManager()
	{
		Release();
	}

	bool CRenderManager::Init(CGraphicsFramework* framework, CWindowHandler* windowHandler)
	{
		Framework = framework;
		Context = Framework->GetContext();

		D3D11_BUFFER_DESC bufferDescription = { 0 };
		bufferDescription.Usage = D3D11_USAGE_DYNAMIC;
		bufferDescription.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		bufferDescription.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

		bufferDescription.ByteWidth = sizeof(SFrameBufferData);
		ENGINE_HR_BOOL_MESSAGE(Framework->GetDevice()->CreateBuffer(&bufferDescription, nullptr, &FrameBuffer), "Frame Buffer could not be created.");

		bufferDescription.ByteWidth = sizeof(SObjectBufferData);
		ENGINE_HR_BOOL_MESSAGE(Framework->GetDevice()->CreateBuffer(&bufferDescription, nullptr, &ObjectBuffer), "Object Buffer could not be created.");

		bufferDescription.ByteWidth = sizeof(SDirectionalLightBufferData);
		ENGINE_HR_BOOL_MESSAGE(Framework->GetDevice()->CreateBuffer(&bufferDescription, nullptr, &LightBuffer), "Light Buffer could not be created.");

		//ENGINE_ERROR_BOOL_MESSAGE(ForwardRenderer.Init(aFramework), "Failed to Init Forward Renderer.");
		//ENGINE_ERROR_BOOL_MESSAGE(myLightRenderer.Init(aFramework), "Failed to Init Light Renderer.");
		//ENGINE_ERROR_BOOL_MESSAGE(myDeferredRenderer.Init(aFramework, &CMainSingleton::MaterialHandler()), "Failed to Init Deferred Renderer.");
		ENGINE_ERROR_BOOL_MESSAGE(FullscreenRenderer.Init(framework), "Failed to Init Fullscreen Renderer.");
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

		//RenderedScene = FullscreenTextureFactory.CreateTexture(backbufferTexture);
		Backbuffer = FullscreenTextureFactory.CreateTexture(backbufferTexture);
		InitRenderTextures(windowHandler);

		// Load default resources
		//const std::string vsData = AddShader("Shaders/Demo_VS.cso", EShaderType::Vertex);
		const std::string vsData = AddShader("Shaders/DeferredModel_VS.cso", EShaderType::Vertex);
		AddInputLayout(vsData, EInputLayoutType::Pos3Nor3Tan3Bit3UV2);
		AddShader("Shaders/DeferredVertexShader_VS.cso", EShaderType::Vertex);

		//AddShader("Shaders/Demo_PS.cso", EShaderType::Pixel);
		AddShader("Shaders/GBuffer_PS.cso", EShaderType::Pixel);
		AddShader("Shaders/DeferredLightEnvironment_PS.cso", EShaderType::Pixel);

		AddSampler(ESamplerType::Wrap);
		AddSampler(ESamplerType::Border);
		AddTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		LoadDemoSceneResources();

		return true;
	}

	bool CRenderManager::ReInit(CGraphicsFramework* framework, CWindowHandler* windowHandler)
	{
		ENGINE_ERROR_BOOL_MESSAGE(RenderStateManager.Init(framework), "Failed to Init Render State Manager.");

		ID3D11Texture2D* backbufferTexture = framework->GetBackbufferTexture();
		ENGINE_ERROR_BOOL_MESSAGE(backbufferTexture, "Backbuffer Texture is null.");

		Backbuffer = FullscreenTextureFactory.CreateTexture(backbufferTexture);
		InitRenderTextures(windowHandler);

		return true;
	}

	void CRenderManager::InitRenderTextures(CWindowHandler* windowHandler)
	{
		RenderedScene = FullscreenTextureFactory.CreateTexture(windowHandler->GetResolution(), DXGI_FORMAT_R16G16B16A16_FLOAT);
		IntermediateDepth = FullscreenTextureFactory.CreateDepth(windowHandler->GetResolution(), DXGI_FORMAT_R24G8_TYPELESS);

		DefaultCubemap = CEngine::GetInstance()->GetMaterialHandler()->RequestCubemap("ForestCubemap");

		const SVector2<F32> shadowAtlasResolution = {8192.0f, 8192.0f};
		InitShadowmapAtlas(shadowAtlasResolution);
		//myBoxLightShadowDepth = FullscreenTextureFactory.CreateDepth(aWindowHandler->GetResolution(), DXGI_FORMAT_R32_TYPELESS);
		DepthCopy = FullscreenTextureFactory.CreateTexture(windowHandler->GetResolution(), DXGI_FORMAT_R32_FLOAT);
		//myDownsampledDepth = FullscreenTextureFactory.CreateTexture(aWindowHandler->GetResolution() / 2.0f, DXGI_FORMAT_R32_FLOAT);

		IntermediateTexture = FullscreenTextureFactory.CreateTexture(shadowAtlasResolution, DXGI_FORMAT_R16G16B16A16_FLOAT);
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

		SSAOBuffer = FullscreenTextureFactory.CreateTexture(windowHandler->GetResolution() / 2.0f, DXGI_FORMAT_R16G16B16A16_FLOAT);
		SSAOBlurTexture = FullscreenTextureFactory.CreateTexture(windowHandler->GetResolution() / 2.0f, DXGI_FORMAT_R16G16B16A16_FLOAT);

		TonemappedTexture = FullscreenTextureFactory.CreateTexture(windowHandler->GetResolution(), DXGI_FORMAT_R16G16B16A16_FLOAT);
		AntiAliasedTexture = FullscreenTextureFactory.CreateTexture(windowHandler->GetResolution(), DXGI_FORMAT_R16G16B16A16_FLOAT);
		GBuffer = FullscreenTextureFactory.CreateGBuffer(windowHandler->GetResolution());
		//myGBufferCopy = FullscreenTextureFactory.CreateGBuffer(aWindowHandler->GetResolution());
	}

	void CRenderManager::InitShadowmapAtlas(SVector2<F32> atlasResolution)
	{
		ShadowAtlasDepth = FullscreenTextureFactory.CreateDepth(atlasResolution, DXGI_FORMAT_R32_TYPELESS);

		// LOD 1
		U16 mapsInLod = 8;
		SVector2<F32> topLeftCoordinate = SVector2<F32>::Zero;
		SVector2<F32> widthAndHeight = atlasResolution * (1.0f / (mapsInLod / 2));
		const SVector2<F32> depth = { 0.0f, 1.0f };
		InitShadowmapLOD(topLeftCoordinate, widthAndHeight, depth, mapsInLod, 0);

		// LOD 2
		mapsInLod = 16;
		topLeftCoordinate = { 0.0f, atlasResolution.Y * 0.5f };
		widthAndHeight = atlasResolution * (1.0f / (mapsInLod / 2));
		InitShadowmapLOD(topLeftCoordinate, widthAndHeight, depth, mapsInLod, 8);

		// LOD 3
		mapsInLod = 32;
		topLeftCoordinate = {0.0f, atlasResolution.Y * 0.75f };
		widthAndHeight = atlasResolution * (1.0f / (mapsInLod / 2));
		InitShadowmapLOD(topLeftCoordinate, widthAndHeight, depth, mapsInLod, 24);

		// LOD 4
		mapsInLod = 128;
		topLeftCoordinate = { 0.0f, atlasResolution.Y * 0.875f };
		widthAndHeight = atlasResolution * (1.0f / (mapsInLod / 4));
		InitShadowmapLOD(topLeftCoordinate, widthAndHeight, depth, mapsInLod, 56);
	}

	void CRenderManager::InitShadowmapLOD(SVector2<F32> topLeftCoordinate, const SVector2<F32>& widthAndHeight, const SVector2<F32>& depth, U16 mapsInLod, U16 startIndex)
	{
		const float startingYCoordinate = topLeftCoordinate.Y;
		const U16 mapsPerRow = static_cast<U16>(8192.0f / widthAndHeight.X);
		for (U16 i = startIndex; i < startIndex + mapsInLod; i++)
		{
			const U16 relativeIndex = i - startIndex;
			topLeftCoordinate.X = static_cast<U16>(relativeIndex % mapsPerRow) * widthAndHeight.X;
			topLeftCoordinate.Y = startingYCoordinate + static_cast<U16>(relativeIndex / mapsPerRow) * widthAndHeight.Y;
			AddViewport(topLeftCoordinate, widthAndHeight, depth);
		}
	}

	void CRenderManager::LoadDemoSceneResources()
	{
		CModelImporter::ImportFBX("Assets/Tests/En_P_PendulumClock.fbx");
		CModelImporter::ImportFBX("Assets/Tests/En_P_Bed.fbx");
		CModelImporter::ImportFBX("Assets/Tests/Quad.fbx");
	}

	void CRenderManager::Render()
	{
		while (true)
		{
			std::unique_lock<std::mutex> uniqueLock(CThreadManager::RenderMutex);
			CThreadManager::RenderCondition.wait(uniqueLock, [] 
				{ return CThreadManager::RenderThreadStatus == ERenderThreadStatus::ReadyToRender; });

			CRenderManager::NumberOfDrawCallsThisFrame = 0;
			RenderStateManager.SetAllDefault();

			Backbuffer.ClearTexture();
			ShadowAtlasDepth.ClearDepth();
			SSAOBuffer.ClearTexture();

			RenderedScene.ClearTexture();
			IntermediateTexture.ClearTexture();
			IntermediateDepth.ClearDepth();
			GBuffer.ClearTextures(ClearColor);

			ShadowAtlasDepth.SetAsDepthTarget(&IntermediateTexture);

			ID3D11DeviceContext* context = Framework->GetContext();

			const U16 commandsInHeap = static_cast<U16>(PopFromCommands->size());
			for (U16 i = 0; i < commandsInHeap; ++i)
			{
				SRenderCommand currentCommand = PopFromCommands->top();
				switch (currentCommand.Type)
				{
				case ERenderCommandType::ShadowAtlasPrePass:
				{
					const auto transformComp = currentCommand.GetComponent(TransformComponent);
					const auto staticMeshComp = currentCommand.GetComponent(StaticMeshComponent);
					const auto directionalLightComp = currentCommand.GetComponent(DirectionalLightComponent);

					FrameBufferData.ToCameraFromWorld = directionalLightComp->ShadowViewMatrix;
					FrameBufferData.ToWorldFromCamera = directionalLightComp->ShadowViewMatrix.FastInverse();
					FrameBufferData.ToProjectionFromCamera = directionalLightComp->ShadowProjectionMatrix;
					FrameBufferData.ToCameraFromProjection = directionalLightComp->ShadowProjectionMatrix.Inverse();
					FrameBufferData.CameraPosition = directionalLightComp->ShadowPosition;
					BindBuffer(FrameBuffer, FrameBufferData, "Frame Buffer");

					ObjectBufferData.ToWorldFromObject = transformComp->Transform.GetMatrix();
					BindBuffer(ObjectBuffer, ObjectBufferData, "Object Buffer");

					context->VSSetConstantBuffers(0, 1, &FrameBuffer);
					context->VSSetConstantBuffers(1, 1, &ObjectBuffer);
					context->IASetPrimitiveTopology(Topologies[staticMeshComp->TopologyIndex]);
					context->IASetInputLayout(InputLayouts[staticMeshComp->InputLayoutIndex]);

					context->VSSetShader(VertexShaders[staticMeshComp->VertexShaderIndex], nullptr, 0);
					context->PSSetShader(nullptr, nullptr, 0);
					
					context->RSSetViewports(1, &Viewports[directionalLightComp->ShadowmapViewportIndex]);

					for (U8 drawCallIndex = 0; drawCallIndex < static_cast<U8>(staticMeshComp->DrawCallData.size()); drawCallIndex++)
					{
						const SDrawCallData& drawData = staticMeshComp->DrawCallData[drawCallIndex];
						ID3D11Buffer* vertexBuffer = VertexBuffers[drawData.VertexBufferIndex];
						context->IASetVertexBuffers(0, 1, &vertexBuffer, &MeshVertexStrides[drawData.VertexStrideIndex], &MeshVertexOffsets[drawData.VertexStrideIndex]);
						context->IASetIndexBuffer(IndexBuffers[drawData.IndexBufferIndex], DXGI_FORMAT_R32_UINT, 0);
						context->DrawIndexed(drawData.IndexCount, 0, 0);
						CRenderManager::NumberOfDrawCallsThisFrame++;
					}
				}
				break;

				case ERenderCommandType::CameraDataStorage:
				{
					GBuffer.SetAsActiveTarget(&IntermediateDepth);

					const auto transformComp = currentCommand.GetComponent(TransformComponent);
					const auto cameraComp = currentCommand.GetComponent(CameraComponent);

					FrameBufferData.ToCameraFromWorld = transformComp->Transform.GetMatrix().FastInverse();
					FrameBufferData.ToWorldFromCamera = transformComp->Transform.GetMatrix();
					FrameBufferData.ToProjectionFromCamera = cameraComp->ProjectionMatrix;
					FrameBufferData.ToCameraFromProjection = cameraComp->ProjectionMatrix.Inverse();
					FrameBufferData.CameraPosition = transformComp->Transform.GetMatrix().Translation4();
					BindBuffer(FrameBuffer, FrameBufferData, "Frame Buffer");

					context->VSSetConstantBuffers(0, 1, &FrameBuffer);
					context->PSSetConstantBuffers(0, 1, &FrameBuffer);
				}
				break;

				case ERenderCommandType::GBufferData:
				{
					const auto transformComp = currentCommand.GetComponent(TransformComponent);
					const auto staticMeshComp = currentCommand.GetComponent(StaticMeshComponent);
					const auto materialComp = currentCommand.GetComponent(MaterialComponent);

					ObjectBufferData.ToWorldFromObject = transformComp->Transform.GetMatrix();
					BindBuffer(ObjectBuffer, ObjectBufferData, "Object Buffer");

					context->VSSetConstantBuffers(1, 1, &ObjectBuffer);
					context->IASetPrimitiveTopology(Topologies[staticMeshComp->TopologyIndex]);
					context->IASetInputLayout(InputLayouts[staticMeshComp->InputLayoutIndex]);

					context->VSSetShader(VertexShaders[staticMeshComp->VertexShaderIndex], nullptr, 0);
					context->PSSetShader(PixelShaders[staticMeshComp->PixelShaderIndex], nullptr, 0);

					ID3D11SamplerState* sampler = Samplers[staticMeshComp->SamplerIndex];
					context->PSSetSamplers(0, 1, &sampler);

					for (U8 drawCallIndex = 0; drawCallIndex < static_cast<U8>(staticMeshComp->DrawCallData.size()); drawCallIndex++)
					{
						// Load Textures
						std::vector<ID3D11ShaderResourceView*> resourceViewPointers;
						resourceViewPointers.resize(TexturesPerMaterial);
						for (U8 textureIndex = 0, pointerTracker = 0; textureIndex < TexturesPerMaterial; textureIndex++, pointerTracker++)
						{
							resourceViewPointers[pointerTracker] = Textures[materialComp->MaterialReferences[textureIndex + drawCallIndex * TexturesPerMaterial]];
						}
						context->PSSetShaderResources(5, TexturesPerMaterial, resourceViewPointers.data());

						const SDrawCallData& drawData = staticMeshComp->DrawCallData[drawCallIndex];
						ID3D11Buffer* vertexBuffer = VertexBuffers[drawData.VertexBufferIndex];
						context->IASetVertexBuffers(0, 1, &vertexBuffer, &MeshVertexStrides[drawData.VertexStrideIndex], &MeshVertexOffsets[drawData.VertexStrideIndex]);
						context->IASetIndexBuffer(IndexBuffers[drawData.IndexBufferIndex], DXGI_FORMAT_R32_UINT, 0);
						context->DrawIndexed(drawData.IndexCount, 0, 0);
						CRenderManager::NumberOfDrawCallsThisFrame++;
					}
				}
				break;

				case ERenderCommandType::DeferredLighting:
				{
					// === SSAO ===
					SSAOBuffer.SetAsActiveTarget();
					GBuffer.SetAsResourceOnSlot(CGBuffer::EGBufferTextures::Normal, 2);
					IntermediateDepth.SetAsResourceOnSlot(21);
					FullscreenRenderer.Render(CFullscreenRenderer::EFullscreenShader::SSAO);
					RenderStateManager.SetBlendState(CRenderStateManager::EBlendStates::Disable);
			
					SSAOBlurTexture.SetAsActiveTarget();
					SSAOBuffer.SetAsResourceOnSlot(0);
					FullscreenRenderer.Render(CFullscreenRenderer::EFullscreenShader::SSAOBlur);
					// === !SSAO ===

					RenderedScene.SetAsActiveTarget();
					GBuffer.SetAllAsResources(1);
					IntermediateDepth.SetAsResourceOnSlot(21);
					RenderStateManager.SetBlendState(CRenderStateManager::EBlendStates::AdditiveBlend);

					// add Alpha blend PS shader

					ShadowAtlasDepth.SetAsResourceOnSlot(22);
					SSAOBlurTexture.SetAsResourceOnSlot(23);
					Context->PSSetShaderResources(0, 1, &DefaultCubemap);

					const auto directionalLightComp = currentCommand.GetComponent(DirectionalLightComponent);
					// Update lightbufferdata and fill lightbuffer
					DirectionalLightBufferData.DirectionalLightDirection = directionalLightComp->Direction;
					DirectionalLightBufferData.DirectionalLightColor = directionalLightComp->Color;
					DirectionalLightBufferData.DirectionalLightPosition = directionalLightComp->ShadowPosition;
					DirectionalLightBufferData.ToDirectionalLightView = directionalLightComp->ShadowViewMatrix;
					DirectionalLightBufferData.ToDirectionalLightProjection = directionalLightComp->ShadowProjectionMatrix;
					DirectionalLightBufferData.DirectionalLightShadowMapResolution = directionalLightComp->ShadowmapResolution;
					BindBuffer(LightBuffer, DirectionalLightBufferData, "Light Buffer");
					Context->PSSetConstantBuffers(2, 1, &LightBuffer);

					// Emissive Post Processing 
					//EmissiveBufferData.EmissiveStrength = GetPostProcessingBufferData().EmissiveStrength;
					//BindBuffer(EmissiveBuffer, EmissiveBufferData, "Emissive Buffer");
					//Context->PSSetConstantBuffers(5, 1, &EmissiveBuffer);

					Context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
					Context->IASetInputLayout(nullptr);
					Context->IASetVertexBuffers(0, 0, nullptr, nullptr, nullptr);
					Context->IASetIndexBuffer(nullptr, DXGI_FORMAT_UNKNOWN, 0);

					Context->GSSetShader(nullptr, nullptr, 0);

					Context->VSSetShader(VertexShaders[1], nullptr, 0);

					Context->PSSetShader(PixelShaders[1], nullptr, 0);

					Context->PSSetSamplers(0, 1, &Samplers[0]);
					Context->PSSetSamplers(1, 1, &Samplers[1]);

					Context->Draw(3, 0);
					CRenderManager::NumberOfDrawCallsThisFrame++;
				}
				break;

				default:
					break;
				}
				PopFromCommands->pop();
			}

			RenderStateManager.SetBlendState(CRenderStateManager::EBlendStates::Disable);
			RenderStateManager.SetDepthStencilState(CRenderStateManager::EDepthStencilStates::Default);
	
			// Bloom
			//RenderBloom();

			// Tonemapping
			TonemappedTexture.SetAsActiveTarget();
			RenderedScene.SetAsResourceOnSlot(0);
			FullscreenRenderer.Render(CFullscreenRenderer::EFullscreenShader::Tonemap);
	
			// Anti-aliasing
			AntiAliasedTexture.SetAsActiveTarget();
			TonemappedTexture.SetAsResourceOnSlot(0);
			FullscreenRenderer.Render(CFullscreenRenderer::EFullscreenShader::FXAA);

			// Gamma correction
			RenderedScene.SetAsActiveTarget();
			AntiAliasedTexture.SetAsResourceOnSlot(0);
			FullscreenRenderer.Render(CFullscreenRenderer::EFullscreenShader::GammaCorrection);

			// Draw debug shadow atlas
			//D3D11_VIEWPORT viewport;
			//viewport.TopLeftX = 0.0f;
			//viewport.TopLeftY = 0.0f;
			//viewport.Width = 256.0f;
			//viewport.Height = 256.0f;
			//viewport.MinDepth = 0.0f;
			//viewport.MaxDepth = 1.0f;
			//context->RSSetViewports(1, &viewport);
			//ShadowAtlasDepth.SetAsResourceOnSlot(0);
			//FullscreenRenderer.Render(CFullscreenRenderer::EFullscreenShader::CopyDepth);

			// RenderedScene should be complete as that is the texture we send to the viewport
			Backbuffer.SetAsActiveTarget();
			RenderedScene.SetAsResourceOnSlot(0);
			FullscreenRenderer.Render(CFullscreenRenderer::EFullscreenShader::Copy);

			CThreadManager::RenderThreadStatus = ERenderThreadStatus::PostRender;
			uniqueLock.unlock();
			CThreadManager::RenderCondition.notify_one();
		}

		//Backbuffer.ClearTexture(ClearColor);

//#ifndef EXCELSIOR_BUILD
//		if (CInput::GetInstance()->IsKeyPressed(VK_F6))
//		{
//			ToggleRenderPass();
//			ForwardRenderer.ToggleRenderPass(RenderPassIndex);
//		}
//		if (CInput::GetInstance()->IsKeyPressed(VK_F7))
//		{
//			ToggleRenderPass(false);
//			ForwardRenderer.ToggleRenderPass(RenderPassIndex);
//		}
//#endif
//
		//Backbuffer.ClearTexture(ClearColor);
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
		//		RenderStateManager.SetDepthStencilState(CRenderStateManager::EDepthStencilStates::OnlyRead);
		//		RenderStateManager.SetBlendState(CRenderStateManager::EBlendStates::AlphaBlend);
		//		myGBuffer.SetAsActiveTarget(&myIntermediateDepth);
		//		myDepthCopy.SetAsResourceOnSlot(21);
		//		myDecalRenderer.Render(maincamera, gameObjects);
		//
		//		// SSAO
		//		mySSAOBuffer.SetAsActiveTarget();
		//		myGBuffer.SetAsResourceOnSlot(CGBuffer::EGBufferTextures::NORMAL, 2);
		//		myIntermediateDepth.SetAsResourceOnSlot(21);
		//		myFullscreenRenderer.Render(CFullscreenRenderer::FullscreenShader::SSAO);
		//		RenderStateManager.SetBlendState(CRenderStateManager::EBlendStates::Disable);
		//
		//		mySSAOBlurTexture.SetAsActiveTarget();
		//		mySSAOBuffer.SetAsResourceOnSlot(0);
		//		myFullscreenRenderer.Render(CFullscreenRenderer::FullscreenShader::SSAOBlur);
		//
		//		// Lighting
		//		myDeferredLightingTexture.SetAsActiveTarget();
		//		myGBuffer.SetAllAsResources();
		//		myDepthCopy.SetAsResourceOnSlot(21);
		//		RenderStateManager.SetBlendState(CRenderStateManager::EBlendStates::AdditiveBlend);
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
		//			RenderStateManager.SetRasterizerState(CRenderStateManager::ERasterizerStates::FrontFaceCulling);
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
		//		RenderStateManager.SetBlendState(CRenderStateManager::EBlendStates::Disable);
		//		myDeferredLightingTexture.SetAsActiveTarget(&myIntermediateDepth);
		//
		//		RenderStateManager.SetDepthStencilState(CRenderStateManager::EDepthStencilStates::DepthFirst);
		//		RenderStateManager.SetRasterizerState(CRenderStateManager::ERasterizerStates::FrontFaceCulling);
		//		myDeferredRenderer.RenderSkybox(maincamera, environmentlight);
		//		RenderStateManager.SetDepthStencilState(CRenderStateManager::EDepthStencilStates::Default);
		//		RenderStateManager.SetRasterizerState(CRenderStateManager::ERasterizerStates::Default);
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
		//		////RenderStateManager.SetBlendState(CRenderStateManager::EBlendStates::AlphaBlend);
		//		//RenderStateManager.SetBlendState(CRenderStateManager::EBlendStates::Disable); // Alpha clipped
		//		//RenderStateManager.SetDepthStencilState(CRenderStateManager::EDepthStencilStates::Default);
		//		////RenderStateManager.SetDepthStencilState(CRenderStateManager::EDepthStencilStates::OnlyRead);
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
		//	//		RenderStateManager.SetBlendState(CRenderStateManager::EBlendStates::AdditiveBlend);
		//	//		RenderStateManager.SetRasterizerState(CRenderStateManager::ERasterizerStates::NoFaceCulling);
		//	//
		//	//		myLightRenderer.RenderVolumetric(maincamera, onlyPointLights);
		//	//		myLightRenderer.RenderVolumetric(maincamera, onlySpotLights);
		//	//		myBoxLightShadowDepth.SetAsResourceOnSlot(22);
		//	//		myLightRenderer.RenderVolumetric(maincamera, onlyBoxLights);
		//	//		RenderStateManager.SetRasterizerState(CRenderStateManager::ERasterizerStates::Default);
		//	//		myEnvironmentShadowDepth.SetAsResourceOnSlot(22);
		//	//		myLightRenderer.RenderVolumetric(maincamera, environmentlight);
		//	//		myLightRenderer.RenderVolumetric(maincamera, onlySecondaryEnvironmentLights);
		//	//
		//	//		// Downsampling and Blur
		//	//		RenderStateManager.SetBlendState(CRenderStateManager::EBlendStates::Disable);
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
		//	//		RenderStateManager.SetBlendState(CRenderStateManager::EBlendStates::AdditiveBlend);
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
		//		RenderStateManager.SetBlendState(CRenderStateManager::EBlendStates::AdditiveBlend);
		//		RenderStateManager.SetDepthStencilState(CRenderStateManager::EDepthStencilStates::OnlyRead);
		//		RenderStateManager.SetRasterizerState(CRenderStateManager::ERasterizerStates::NoFaceCulling);
		//		myVFXRenderer.Render(maincamera, gameObjects);
		//		RenderStateManager.SetRasterizerState(CRenderStateManager::ERasterizerStates::Default);
		//
		//		myParticleRenderer.Render(maincamera, gameObjects);
		//
		//		RenderStateManager.SetBlendState(CRenderStateManager::EBlendStates::Disable);
		//		//RenderStateManager.SetDepthStencilState(CRenderStateManager::EDepthStencilStates::Default);
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
		//		RenderStateManager.SetBlendState(CRenderStateManager::EBlendStates::AlphaBlend);
		//		RenderStateManager.SetDepthStencilState(CRenderStateManager::EDepthStencilStates::OnlyRead);
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
		//		RenderStateManager.SetBlendState(CRenderStateManager::EBlendStates::Disable);
		//		RenderStateManager.SetDepthStencilState(CRenderStateManager::EDepthStencilStates::Default);
		//		myTextRenderer.Render(textsToRender);
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

	void CRenderManager::WriteAssetFile(const std::string& fileName, EAssetType assetType)
	{
		SStaticMeshVertex vertices[24] =
		{
			// X      Y      Z        nX, nY, nZ    tX, tY, tZ,    bX, bY, bZ,    UV	
			{ -0.5f, -0.5f, -0.5f,   -1,  0,  0,    0,  0,  1,     0,  1,  0,     0, 0 }, // 0
			{  0.5f, -0.5f, -0.5f,    1,  0,  0,    0,  0, -1,     0,  1,  0,     1, 0 }, // 1
			{ -0.5f,  0.5f, -0.5f,   -1,  0,  0,    0,  0,  1,     0,  1,  0,     0, 1 }, // 2
			{  0.5f,  0.5f, -0.5f,    1,  0,  0,    0,  0, -1,     0,  1,  0,     1, 1 }, // 3
			{ -0.5f, -0.5f,  0.5f,   -1,  0,  0,    0,  0,  1,     0,  1,  0,     0, 0 }, // 4
			{  0.5f, -0.5f,  0.5f,    1,  0,  0,    0,  0, -1,     0,  1,  0,     1, 0 }, // 5
			{ -0.5f,  0.5f,  0.5f,   -1,  0,  0,    0,  0,  1,     0,  1,  0,     0, 1 }, // 6
			{  0.5f,  0.5f,  0.5f,    1,  0,  0,    0,  0, -1,     0,  1,  0,     1, 1 }, // 7
			// X      Y      Z        nX, nY, nZ    nX, nY, nZ,    nX, nY, nZ,    UV	  
			{ -0.5f, -0.5f, -0.5f,    0, -1,  0,    1,  0,  0,     0,  0,  1,     0, 0 }, // 8  // 0
			{  0.5f, -0.5f, -0.5f,    0, -1,  0,    1,  0,  0,     0,  0,  1,     1, 0 }, // 9	// 1
			{ -0.5f,  0.5f, -0.5f,    0,  1,  0,   -1,  0,  0,     0,  0,  1,     0, 0 }, // 10	// 2
			{  0.5f,  0.5f, -0.5f,    0,  1,  0,   -1,  0,  0,     0,  0,  1,     1, 0 }, // 11	// 3
			{ -0.5f, -0.5f,  0.5f,    0, -1,  0,    1,  0,  0,     0,  0,  1,     0, 1 }, // 12	// 4
			{  0.5f, -0.5f,  0.5f,    0, -1,  0,    1,  0,  0,     0,  0,  1,     0, 1 }, // 13	// 5
			{ -0.5f,  0.5f,  0.5f,    0,  1,  0,   -1,  0,  0,     0,  0,  1,     1, 1 }, // 14	// 6
			{  0.5f,  0.5f,  0.5f,    0,  1,  0,   -1,  0,  0,     0,  0,  1,     1, 1 }, // 15	// 7
			// X      Y      Z        nX, nY, nZ    nX, nY, nZ,    nX, nY, nZ,    UV	  
			{ -0.5f, -0.5f, -0.5f,    0,  0, -1,   -1,  0,  0,     0,  1,  0,     0, 0 }, // 16 // 0
			{  0.5f, -0.5f, -0.5f,    0,  0, -1,   -1,  0,  0,     0,  1,  0,     0, 0 }, // 17	// 1
			{ -0.5f,  0.5f, -0.5f,    0,  0, -1,   -1,  0,  0,     0,  1,  0,     1, 0 }, // 18	// 2
			{  0.5f,  0.5f, -0.5f,    0,  0, -1,   -1,  0,  0,     0,  1,  0,     1, 0 }, // 19	// 3
			{ -0.5f, -0.5f,  0.5f,    0,  0,  1,    1,  0,  0,     0,  1,  0,     0, 1 }, // 20	// 4
			{  0.5f, -0.5f,  0.5f,    0,  0,  1,    1,  0,  0,     0,  1,  0,     1, 1 }, // 21	// 5
			{ -0.5f,  0.5f,  0.5f,    0,  0,  1,    1,  0,  0,     0,  1,  0,     0, 1 }, // 22	// 6
			{  0.5f,  0.5f,  0.5f,    0,  0,  1,    1,  0,  0,     0,  1,  0,     1, 1 }  // 23	// 7
		};
		U32 indices[36] =
		{
			0,4,2,
			4,6,2,
			1,3,5,
			3,7,5,
			8,9,12,
			9,13,12,
			10,14,11,
			14,15,11,
			16,18,17,
			18,19,17,
			20,21,22,
			21,23,22
		};

		SStaticModelFileHeader asset;
		asset.AssetType = EAssetType::StaticModel;
		asset.Name = "PrimitiveCube";
		asset.NameLength = static_cast<U32>(asset.Name.length());
		asset.Meshes.emplace_back();
		asset.Meshes.back().NumberOfVertices = 24;
		asset.Meshes.back().Vertices.assign(vertices, vertices + 24);
		asset.Meshes.back().NumberOfIndices = 36;
		asset.Meshes.back().Indices.assign(indices, indices + 36);

		const auto data = new char[asset.GetSize()];

		switch (assetType)
		{
		case EAssetType::StaticModel:
			{
				asset.Serialize(data);
				CEngine::GetInstance()->GetFileSystem()->Serialize(fileName, &data[0], asset.GetSize());
			}
			break;
		case EAssetType::SkeletalMesh: 
			break;
		case EAssetType::Animation: 
			break;
		case EAssetType::AudioOneShot: 
			break;
		case EAssetType::AudioCollection: 
			break;
		case EAssetType::VisualFX: 
			break;
		}
	}

	void CRenderManager::LoadStaticMeshComponent(const std::string& fileName, SStaticMeshComponent* outStaticMeshComponent)
	{
		SStaticMeshAsset asset;
		if (!LoadedStaticMeshes.contains(fileName))
		{
			// Asset Loading
			const U64 fileSize = CEngine::GetInstance()->GetFileSystem()->GetFileSize(fileName);
			char* data = new char[fileSize];

			CEngine::GetInstance()->GetFileSystem()->Deserialize(fileName, data, static_cast<U32>(fileSize));

			SStaticModelFileHeader assetFile;
			assetFile.Deserialize(data);
			asset = SStaticMeshAsset(assetFile);

			for (U16 i = 0; i < assetFile.NumberOfMeshes; i++)
			{
				asset.DrawCallData[i].VertexBufferIndex = AddVertexBuffer(assetFile.Meshes[i].Vertices);
				asset.DrawCallData[i].IndexBufferIndex = AddIndexBuffer(assetFile.Meshes[i].Indices);
				asset.DrawCallData[i].VertexStrideIndex = AddMeshVertexStride(static_cast<U32>(sizeof(SStaticMeshVertex)));
				asset.DrawCallData[i].VertexOffsetIndex = AddMeshVertexOffset(0);
			}

			LoadedStaticMeshes.emplace(fileName, asset);
		}
		else
		{
			asset = LoadedStaticMeshes.at(fileName);
		}

		// Geometry
		outStaticMeshComponent->DrawCallData = asset.DrawCallData;
	}

	void CRenderManager::LoadMaterialComponent(const std::vector<std::string>& materialNames, SMaterialComponent* outMaterialComponent)
	{
		outMaterialComponent->MaterialReferences.clear();
		for (const std::string& materialName : materialNames)
		{
			std::vector<U16> references = AddMaterial(materialName, MaterialConfiguration);
			
			for (U16 reference : references)
				outMaterialComponent->MaterialReferences.emplace_back(reference);
		}
	}

	const CFullscreenTexture& CRenderManager::GetRenderedSceneTexture() const
	{
		return RenderedScene;
	}

	void CRenderManager::PushRenderCommand(SRenderCommand& command)
	{
		PushToCommands->push(command);
	}

	void CRenderManager::SwapRenderCommandBuffers()
	{
		std::swap(PushToCommands, PopFromCommands);
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

	U16 CRenderManager::AddIndexBuffer(const std::vector<U32>& indices)
	{
		D3D11_BUFFER_DESC indexBufferDesc = { 0 };
		indexBufferDesc.ByteWidth = sizeof(U32) * static_cast<U32>(indices.size());
		indexBufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
		indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;

		D3D11_SUBRESOURCE_DATA indexSubresourceData = { 0 };
		indexSubresourceData.pSysMem = indices.data();

		ID3D11Buffer* indexBuffer;
		ENGINE_HR_MESSAGE(Framework->GetDevice()->CreateBuffer(&indexBufferDesc, &indexSubresourceData, &indexBuffer), "Index Buffer could not be created.");
		IndexBuffers.emplace_back(indexBuffer);

		return static_cast<U16>(IndexBuffers.size() - 1);
	}

	U16 CRenderManager::AddMeshVertexOffset(U32 offset)
	{
		MeshVertexOffsets.emplace_back(offset);
		return static_cast<U16>(MeshVertexOffsets.size() - 1);
	}

	U16 CRenderManager::AddMeshVertexStride(U32 stride)
	{
		MeshVertexStrides.emplace_back(stride);
		return static_cast<U16>(MeshVertexStrides.size() - 1);
	}

	std::string CRenderManager::AddShader(const std::string& fileName, const EShaderType shaderType)
	{
		std::string outShaderData;

		switch (shaderType)
		{
		case EShaderType::Vertex:
		{
			ID3D11VertexShader* vertexShader;
			UGraphicsUtils::CreateVertexShader(fileName, Framework, &vertexShader, outShaderData);
			VertexShaders.emplace_back(vertexShader);
		}
		break;
		case EShaderType::Compute:
		case EShaderType::Geometry:
			break;
		case EShaderType::Pixel:
		{
			ID3D11PixelShader* pixelShader;
			UGraphicsUtils::CreatePixelShader(fileName, Framework, &pixelShader);
			PixelShaders.emplace_back(pixelShader);
		}
		break;
		}

		return outShaderData;
	}

	void CRenderManager::AddInputLayout(const std::string& vsData, EInputLayoutType layoutType)
	{
		std::vector<D3D11_INPUT_ELEMENT_DESC> layout;
		switch (layoutType)
		{
		case EInputLayoutType::Pos3Nor3Tan3Bit3UV2:
			layout =
			{
				{"POSITION"	,	0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
				{"NORMAL"   ,   0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
				{"TANGENT"  ,   0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
				{"BINORMAL" ,   0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
				{"UV"		,   0, DXGI_FORMAT_R32G32_FLOAT,	0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0}
			};
			break;
		}
		ID3D11InputLayout* inputLayout;
		ENGINE_HR_MESSAGE(Framework->GetDevice()->CreateInputLayout(layout.data(), static_cast<U32>(layout.size()), vsData.data(), vsData.size(), &inputLayout), "Input Layout could not be created.")
			InputLayouts.emplace_back(inputLayout);
	}

	void CRenderManager::AddSampler(ESamplerType samplerType)
	{
		// TODO.NR: Extend to different LOD levels and filters
		D3D11_SAMPLER_DESC samplerDesc = CD3D11_SAMPLER_DESC{ CD3D11_DEFAULT{} };
		samplerDesc.BorderColor[0] = 1.0f;
		samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
		samplerDesc.MinLOD = 0;
		samplerDesc.MaxLOD = 10;

		switch (samplerType)
		{
		case ESamplerType::Border:
			samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_BORDER;
			samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_BORDER;
			samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_BORDER;
			break;
		case ESamplerType::Clamp:
			samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
			samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
			samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
			break;
		case ESamplerType::Mirror:
			samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_MIRROR;
			samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_MIRROR;
			samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_MIRROR;
			break;
		case ESamplerType::Wrap:
			samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
			samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
			samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
			break;
		}

		ID3D11SamplerState* samplerState;
		ENGINE_HR_MESSAGE(Framework->GetDevice()->CreateSamplerState(&samplerDesc, &samplerState), "Sampler could not be created.");
		Samplers.emplace_back(samplerState);
	}

	void CRenderManager::AddTopology(D3D11_PRIMITIVE_TOPOLOGY topology)
	{
		Topologies.emplace_back(topology);
	}

	void CRenderManager::AddViewport(SVector2<F32> topLeftCoordinate, SVector2<F32> widthAndHeight, SVector2<F32> depth)
	{
		Viewports.emplace_back(D3D11_VIEWPORT(topLeftCoordinate.X, topLeftCoordinate.Y, widthAndHeight.X, widthAndHeight.Y, depth.X, depth.Y));
	}

	std::vector<U16> CRenderManager::AddMaterial(const std::string& materialName, EMaterialConfiguration configuration)
	{
		std::vector<U16> references;
		
		switch (configuration)
		{
		case EMaterialConfiguration::AlbedoMaterialNormal_Packed:
		{
			auto textures = CEngine::GetInstance()->GetMaterialHandler()->RequestMaterial(materialName);
			if (const auto it = std::ranges::find(MaterialNames, materialName); it == MaterialNames.end())
			{
				Textures.emplace_back(std::move(textures[0]));
				references.emplace_back(static_cast<U16>(Textures.size() - 1));
				Textures.emplace_back(std::move(textures[1]));
				references.emplace_back(static_cast<U16>(Textures.size() - 1));
				Textures.emplace_back(std::move(textures[2]));
				references.emplace_back(static_cast<U16>(Textures.size() - 1));
				MaterialNames.emplace_back(materialName);
			}
			else
			{
				const U16 materialTextureStartIndex = static_cast<U16>(std::distance(MaterialNames.begin(), it)) * TexturesPerMaterial;
				references.emplace_back(static_cast<U16>(materialTextureStartIndex));
				references.emplace_back(static_cast<U16>(materialTextureStartIndex + 1));
				references.emplace_back(static_cast<U16>(materialTextureStartIndex + 2));
			}
		}
			break;
		}

		return references;
	}

	bool SRenderCommandComparer::operator()(const SRenderCommand& a, const SRenderCommand& b) const
	{
		return 	static_cast<U16>(a.Type) > static_cast<U16>(b.Type);
	}
}
