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
#include "GeometryPrimitives.h"
#include "FileSystem/FileHeaderDeclarations.h"

#include <algorithm>
#include <future>

#include "FileSystem/FileSystem.h"
#include "Threading/ThreadManager.h"
#include "MaterialHandler.h"
#include "TextureBank.h"

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
	    , DirectionalLightBuffer(nullptr)
		, PointLightBuffer(nullptr)
		, SpotLightBuffer(nullptr)
		, ShadowmapBuffer(nullptr)
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
		ENGINE_HR_BOOL_MESSAGE(Framework->GetDevice()->CreateBuffer(&bufferDescription, nullptr, &DirectionalLightBuffer), "Directional Light Buffer could not be created.");

		bufferDescription.ByteWidth = sizeof(SPointLightBufferData);
		ENGINE_HR_BOOL_MESSAGE(Framework->GetDevice()->CreateBuffer(&bufferDescription, nullptr, &PointLightBuffer), "Point Light Buffer could not be created.");

		bufferDescription.ByteWidth = sizeof(SSpotLightBufferData);
		ENGINE_HR_BOOL_MESSAGE(Framework->GetDevice()->CreateBuffer(&bufferDescription, nullptr, &SpotLightBuffer), "Spot Light Buffer could not be created.");

		bufferDescription.ByteWidth = sizeof(SShadowmapBufferData) * 6;
		ENGINE_HR_BOOL_MESSAGE(Framework->GetDevice()->CreateBuffer(&bufferDescription, nullptr, &ShadowmapBuffer), "Shadowmap Buffer could not be created.");

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

		Backbuffer = FullscreenTextureFactory.CreateTexture(backbufferTexture);
		InitRenderTextures(windowHandler);

		// Load default resources
		const std::string vsData = AddShader("Shaders/DeferredModel_VS.cso", EShaderType::Vertex);
		AddInputLayout(vsData, EInputLayoutType::Pos3Nor3Tan3Bit3UV2);
		AddShader("Shaders/DeferredVertexShader_VS.cso", EShaderType::Vertex);

		AddShader("Shaders/GBuffer_PS.cso", EShaderType::Pixel);
		AddShader("Shaders/DeferredLightEnvironment_PS.cso", EShaderType::Pixel);

		AddSampler(ESamplerType::Wrap);
		AddSampler(ESamplerType::Border);
		AddTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		InitPointLightResources();
		InitSpotLightResources();
		InitEditorResources();
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

		//DefaultCubemap = CEngine::GetInstance()->GetMaterialHandler()->RequestCubemap("CubemapTheVisit");
		DefaultCubemap = CEngine::GetInstance()->GetTextureBank()->GetTexture("Assets/Textures/Cubemaps/CubemapTheVisit.hva");

		ShadowAtlasResolution = {8192.0f, 8192.0f};
		InitShadowmapAtlas(ShadowAtlasResolution);
		//myBoxLightShadowDepth = FullscreenTextureFactory.CreateDepth(aWindowHandler->GetResolution(), DXGI_FORMAT_R32_TYPELESS);
		DepthCopy = FullscreenTextureFactory.CreateTexture(windowHandler->GetResolution(), DXGI_FORMAT_R32_FLOAT);
		//myDownsampledDepth = FullscreenTextureFactory.CreateTexture(aWindowHandler->GetResolution() / 2.0f, DXGI_FORMAT_R32_FLOAT);

		IntermediateTexture = FullscreenTextureFactory.CreateTexture(ShadowAtlasResolution, DXGI_FORMAT_R16G16B16A16_FLOAT);
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
		InitShadowmapLOD(topLeftCoordinate, widthAndHeight, depth, atlasResolution, mapsInLod, 0);

		// LOD 2
		mapsInLod = 16;
		topLeftCoordinate = { 0.0f, atlasResolution.Y * 0.5f };
		widthAndHeight = atlasResolution * (1.0f / (mapsInLod / 2));
		InitShadowmapLOD(topLeftCoordinate, widthAndHeight, depth, atlasResolution, mapsInLod, 8);

		// LOD 3
		mapsInLod = 32;
		topLeftCoordinate = {0.0f, atlasResolution.Y * 0.75f };
		widthAndHeight = atlasResolution * (1.0f / (mapsInLod / 2));
		InitShadowmapLOD(topLeftCoordinate, widthAndHeight, depth, atlasResolution, mapsInLod, 24);

		// LOD 4
		mapsInLod = 128;
		topLeftCoordinate = { 0.0f, atlasResolution.Y * 0.875f };
		widthAndHeight = atlasResolution * (1.0f / (mapsInLod / 4));
		InitShadowmapLOD(topLeftCoordinate, widthAndHeight, depth, atlasResolution, mapsInLod, 56);
	}

	void CRenderManager::InitShadowmapLOD(SVector2<F32> topLeftCoordinate, const SVector2<F32>& widthAndHeight, const SVector2<F32>& depth, const SVector2<F32>& atlasResolution, U16 mapsInLod, U16 startIndex)
	{
		const float startingYCoordinate = topLeftCoordinate.Y;
		const U16 mapsPerRow = static_cast<U16>(atlasResolution.X / widthAndHeight.X);
		for (U16 i = startIndex; i < startIndex + mapsInLod; i++)
		{
			const U16 relativeIndex = i - startIndex;
			topLeftCoordinate.X = static_cast<U16>(relativeIndex % mapsPerRow) * widthAndHeight.X;
			topLeftCoordinate.Y = startingYCoordinate + static_cast<U16>(relativeIndex / mapsPerRow) * widthAndHeight.Y;
			AddViewport(topLeftCoordinate, widthAndHeight, depth);
		}
	}

	void CRenderManager::InitPointLightResources()
	{
		AddVertexBuffer<SPositionVertex>(PointLightCube);
		AddIndexBuffer(PointLightCubeIndices);

		AddMeshVertexStride(sizeof(SPositionVertex));
		AddMeshVertexOffset(0);

		const std::string vsData = AddShader("Shaders/PointLight_VS.cso", EShaderType::Vertex);
		AddInputLayout(vsData, EInputLayoutType::Pos4);

		AddShader("Shaders/DeferredLightPoint_PS.cso", EShaderType::Pixel);
	}

	void CRenderManager::InitSpotLightResources()
	{
		AddShader("Shaders/DeferredLightSpot_PS.cso", EShaderType::Pixel);
	}

	void CRenderManager::InitEditorResources()
	{
		AddShader("Shaders/EditorPreview_VS.cso", EShaderType::Vertex);
		AddShader("Shaders/EditorPreview_PS.cso", EShaderType::Pixel);
	}

	void CRenderManager::LoadDemoSceneResources()
	{
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

			const U16 commandsInHeap = static_cast<U16>(PopFromCommands->size());
			for (U16 i = 0; i < commandsInHeap; ++i)
			{
				SRenderCommand currentCommand = PopFromCommands->top();
				switch (currentCommand.Type)
				{
				case ERenderCommandType::ShadowAtlasPrePassDirectional:
				{
					const auto transformComp = currentCommand.GetComponent(TransformComponent);
					const auto staticMeshComp = currentCommand.GetComponent(StaticMeshComponent);
					const auto directionalLightComp = currentCommand.GetComponent(DirectionalLightComponent);

					FrameBufferData.ToCameraFromWorld = directionalLightComp->ShadowmapView.ShadowViewMatrix;
					FrameBufferData.ToWorldFromCamera = directionalLightComp->ShadowmapView.ShadowViewMatrix.FastInverse();
					FrameBufferData.ToProjectionFromCamera = directionalLightComp->ShadowmapView.ShadowProjectionMatrix;
					FrameBufferData.ToCameraFromProjection = directionalLightComp->ShadowmapView.ShadowProjectionMatrix.Inverse();
					FrameBufferData.CameraPosition = directionalLightComp->ShadowmapView.ShadowPosition;
					BindBuffer(FrameBuffer, FrameBufferData, "Frame Buffer");

					ObjectBufferData.ToWorldFromObject = transformComp->Transform.GetMatrix();
					BindBuffer(ObjectBuffer, ObjectBufferData, "Object Buffer");

					Context->VSSetConstantBuffers(0, 1, &FrameBuffer);
					Context->VSSetConstantBuffers(1, 1, &ObjectBuffer);
					Context->IASetPrimitiveTopology(Topologies[staticMeshComp->TopologyIndex]);
					Context->IASetInputLayout(InputLayouts[staticMeshComp->InputLayoutIndex]);

					Context->VSSetShader(VertexShaders[staticMeshComp->VertexShaderIndex], nullptr, 0);
					Context->PSSetShader(nullptr, nullptr, 0);

					Context->RSSetViewports(1, &Viewports[directionalLightComp->ShadowmapView.ShadowmapViewportIndex]);

					for (U8 drawCallIndex = 0; drawCallIndex < static_cast<U8>(staticMeshComp->DrawCallData.size()); drawCallIndex++)
					{
						const SDrawCallData& drawData = staticMeshComp->DrawCallData[drawCallIndex];
						ID3D11Buffer* vertexBuffer = VertexBuffers[drawData.VertexBufferIndex];
						Context->IASetVertexBuffers(0, 1, &vertexBuffer, &MeshVertexStrides[drawData.VertexStrideIndex], &MeshVertexOffsets[drawData.VertexStrideIndex]);
						Context->IASetIndexBuffer(IndexBuffers[drawData.IndexBufferIndex], DXGI_FORMAT_R32_UINT, 0);
						Context->DrawIndexed(drawData.IndexCount, 0, 0);
						CRenderManager::NumberOfDrawCallsThisFrame++;
					}
				}
				break;

				case ERenderCommandType::ShadowAtlasPrePassPoint:
				{
					const auto transformComp = currentCommand.GetComponent(TransformComponent);
					const auto staticMeshComp = currentCommand.GetComponent(StaticMeshComponent);
					const auto pointLightComp = currentCommand.GetComponent(PointLightComponent);

					ObjectBufferData.ToWorldFromObject = transformComp->Transform.GetMatrix();
					BindBuffer(ObjectBuffer, ObjectBufferData, "Object Buffer");

					Context->VSSetConstantBuffers(1, 1, &ObjectBuffer);
					Context->IASetPrimitiveTopology(Topologies[staticMeshComp->TopologyIndex]);
					Context->IASetInputLayout(InputLayouts[staticMeshComp->InputLayoutIndex]);

					Context->VSSetShader(VertexShaders[staticMeshComp->VertexShaderIndex], nullptr, 0);
					Context->PSSetShader(nullptr, nullptr, 0);

					for (const auto& shadowmapView : pointLightComp->ShadowmapViews)
					{
						FrameBufferData.ToCameraFromWorld = shadowmapView.ShadowViewMatrix;
						FrameBufferData.ToWorldFromCamera = shadowmapView.ShadowViewMatrix.FastInverse();
						FrameBufferData.ToProjectionFromCamera = shadowmapView.ShadowProjectionMatrix;
						FrameBufferData.ToCameraFromProjection = shadowmapView.ShadowProjectionMatrix.Inverse();
						FrameBufferData.CameraPosition = shadowmapView.ShadowPosition;
						BindBuffer(FrameBuffer, FrameBufferData, "Frame Buffer");

						Context->VSSetConstantBuffers(0, 1, &FrameBuffer);

						Context->RSSetViewports(1, &Viewports[shadowmapView.ShadowmapViewportIndex]);

						for (U8 drawCallIndex = 0; drawCallIndex < static_cast<U8>(staticMeshComp->DrawCallData.size()); drawCallIndex++)
						{
							const SDrawCallData& drawData = staticMeshComp->DrawCallData[drawCallIndex];
							ID3D11Buffer* vertexBuffer = VertexBuffers[drawData.VertexBufferIndex];
							Context->IASetVertexBuffers(0, 1, &vertexBuffer, &MeshVertexStrides[drawData.VertexStrideIndex], &MeshVertexOffsets[drawData.VertexStrideIndex]);
							Context->IASetIndexBuffer(IndexBuffers[drawData.IndexBufferIndex], DXGI_FORMAT_R32_UINT, 0);
							Context->DrawIndexed(drawData.IndexCount, 0, 0);
							CRenderManager::NumberOfDrawCallsThisFrame++;
						}
					}
				}
				break;

				case ERenderCommandType::ShadowAtlasPrePassSpot:
				{
					const auto transformComp = currentCommand.GetComponent(TransformComponent);
					const auto staticMeshComp = currentCommand.GetComponent(StaticMeshComponent);
					const auto spotLightComp = currentCommand.GetComponent(SpotLightComponent);

					FrameBufferData.ToCameraFromWorld = spotLightComp->ShadowmapView.ShadowViewMatrix;
					FrameBufferData.ToWorldFromCamera = spotLightComp->ShadowmapView.ShadowViewMatrix.FastInverse();
					FrameBufferData.ToProjectionFromCamera = spotLightComp->ShadowmapView.ShadowProjectionMatrix;
					FrameBufferData.ToCameraFromProjection = spotLightComp->ShadowmapView.ShadowProjectionMatrix.Inverse();
					FrameBufferData.CameraPosition = spotLightComp->ShadowmapView.ShadowPosition;
					BindBuffer(FrameBuffer, FrameBufferData, "Frame Buffer");

					ObjectBufferData.ToWorldFromObject = transformComp->Transform.GetMatrix();
					BindBuffer(ObjectBuffer, ObjectBufferData, "Object Buffer");

					Context->VSSetConstantBuffers(0, 1, &FrameBuffer);
					Context->VSSetConstantBuffers(1, 1, &ObjectBuffer);
					Context->IASetPrimitiveTopology(Topologies[staticMeshComp->TopologyIndex]);
					Context->IASetInputLayout(InputLayouts[staticMeshComp->InputLayoutIndex]);

					Context->VSSetShader(VertexShaders[staticMeshComp->VertexShaderIndex], nullptr, 0);
					Context->PSSetShader(nullptr, nullptr, 0);

					Context->RSSetViewports(1, &Viewports[spotLightComp->ShadowmapView.ShadowmapViewportIndex]);

					for (U8 drawCallIndex = 0; drawCallIndex < static_cast<U8>(staticMeshComp->DrawCallData.size()); drawCallIndex++)
					{
						const SDrawCallData& drawData = staticMeshComp->DrawCallData[drawCallIndex];
						ID3D11Buffer* vertexBuffer = VertexBuffers[drawData.VertexBufferIndex];
						Context->IASetVertexBuffers(0, 1, &vertexBuffer, &MeshVertexStrides[drawData.VertexStrideIndex], &MeshVertexOffsets[drawData.VertexStrideIndex]);
						Context->IASetIndexBuffer(IndexBuffers[drawData.IndexBufferIndex], DXGI_FORMAT_R32_UINT, 0);
						Context->DrawIndexed(drawData.IndexCount, 0, 0);
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

					Context->VSSetConstantBuffers(0, 1, &FrameBuffer);
					Context->PSSetConstantBuffers(0, 1, &FrameBuffer);
				}
				break;

				case ERenderCommandType::GBufferData:
				{
					const auto transformComp = currentCommand.GetComponent(TransformComponent);
					const auto staticMeshComp = currentCommand.GetComponent(StaticMeshComponent);
					const auto materialComp = currentCommand.GetComponent(MaterialComponent);

					ObjectBufferData.ToWorldFromObject = transformComp->Transform.GetMatrix();
					BindBuffer(ObjectBuffer, ObjectBufferData, "Object Buffer");

					Context->VSSetConstantBuffers(1, 1, &ObjectBuffer);
					Context->IASetPrimitiveTopology(Topologies[staticMeshComp->TopologyIndex]);
					Context->IASetInputLayout(InputLayouts[staticMeshComp->InputLayoutIndex]);

					Context->VSSetShader(VertexShaders[staticMeshComp->VertexShaderIndex], nullptr, 0);
					Context->PSSetShader(PixelShaders[staticMeshComp->PixelShaderIndex], nullptr, 0);

					ID3D11SamplerState* sampler = Samplers[staticMeshComp->SamplerIndex];
					Context->PSSetSamplers(0, 1, &sampler);

					auto textureBank = CEngine::GetInstance()->GetTextureBank();
					for (U8 drawCallIndex = 0; drawCallIndex < static_cast<U8>(staticMeshComp->DrawCallData.size()); drawCallIndex++)
					{
						// Load Textures
						std::vector<ID3D11ShaderResourceView*> resourceViewPointers;
						resourceViewPointers.resize(TexturesPerMaterial);
						for (U8 textureIndex = 0, pointerTracker = 0; textureIndex < TexturesPerMaterial; textureIndex++, pointerTracker++)
						{
							resourceViewPointers[pointerTracker] = textureBank->GetTexture(materialComp->MaterialReferences[textureIndex + drawCallIndex * TexturesPerMaterial]);
						}
						Context->PSSetShaderResources(5, TexturesPerMaterial, resourceViewPointers.data());

						const SDrawCallData& drawData = staticMeshComp->DrawCallData[drawCallIndex];
						ID3D11Buffer* vertexBuffer = VertexBuffers[drawData.VertexBufferIndex];
						Context->IASetVertexBuffers(0, 1, &vertexBuffer, &MeshVertexStrides[drawData.VertexStrideIndex], &MeshVertexOffsets[drawData.VertexStrideIndex]);
						Context->IASetIndexBuffer(IndexBuffers[drawData.IndexBufferIndex], DXGI_FORMAT_R32_UINT, 0);
						Context->DrawIndexed(drawData.IndexCount, 0, 0);
						CRenderManager::NumberOfDrawCallsThisFrame++;
					}
				}
				break;

				case ERenderCommandType::PreLightingPass:
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
				}
				break;

				case ERenderCommandType::DeferredLightingDirectional:
				{
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
					BindBuffer(DirectionalLightBuffer, DirectionalLightBufferData, "Light Buffer");
					Context->PSSetConstantBuffers(2, 1, &DirectionalLightBuffer);

					ShadowmapBufferData.ToShadowmapView = directionalLightComp->ShadowmapView.ShadowViewMatrix;
					ShadowmapBufferData.ToShadowmapProjection = directionalLightComp->ShadowmapView.ShadowProjectionMatrix;
					ShadowmapBufferData.ShadowmapPosition = directionalLightComp->ShadowmapView.ShadowPosition;

					const auto& viewport = Viewports[directionalLightComp->ShadowmapView.ShadowmapViewportIndex];
					ShadowmapBufferData.ShadowmapResolution = { viewport.Width, viewport.Height };
					ShadowmapBufferData.ShadowAtlasResolution = ShadowAtlasResolution;
					ShadowmapBufferData.ShadowmapStartingUV = { viewport.TopLeftX / ShadowAtlasResolution.X, viewport.TopLeftY / ShadowAtlasResolution.Y };
					ShadowmapBufferData.ShadowTestTolerance = 0.001f;
					
					BindBuffer(ShadowmapBuffer, ShadowmapBufferData, "Shadowmap Buffer");
					Context->PSSetConstantBuffers(5, 1, &ShadowmapBuffer);

					// Emissive Post Processing 
					//EmissiveBufferData.EmissiveStrength = GetPostProcessingBufferData().EmissiveStrength;
					//BindBuffer(EmissiveBuffer, EmissiveBufferData, "Emissive Buffer");
					//Context->PSSetConstantBuffers(5, 1, &EmissiveBuffer);

					Context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
					Context->IASetInputLayout(nullptr);
					Context->IASetVertexBuffers(0, 0, nullptr, nullptr, nullptr);
					Context->IASetIndexBuffer(nullptr, DXGI_FORMAT_UNKNOWN, 0);

					Context->VSSetShader(VertexShaders[1], nullptr, 0);
					Context->GSSetShader(nullptr, nullptr, 0);
					Context->PSSetShader(PixelShaders[1], nullptr, 0);

					Context->PSSetSamplers(0, 1, &Samplers[0]);
					Context->PSSetSamplers(1, 1, &Samplers[1]);

					Context->Draw(3, 0);
					CRenderManager::NumberOfDrawCallsThisFrame++;
				}
				break;

				case ERenderCommandType::DeferredLightingPoint: 
				{
					RenderedScene.SetAsActiveTarget();
					GBuffer.SetAllAsResources(1);
					IntermediateDepth.SetAsResourceOnSlot(21);
					RenderStateManager.SetBlendState(CRenderStateManager::EBlendStates::AdditiveBlend);

					ShadowAtlasDepth.SetAsResourceOnSlot(22);
					RenderStateManager.SetRasterizerState(CRenderStateManager::ERasterizerStates::FrontFaceCulling);

					// Update lightbufferdata and fill lightbuffer
					const auto pointLightComp = currentCommand.GetComponent(PointLightComponent);
					const auto transformComponent = currentCommand.GetComponent(TransformComponent);
					SVector position = transformComponent->Transform.GetMatrix().Translation();
					PointLightBufferData.ToWorldFromObject = transformComponent->Transform.GetMatrix();
					PointLightBufferData.ColorAndIntensity = pointLightComp->ColorAndIntensity;
					PointLightBufferData.PositionAndRange = { position.X, position.Y, position.Z, pointLightComp->Range };
					BindBuffer(PointLightBuffer, PointLightBufferData, "Point Light Buffer");
					Context->VSSetConstantBuffers(3, 1, &PointLightBuffer);
					Context->PSSetConstantBuffers(3, 1, &PointLightBuffer);

					SShadowmapBufferData shadowmapBufferData[6];
					for (U8 shadowmapViewIndex = 0; shadowmapViewIndex < 6; shadowmapViewIndex++)
					{
						shadowmapBufferData[shadowmapViewIndex].ToShadowmapView = pointLightComp->ShadowmapViews[shadowmapViewIndex].ShadowViewMatrix;
						shadowmapBufferData[shadowmapViewIndex].ToShadowmapProjection = pointLightComp->ShadowmapViews[shadowmapViewIndex].ShadowProjectionMatrix;
						shadowmapBufferData[shadowmapViewIndex].ShadowmapPosition = pointLightComp->ShadowmapViews[shadowmapViewIndex].ShadowPosition;

						const auto& viewport = Viewports[pointLightComp->ShadowmapViews[shadowmapViewIndex].ShadowmapViewportIndex];
						shadowmapBufferData[shadowmapViewIndex].ShadowmapResolution = { viewport.Width, viewport.Height };
						shadowmapBufferData[shadowmapViewIndex].ShadowAtlasResolution = ShadowAtlasResolution;
						shadowmapBufferData[shadowmapViewIndex].ShadowmapStartingUV = { viewport.TopLeftX / ShadowAtlasResolution.X, viewport.TopLeftY / ShadowAtlasResolution.Y };
						shadowmapBufferData[shadowmapViewIndex].ShadowTestTolerance = 0.00001f;
					}

					BindBuffer(ShadowmapBuffer, shadowmapBufferData, "Shadowmap Buffer");
					Context->PSSetConstantBuffers(5, 1, &ShadowmapBuffer);

					Context->IASetPrimitiveTopology(Topologies[0]);
					Context->IASetInputLayout(InputLayouts[1]);
					Context->IASetVertexBuffers(0, 1, &VertexBuffers[0], &MeshVertexStrides[0], &MeshVertexOffsets[0]);
					Context->IASetIndexBuffer(IndexBuffers[0], DXGI_FORMAT_R32_UINT, 0);

					Context->VSSetShader(VertexShaders[2], nullptr, 0);
					Context->PSSetShader(PixelShaders[2], nullptr, 0);

					Context->PSSetSamplers(0, 1, &Samplers[0]);
					Context->PSSetSamplers(1, 1, &Samplers[1]);

					Context->DrawIndexed(36, 0, 0);
					CRenderManager::NumberOfDrawCallsThisFrame++;
					RenderStateManager.SetRasterizerState(CRenderStateManager::ERasterizerStates::Default);
				}
				break;

				case ERenderCommandType::DeferredLightingSpot:
				{
					RenderedScene.SetAsActiveTarget();
					GBuffer.SetAllAsResources(1);
					IntermediateDepth.SetAsResourceOnSlot(21);
					RenderStateManager.SetBlendState(CRenderStateManager::EBlendStates::AdditiveBlend);

					ShadowAtlasDepth.SetAsResourceOnSlot(22);
					RenderStateManager.SetRasterizerState(CRenderStateManager::ERasterizerStates::FrontFaceCulling);

					// Update lightbufferdata and fill lightbuffer
					const auto spotLightComp = currentCommand.GetComponent(SpotLightComponent);
					const auto transformComponent = currentCommand.GetComponent(TransformComponent);
					SVector position = transformComponent->Transform.GetMatrix().Translation();
					PointLightBufferData.ToWorldFromObject = transformComponent->Transform.GetMatrix();
					PointLightBufferData.ColorAndIntensity = spotLightComp->ColorAndIntensity;
					PointLightBufferData.PositionAndRange = { position.X, position.Y, position.Z, spotLightComp->Range };
					
					BindBuffer(PointLightBuffer, PointLightBufferData, "Spotlight Vertex Shader Buffer");
					Context->VSSetConstantBuffers(3, 1, &PointLightBuffer);
					
					SpotLightBufferData.ColorAndIntensity = spotLightComp->ColorAndIntensity;
					SpotLightBufferData.PositionAndRange = { position.X, position.Y, position.Z, spotLightComp->Range };
					SpotLightBufferData.Direction = spotLightComp->Direction;
					SpotLightBufferData.DirectionNormal1 = spotLightComp->DirectionNormal1;
					SpotLightBufferData.DirectionNormal2 = spotLightComp->DirectionNormal2;
					SpotLightBufferData.OuterAngle = spotLightComp->OuterAngle;
					SpotLightBufferData.InnerAngle = spotLightComp->InnerAngle;
					
					BindBuffer(SpotLightBuffer, SpotLightBufferData, "Spotlight Pixel Shader Buffer");
					Context->PSSetConstantBuffers(3, 1, &SpotLightBuffer);

					SShadowmapBufferData shadowmapBufferData;
					shadowmapBufferData.ToShadowmapView = spotLightComp->ShadowmapView.ShadowViewMatrix;
					shadowmapBufferData.ToShadowmapProjection = spotLightComp->ShadowmapView.ShadowProjectionMatrix;
					shadowmapBufferData.ShadowmapPosition = spotLightComp->ShadowmapView.ShadowPosition;

					const auto& viewport = Viewports[spotLightComp->ShadowmapView.ShadowmapViewportIndex];
					shadowmapBufferData.ShadowmapResolution = { viewport.Width, viewport.Height };
					shadowmapBufferData.ShadowAtlasResolution = ShadowAtlasResolution;
					shadowmapBufferData.ShadowmapStartingUV = { viewport.TopLeftX / ShadowAtlasResolution.X, viewport.TopLeftY / ShadowAtlasResolution.Y };
					shadowmapBufferData.ShadowTestTolerance = 0.00001f;

					BindBuffer(ShadowmapBuffer, shadowmapBufferData, "Shadowmap Buffer");
					Context->PSSetConstantBuffers(5, 1, &ShadowmapBuffer);

					Context->IASetPrimitiveTopology(Topologies[0]);
					Context->IASetInputLayout(InputLayouts[1]);
					Context->IASetVertexBuffers(0, 1, &VertexBuffers[0], &MeshVertexStrides[0], &MeshVertexOffsets[0]);
					Context->IASetIndexBuffer(IndexBuffers[0], DXGI_FORMAT_R32_UINT, 0);

					// Use Point Light Vertex Shader
					Context->VSSetShader(VertexShaders[2], nullptr, 0);
					Context->PSSetShader(PixelShaders[3], nullptr, 0);

					Context->PSSetSamplers(0, 1, &Samplers[0]);
					Context->PSSetSamplers(1, 1, &Samplers[1]);

					Context->DrawIndexed(36, 0, 0);
					CRenderManager::NumberOfDrawCallsThisFrame++;
					RenderStateManager.SetRasterizerState(CRenderStateManager::ERasterizerStates::Default);
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

			//// Draw debug shadow atlas
			//D3D11_VIEWPORT viewport;
			//viewport.TopLeftX = 0.0f;
			//viewport.TopLeftY = 0.0f;
			//viewport.Width = 256.0f;
			//viewport.Height = 256.0f;
			//viewport.MinDepth = 0.0f;
			//viewport.MaxDepth = 1.0f;
			//Context->RSSetViewports(1, &viewport);
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

	void CRenderManager::ConvertToHVA(const std::string& fileName, EAssetType assetType)
	{
		switch (assetType)
		{
		case EAssetType::StaticMesh:
			{
				//SStaticMeshVertex vertices[24] =
				//{
				//	// X      Y      Z        nX, nY, nZ    tX, tY, tZ,    bX, bY, bZ,    UV	
				//	{ -0.5f, -0.5f, -0.5f,   -1,  0,  0,    0,  0,  1,     0,  1,  0,     0, 0 }, // 0
				//	{  0.5f, -0.5f, -0.5f,    1,  0,  0,    0,  0, -1,     0,  1,  0,     1, 0 }, // 1
				//	{ -0.5f,  0.5f, -0.5f,   -1,  0,  0,    0,  0,  1,     0,  1,  0,     0, 1 }, // 2
				//	{  0.5f,  0.5f, -0.5f,    1,  0,  0,    0,  0, -1,     0,  1,  0,     1, 1 }, // 3
				//	{ -0.5f, -0.5f,  0.5f,   -1,  0,  0,    0,  0,  1,     0,  1,  0,     0, 0 }, // 4
				//	{  0.5f, -0.5f,  0.5f,    1,  0,  0,    0,  0, -1,     0,  1,  0,     1, 0 }, // 5
				//	{ -0.5f,  0.5f,  0.5f,   -1,  0,  0,    0,  0,  1,     0,  1,  0,     0, 1 }, // 6
				//	{  0.5f,  0.5f,  0.5f,    1,  0,  0,    0,  0, -1,     0,  1,  0,     1, 1 }, // 7
				//	// X      Y      Z        nX, nY, nZ    nX, nY, nZ,    nX, nY, nZ,    UV	  
				//	{ -0.5f, -0.5f, -0.5f,    0, -1,  0,    1,  0,  0,     0,  0,  1,     0, 0 }, // 8  // 0
				//	{  0.5f, -0.5f, -0.5f,    0, -1,  0,    1,  0,  0,     0,  0,  1,     1, 0 }, // 9	// 1
				//	{ -0.5f,  0.5f, -0.5f,    0,  1,  0,   -1,  0,  0,     0,  0,  1,     0, 0 }, // 10	// 2
				//	{  0.5f,  0.5f, -0.5f,    0,  1,  0,   -1,  0,  0,     0,  0,  1,     1, 0 }, // 11	// 3
				//	{ -0.5f, -0.5f,  0.5f,    0, -1,  0,    1,  0,  0,     0,  0,  1,     0, 1 }, // 12	// 4
				//	{  0.5f, -0.5f,  0.5f,    0, -1,  0,    1,  0,  0,     0,  0,  1,     0, 1 }, // 13	// 5
				//	{ -0.5f,  0.5f,  0.5f,    0,  1,  0,   -1,  0,  0,     0,  0,  1,     1, 1 }, // 14	// 6
				//	{  0.5f,  0.5f,  0.5f,    0,  1,  0,   -1,  0,  0,     0,  0,  1,     1, 1 }, // 15	// 7
				//	// X      Y      Z        nX, nY, nZ    nX, nY, nZ,    nX, nY, nZ,    UV	  
				//	{ -0.5f, -0.5f, -0.5f,    0,  0, -1,   -1,  0,  0,     0,  1,  0,     0, 0 }, // 16 // 0
				//	{  0.5f, -0.5f, -0.5f,    0,  0, -1,   -1,  0,  0,     0,  1,  0,     0, 0 }, // 17	// 1
				//	{ -0.5f,  0.5f, -0.5f,    0,  0, -1,   -1,  0,  0,     0,  1,  0,     1, 0 }, // 18	// 2
				//	{  0.5f,  0.5f, -0.5f,    0,  0, -1,   -1,  0,  0,     0,  1,  0,     1, 0 }, // 19	// 3
				//	{ -0.5f, -0.5f,  0.5f,    0,  0,  1,    1,  0,  0,     0,  1,  0,     0, 1 }, // 20	// 4
				//	{  0.5f, -0.5f,  0.5f,    0,  0,  1,    1,  0,  0,     0,  1,  0,     1, 1 }, // 21	// 5
				//	{ -0.5f,  0.5f,  0.5f,    0,  0,  1,    1,  0,  0,     0,  1,  0,     0, 1 }, // 22	// 6
				//	{  0.5f,  0.5f,  0.5f,    0,  0,  1,    1,  0,  0,     0,  1,  0,     1, 1 }  // 23	// 7
				//};
				//U32 indices[36] =
				//{
				//	0,4,2,
				//	4,6,2,
				//	1,3,5,
				//	3,7,5,
				//	8,9,12,
				//	9,13,12,
				//	10,14,11,
				//	14,15,11,
				//	16,18,17,
				//	18,19,17,
				//	20,21,22,
				//	21,23,22
				//};

				//SStaticModelFileHeader asset;
				//asset.AssetType = EAssetType::StaticMesh;
				//asset.Name = "PrimitiveCube";
				//asset.NameLength = static_cast<U32>(asset.Name.length());
				//asset.Meshes.emplace_back();
				//asset.Meshes.back().NumberOfVertices = 24;
				//asset.Meshes.back().Vertices.assign(vertices, vertices + 24);
				//asset.Meshes.back().NumberOfIndices = 36;
				//asset.Meshes.back().Indices.assign(indices, indices + 36);

				//const auto data = new char[asset.GetSize()];

				//asset.Serialize(data);
				//CEngine::GetInstance()->GetFileSystem()->Serialize(fileName, &data[0], asset.GetSize());
				//delete[] data;

				CModelImporter::ImportFBX(fileName);
			}
			break;
		case EAssetType::Texture:
			{
				std::string textureFileData;
				CEngine::GetInstance()->GetFileSystem()->Deserialize(fileName, textureFileData);

				ETextureFormat format = {};
				if (const std::string extension = fileName.substr(fileName.size() - 4); extension == ".dds")
					format = ETextureFormat::DDS;
				else if (extension == ".tga")
					format = ETextureFormat::TGA;

				STextureFileHeader asset;
				asset.AssetType = EAssetType::Texture;
				asset.MaterialName = fileName.substr(0, fileName.find_last_of("."));
				asset.MaterialNameLength = static_cast<U32>(asset.MaterialName.length());
				asset.OriginalFormat = format;
				asset.Suffix = fileName[fileName.find_last_of(".") - 1];
				asset.DataSize = static_cast<U32>(textureFileData.length() * sizeof(char));
				asset.Data = std::move(textureFileData);

				const auto data = new char[asset.GetSize()];

				asset.Serialize(data);
				CEngine::GetInstance()->GetFileSystem()->Serialize(asset.MaterialName + ".hva", &data[0], asset.GetSize());
				
				delete[] data;
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
			delete[] data;
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

	//ID3D11ShaderResourceView* CRenderManager::GetTexture(I64 textureIndex) const
	//{
	//	return Textures[textureIndex];
	//}

	//const std::vector<ID3D11ShaderResourceView*>& CRenderManager::GetTextures() const
	//{
	//	return Textures;
	//}

	EMaterialConfiguration CRenderManager::GetMaterialConfiguration() const
	{
		return MaterialConfiguration;
	}

	SVector2<F32> CRenderManager::GetShadowAtlasResolution() const
	{
		return ShadowAtlasResolution;
	}

	void* CRenderManager::RenderStaticMeshAssetTexture(const std::string& fileName)
	{
		D3D11_TEXTURE2D_DESC textureDesc = { 0 };
		textureDesc.Width = static_cast<U16>(256.0f);
		textureDesc.Height = static_cast<U16>(256.0f);
		textureDesc.MipLevels = 1;
		textureDesc.ArraySize = 1;
		textureDesc.Format = DXGI_FORMAT_R16G16B16A16_FLOAT;
		textureDesc.SampleDesc.Count = 1;
		textureDesc.SampleDesc.Quality = 0;
		textureDesc.Usage = D3D11_USAGE_DEFAULT;
		textureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
		textureDesc.CPUAccessFlags = 0;
		textureDesc.MiscFlags = 0;

		D3D11_TEXTURE2D_DESC depthStencilDesc = { 0 };
		depthStencilDesc.Width = static_cast<U16>(256.0f);
		depthStencilDesc.Height = static_cast<U16>(256.0f);
		depthStencilDesc.MipLevels = 1;
		depthStencilDesc.ArraySize = 1;
		depthStencilDesc.Format = DXGI_FORMAT_D32_FLOAT;
		depthStencilDesc.SampleDesc.Count = 1;
		depthStencilDesc.SampleDesc.Quality = 0;
		depthStencilDesc.Usage = D3D11_USAGE_DEFAULT;
		depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		depthStencilDesc.CPUAccessFlags = 0;
		depthStencilDesc.MiscFlags = 0;

		D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;
		depthStencilViewDesc.Format = DXGI_FORMAT_D32_FLOAT;
		depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
		depthStencilViewDesc.Texture2D.MipSlice = 0;
		depthStencilViewDesc.Flags = 0;

		ID3D11Texture2D* depthStencilBuffer;
		ENGINE_HR_MESSAGE(Framework->GetDevice()->CreateTexture2D(&depthStencilDesc, nullptr, &depthStencilBuffer), "Texture could not be created.");
		
		ID3D11DepthStencilView* depthStencilView;
		ENGINE_HR_MESSAGE(Framework->GetDevice()->CreateDepthStencilView(depthStencilBuffer, &depthStencilViewDesc, &depthStencilView), "Depth could not be created.");

		ID3D11Texture2D* texture;
		ENGINE_HR_MESSAGE(Framework->GetDevice()->CreateTexture2D(&textureDesc, nullptr, &texture), "Could not create Fullscreen Texture2D");

		ID3D11ShaderResourceView* shaderResource;
		ENGINE_HR_MESSAGE(Framework->GetDevice()->CreateShaderResourceView(texture, nullptr, &shaderResource), "Could not create Fullscreen Shader Resource View.");

		ID3D11RenderTargetView* renderTarget;
		ENGINE_HR_MESSAGE(Framework->GetDevice()->CreateRenderTargetView(texture, nullptr, &renderTarget), "Could not create Fullcreen Render Target View.");

		D3D11_VIEWPORT* viewport;
		D3D11_TEXTURE2D_DESC textureDescription;
		texture->GetDesc(&textureDescription);
		viewport = new D3D11_VIEWPORT({ 0.0f, 0.0f, static_cast<F32>(textureDescription.Width), static_cast<F32>(textureDescription.Height), 0.0f, 1.0f });

		// TODO.NR: Figure out why the depth doesn't work
		Context->OMSetRenderTargets(1, &renderTarget, nullptr);
		Context->RSSetViewports(1, viewport);

		STransform camTransform;
		camTransform.Translate(SVector4::Right * 1.5f);
		camTransform.Translate(SVector4::Backward * 2.5f);
		camTransform.Translate(SVector4::Up * 2.0f);
		camTransform.Rotate({UMath::DegToRad(-30.0f), UMath::DegToRad(30.0f), 0.0f});
		SMatrix camProjection = SMatrix::PerspectiveFovLH(UMath::DegToRad(70.0f), (16.0f / 9.0f), 0.1f, 10.0f);

		FrameBufferData.ToCameraFromWorld = camTransform.GetMatrix().FastInverse();
		FrameBufferData.ToWorldFromCamera = camTransform.GetMatrix();
		FrameBufferData.ToProjectionFromCamera = camProjection;
		FrameBufferData.ToCameraFromProjection = camProjection.Inverse();
		FrameBufferData.CameraPosition = camTransform.GetMatrix().Translation4();
		BindBuffer(FrameBuffer, FrameBufferData, "Frame Buffer");

		Context->VSSetConstantBuffers(0, 1, &FrameBuffer);
		Context->PSSetConstantBuffers(0, 1, &FrameBuffer);

		ObjectBufferData.ToWorldFromObject = SMatrix();
		BindBuffer(ObjectBuffer, ObjectBufferData, "Object Buffer");

		Ref<SEntity> tempEntity = std::make_shared<SEntity>(0, "Temp");
		SStaticMeshComponent* staticMeshComp = new SStaticMeshComponent(tempEntity, EComponentType::StaticMeshComponent);
		LoadStaticMeshComponent(fileName, staticMeshComp);

		Context->VSSetConstantBuffers(1, 1, &ObjectBuffer);
		Context->IASetPrimitiveTopology(Topologies[staticMeshComp->TopologyIndex]);
		Context->IASetInputLayout(InputLayouts[staticMeshComp->InputLayoutIndex]);

		Context->VSSetShader(VertexShaders[3], nullptr, 0);
		Context->PSSetShader(PixelShaders[4], nullptr, 0);

		ID3D11SamplerState* sampler = Samplers[staticMeshComp->SamplerIndex];
		Context->PSSetSamplers(0, 1, &sampler);

		for (U8 drawCallIndex = 0; drawCallIndex < static_cast<U8>(staticMeshComp->DrawCallData.size()); drawCallIndex++)
		{
			const SDrawCallData& drawData = staticMeshComp->DrawCallData[drawCallIndex];
			ID3D11Buffer* vertexBuffer = VertexBuffers[drawData.VertexBufferIndex];
			Context->IASetVertexBuffers(0, 1, &vertexBuffer, &MeshVertexStrides[drawData.VertexStrideIndex], &MeshVertexOffsets[drawData.VertexStrideIndex]);
			Context->IASetIndexBuffer(IndexBuffers[drawData.IndexBufferIndex], DXGI_FORMAT_R32_UINT, 0);
			Context->DrawIndexed(drawData.IndexCount, 0, 0);
			CRenderManager::NumberOfDrawCallsThisFrame++;
		}

		delete staticMeshComp;
		delete viewport;
		renderTarget->Release();
		texture->Release();
		depthStencilView->Release();
		depthStencilBuffer->Release();

		return std::move((void*)shaderResource);
	}

	void* CRenderManager::GetTextureAssetTexture(const std::string& fileName)
	{
		// Asset Loading
		const U64 fileSize = CEngine::GetInstance()->GetFileSystem()->GetFileSize(fileName);
		char* data = new char[fileSize];

		CEngine::GetInstance()->GetFileSystem()->Deserialize(fileName, data, static_cast<U32>(fileSize));

		STextureFileHeader assetFile;
		assetFile.Deserialize(data);
		STextureAsset asset = STextureAsset(assetFile, Framework->GetDevice());

		return asset.ShaderResourceView;
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

		case EInputLayoutType::Pos4:
			layout =
			{
				{"POSITION"	,	0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0}
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
		samplerDesc.BorderColor[1] = 1.0f;
		samplerDesc.BorderColor[2] = 1.0f;
		samplerDesc.BorderColor[3] = 1.0f;
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
			const std::string texturesFolder = "Assets/Textures/";
			auto textureBank = CEngine::GetInstance()->GetTextureBank();

			references.emplace_back(static_cast<U16>(textureBank->GetTextureIndex(texturesFolder + materialName + "_c.hva")));
			references.emplace_back(static_cast<U16>(textureBank->GetTextureIndex(texturesFolder + materialName + "_m.hva")));
			references.emplace_back(static_cast<U16>(textureBank->GetTextureIndex(texturesFolder + materialName + "_n.hva")));
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
