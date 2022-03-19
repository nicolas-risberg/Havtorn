// Copyright 2022 Team Havtorn. All Rights Reserved.

#include "hvpch.h"
#include "RenderManager.h"
#include "GraphicsFramework.h"
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
#include "VertexStructs.h"
#include "FileSystem/FileHeaderDeclarations.h"

#include <algorithm>
#include <future>

#include "FileSystem/FileSystem.h"
#include "Threading/ThreadManager.h"

namespace Havtorn
{
	unsigned int CRenderManager::NumberOfDrawCallsThisFrame = 0;

	CRenderManager::CRenderManager()
		: DoFullRender(true)
		, UseAntiAliasing(true)
		, UseBrokenScreenPass(false)
		, ClearColor(0.5f, 0.5f, 0.5f, 1.0f)
		, RenderPassIndex(0)
		, Framework(nullptr)
		, Context(nullptr)
		, FrameBuffer(nullptr)
		, ObjectBuffer(nullptr)
		, DemoVertexShader(nullptr)
		, DemoPixelShader(nullptr)
		, SamplerState(nullptr)
		, DemoVertexBuffer(nullptr)
		, DemoIndexBuffer(nullptr)
		, DemoInputLayout(nullptr)
		, DemoTopology(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST)
		, DemoNumberOfVertices(0)
		, DemoNumberOfIndices(0)
		, DemoStride(0)
		, DemoOffset(0)
	{
		PushToCommands = &RenderCommandsA;
		PopFromCommands = &RenderCommandsB;
	}

	CRenderManager::~CRenderManager()
	{
		Release();
	}

	bool CRenderManager::Init(CDirectXFramework* framework, CWindowHandler* windowHandler)
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

#pragma region Demo Resources
		std::string vsData;
		UGraphicsUtils::CreateVertexShader("Shaders/Demo_VS.cso", framework, &DemoVertexShader, vsData);
		UGraphicsUtils::CreatePixelShader("Shaders/Demo_PS.cso", framework, &DemoPixelShader);
		VertexShaders.emplace_back(DemoVertexShader);
		PixelShaders.emplace_back(DemoPixelShader);

		D3D11_SAMPLER_DESC samplerDesc = {};
		samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
		samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.MinLOD = 0;
		samplerDesc.MaxLOD = 10;
		ENGINE_HR_BOOL_MESSAGE(framework->GetDevice()->CreateSamplerState(&samplerDesc, &SamplerState), "Sampler could not be created.");
		Samplers.emplace_back(SamplerState);

		SStaticMeshVertex vertices[24] =
		{
			// X      Y      Z      W,    nX, nY, nZ, nW,   tX, tY, tZ, tW,    bX, bY, bZ, bW,    UV	
			{ -0.5f, -0.5f, -0.5f,  1,   -1,  0,  0,  0,    0,  0,  1,  0,     0,  1,  0,  0,     0, 0 }, // 0
			{  0.5f, -0.5f, -0.5f,  1,    1,  0,  0,  0,    0,  0, -1,  0,     0,  1,  0,  0,     1, 0 }, // 1
			{ -0.5f,  0.5f, -0.5f,  1,   -1,  0,  0,  0,    0,  0,  1,  0,     0,  1,  0,  0,     0, 1 }, // 2
			{  0.5f,  0.5f, -0.5f,  1,    1,  0,  0,  0,    0,  0, -1,  0,     0,  1,  0,  0,     1, 1 }, // 3
			{ -0.5f, -0.5f,  0.5f,  1,   -1,  0,  0,  0,    0,  0,  1,  0,     0,  1,  0,  0,     0, 0 }, // 4
			{  0.5f, -0.5f,  0.5f,  1,    1,  0,  0,  0,    0,  0, -1,  0,     0,  1,  0,  0,     1, 0 }, // 5
			{ -0.5f,  0.5f,  0.5f,  1,   -1,  0,  0,  0,    0,  0,  1,  0,     0,  1,  0,  0,     0, 1 }, // 6
			{  0.5f,  0.5f,  0.5f,  1,    1,  0,  0,  0,    0,  0, -1,  0,     0,  1,  0,  0,     1, 1 }, // 7
			// X      Y      Z      W,    nX, nY, nZ, nW,   nX, nY, nZ, nW,    nX, nY, nZ, nW,    UV	  
			{ -0.5f, -0.5f, -0.5f,  1,    0, -1,  0,  0,    1,  0,  0,  0,     0,  0,  1,  0,     0, 0 }, // 8  // 0
			{  0.5f, -0.5f, -0.5f,  1,    0, -1,  0,  0,    1,  0,  0,  0,     0,  0,  1,  0,     1, 0 }, // 9	// 1
			{ -0.5f,  0.5f, -0.5f,  1,    0,  1,  0,  0,   -1,  0,  0,  0,     0,  0,  1,  0,     0, 0 }, // 10	// 2
			{  0.5f,  0.5f, -0.5f,  1,    0,  1,  0,  0,   -1,  0,  0,  0,     0,  0,  1,  0,     1, 0 }, // 11	// 3
			{ -0.5f, -0.5f,  0.5f,  1,    0, -1,  0,  0,    1,  0,  0,  0,     0,  0,  1,  0,     0, 1 }, // 12	// 4
			{  0.5f, -0.5f,  0.5f,  1,    0, -1,  0,  0,    1,  0,  0,  0,     0,  0,  1,  0,     0, 1 }, // 13	// 5
			{ -0.5f,  0.5f,  0.5f,  1,    0,  1,  0,  0,   -1,  0,  0,  0,     0,  0,  1,  0,     1, 1 }, // 14	// 6
			{  0.5f,  0.5f,  0.5f,  1,    0,  1,  0,  0,   -1,  0,  0,  0,     0,  0,  1,  0,     1, 1 }, // 15	// 7
			// X      Y      Z      W,    nX, nY, nZ, nW,   nX, nY, nZ, nW,    nX, nY, nZ, nW,    UV	  
			{ -0.5f, -0.5f, -0.5f,  1,    0,  0, -1,  0,   -1,  0,  0,  0,     0,  1,  0,  0,     0, 0 }, // 16 // 0
			{  0.5f, -0.5f, -0.5f,  1,    0,  0, -1,  0,   -1,  0,  0,  0,     0,  1,  0,  0,     0, 0 }, // 17	// 1
			{ -0.5f,  0.5f, -0.5f,  1,    0,  0, -1,  0,   -1,  0,  0,  0,     0,  1,  0,  0,     1, 0 }, // 18	// 2
			{  0.5f,  0.5f, -0.5f,  1,    0,  0, -1,  0,   -1,  0,  0,  0,     0,  1,  0,  0,     1, 0 }, // 19	// 3
			{ -0.5f, -0.5f,  0.5f,  1,    0,  0,  1,  0,    1,  0,  0,  0,     0,  1,  0,  0,     0, 1 }, // 20	// 4
			{  0.5f, -0.5f,  0.5f,  1,    0,  0,  1,  0,    1,  0,  0,  0,     0,  1,  0,  0,     1, 1 }, // 21	// 5
			{ -0.5f,  0.5f,  0.5f,  1,    0,  0,  1,  0,    1,  0,  0,  0,     0,  1,  0,  0,     0, 1 }, // 22	// 6
			{  0.5f,  0.5f,  0.5f,  1,    0,  0,  1,  0,    1,  0,  0,  0,     0,  1,  0,  0,     1, 1 }  // 23	// 7
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

		D3D11_BUFFER_DESC demoVertexBufferDesc = { 0 };
		demoVertexBufferDesc.ByteWidth = sizeof(vertices);
		demoVertexBufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
		demoVertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

		D3D11_SUBRESOURCE_DATA demoSubVertexResourceData = { 0 };
		demoSubVertexResourceData.pSysMem = vertices;

		ENGINE_HR_BOOL_MESSAGE(framework->GetDevice()->CreateBuffer(&demoVertexBufferDesc, &demoSubVertexResourceData, &DemoVertexBuffer), "Demo Vertex Buffer could not be created.");
		VertexBuffers.emplace_back(DemoVertexBuffer);

		D3D11_BUFFER_DESC demoIndexBufferDesc = { 0 };
		demoIndexBufferDesc.ByteWidth = sizeof(indices);
		demoIndexBufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
		demoIndexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;

		D3D11_SUBRESOURCE_DATA demoIndexSubresourceData = { 0 };
		demoIndexSubresourceData.pSysMem = indices;

		ENGINE_HR_BOOL_MESSAGE(framework->GetDevice()->CreateBuffer(&demoIndexBufferDesc, &demoIndexSubresourceData, &DemoIndexBuffer), "Demo Index Buffer could not be created.");
		IndexBuffers.emplace_back(DemoIndexBuffer);

		DemoNumberOfVertices = static_cast<U32>(sizeof(vertices) / sizeof(SStaticMeshVertex));
		DemoNumberOfIndices = static_cast<U32>(sizeof(indices) / sizeof(U32));
		DemoStride = sizeof(SStaticMeshVertex);
		DemoOffset = 0;
		MeshVertexStrides.emplace_back(DemoStride);
		MeshVertexOffsets.emplace_back(DemoOffset);

		D3D11_INPUT_ELEMENT_DESC demoLayout[] =
		{
			{"POSITION"	,	0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
			{"NORMAL"   ,   0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
			{"TANGENT"  ,   0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
			{"BINORMAL" ,   0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
			{"UV"		,   0, DXGI_FORMAT_R32G32_FLOAT,	   0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0}
		};
		ENGINE_HR_MESSAGE(framework->GetDevice()->CreateInputLayout(demoLayout, sizeof(demoLayout) / sizeof(D3D11_INPUT_ELEMENT_DESC), vsData.data(), vsData.size(), &DemoInputLayout), "Demo Input Layout could not be created.");
		InputLayouts.emplace_back(DemoInputLayout);

		DemoTopology = D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
		Topologies.emplace_back(DemoTopology);

#pragma endregion Demo Resources

		WriteAssetFile("ExampleCube.hvasset", EAssetType::StaticMesh);
		LoadStaticMesh("ExampleCube.hvasset", nullptr);

		return true;
	}

	bool CRenderManager::ReInit(CDirectXFramework* framework, CWindowHandler* windowHandler)
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

	void CRenderManager::Render()
	{
		while (true)
		{
			std::unique_lock<std::mutex> uniqueLock(CThreadManager::RenderMutex);
			CThreadManager::RenderCondition.wait(uniqueLock, [] 
				{return CThreadManager::RenderThreadStatus == ERenderThreadStatus::ReadyToRender; });

			CRenderManager::NumberOfDrawCallsThisFrame = 0;
			RenderStateManager.SetAllDefault();
			
			RenderedScene.ClearTexture(ClearColor);
			IntermediateDepth.ClearDepth();
			RenderedScene.SetAsActiveTarget(&IntermediateDepth);

			ID3D11DeviceContext* context = Framework->GetContext();

			for (U16 i = 0; i < PopFromCommands->size(); ++i)
			{
				SRenderCommand currentCommand = PopFromCommands->top();
				switch (currentCommand.Type)
				{
				case ERenderCommandType::CameraDataStorage:
				{
					auto transformComp = currentCommand.GetComponent(TransformComponent);
					auto cameraComp = currentCommand.GetComponent(CameraComponent);

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

					ObjectBufferData.ToWorldFromObject = transformComp->Transform.GetMatrix();
					BindBuffer(ObjectBuffer, ObjectBufferData, "Object Buffer");

					context->VSSetConstantBuffers(1, 1, &ObjectBuffer);
					context->IASetPrimitiveTopology(Topologies[staticMeshComp->TopologyIndex]);
					context->IASetInputLayout(InputLayouts[staticMeshComp->InputLayoutIndex]);

					ID3D11Buffer* vertexBuffer = VertexBuffers[staticMeshComp->VertexBufferIndex];
					context->IASetVertexBuffers(0, 1, &vertexBuffer, &MeshVertexStrides[staticMeshComp->VertexStrideIndex], &MeshVertexOffsets[staticMeshComp->VertexStrideIndex]);
					context->IASetIndexBuffer(IndexBuffers[staticMeshComp->IndexBufferIndex], DXGI_FORMAT_R32_UINT, 0);

					context->VSSetShader(VertexShaders[staticMeshComp->VertexShaderIndex], nullptr, 0);
					context->PSSetShader(PixelShaders[staticMeshComp->PixelShaderIndex], nullptr, 0);

					ID3D11SamplerState* sampler = Samplers[staticMeshComp->SamplerIndex];
					context->PSSetSamplers(0, 1, &sampler);

					context->DrawIndexed(staticMeshComp->IndexCount, 0, 0);
					CRenderManager::NumberOfDrawCallsThisFrame++;
				}
				break;
				default:
					break;
				}
				PopFromCommands->pop();
			}

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
			// X      Y      Z      W,    nX, nY, nZ, nW,   tX, tY, tZ, tW,    bX, bY, bZ, bW,    UV	
			{ -0.5f, -0.5f, -0.5f,  1,   -1,  0,  0,  0,    0,  0,  1,  0,     0,  1,  0,  0,     0, 0 }, // 0
			{  0.5f, -0.5f, -0.5f,  1,    1,  0,  0,  0,    0,  0, -1,  0,     0,  1,  0,  0,     1, 0 }, // 1
			{ -0.5f,  0.5f, -0.5f,  1,   -1,  0,  0,  0,    0,  0,  1,  0,     0,  1,  0,  0,     0, 1 }, // 2
			{  0.5f,  0.5f, -0.5f,  1,    1,  0,  0,  0,    0,  0, -1,  0,     0,  1,  0,  0,     1, 1 }, // 3
			{ -0.5f, -0.5f,  0.5f,  1,   -1,  0,  0,  0,    0,  0,  1,  0,     0,  1,  0,  0,     0, 0 }, // 4
			{  0.5f, -0.5f,  0.5f,  1,    1,  0,  0,  0,    0,  0, -1,  0,     0,  1,  0,  0,     1, 0 }, // 5
			{ -0.5f,  0.5f,  0.5f,  1,   -1,  0,  0,  0,    0,  0,  1,  0,     0,  1,  0,  0,     0, 1 }, // 6
			{  0.5f,  0.5f,  0.5f,  1,    1,  0,  0,  0,    0,  0, -1,  0,     0,  1,  0,  0,     1, 1 }, // 7
			// X      Y      Z      W,    nX, nY, nZ, nW,   nX, nY, nZ, nW,    nX, nY, nZ, nW,    UV	  
			{ -0.5f, -0.5f, -0.5f,  1,    0, -1,  0,  0,    1,  0,  0,  0,     0,  0,  1,  0,     0, 0 }, // 8  // 0
			{  0.5f, -0.5f, -0.5f,  1,    0, -1,  0,  0,    1,  0,  0,  0,     0,  0,  1,  0,     1, 0 }, // 9	// 1
			{ -0.5f,  0.5f, -0.5f,  1,    0,  1,  0,  0,   -1,  0,  0,  0,     0,  0,  1,  0,     0, 0 }, // 10	// 2
			{  0.5f,  0.5f, -0.5f,  1,    0,  1,  0,  0,   -1,  0,  0,  0,     0,  0,  1,  0,     1, 0 }, // 11	// 3
			{ -0.5f, -0.5f,  0.5f,  1,    0, -1,  0,  0,    1,  0,  0,  0,     0,  0,  1,  0,     0, 1 }, // 12	// 4
			{  0.5f, -0.5f,  0.5f,  1,    0, -1,  0,  0,    1,  0,  0,  0,     0,  0,  1,  0,     0, 1 }, // 13	// 5
			{ -0.5f,  0.5f,  0.5f,  1,    0,  1,  0,  0,   -1,  0,  0,  0,     0,  0,  1,  0,     1, 1 }, // 14	// 6
			{  0.5f,  0.5f,  0.5f,  1,    0,  1,  0,  0,   -1,  0,  0,  0,     0,  0,  1,  0,     1, 1 }, // 15	// 7
			// X      Y      Z      W,    nX, nY, nZ, nW,   nX, nY, nZ, nW,    nX, nY, nZ, nW,    UV	  
			{ -0.5f, -0.5f, -0.5f,  1,    0,  0, -1,  0,   -1,  0,  0,  0,     0,  1,  0,  0,     0, 0 }, // 16 // 0
			{  0.5f, -0.5f, -0.5f,  1,    0,  0, -1,  0,   -1,  0,  0,  0,     0,  1,  0,  0,     0, 0 }, // 17	// 1
			{ -0.5f,  0.5f, -0.5f,  1,    0,  0, -1,  0,   -1,  0,  0,  0,     0,  1,  0,  0,     1, 0 }, // 18	// 2
			{  0.5f,  0.5f, -0.5f,  1,    0,  0, -1,  0,   -1,  0,  0,  0,     0,  1,  0,  0,     1, 0 }, // 19	// 3
			{ -0.5f, -0.5f,  0.5f,  1,    0,  0,  1,  0,    1,  0,  0,  0,     0,  1,  0,  0,     0, 1 }, // 20	// 4
			{  0.5f, -0.5f,  0.5f,  1,    0,  0,  1,  0,    1,  0,  0,  0,     0,  1,  0,  0,     1, 1 }, // 21	// 5
			{ -0.5f,  0.5f,  0.5f,  1,    0,  0,  1,  0,    1,  0,  0,  0,     0,  1,  0,  0,     0, 1 }, // 22	// 6
			{  0.5f,  0.5f,  0.5f,  1,    0,  0,  1,  0,    1,  0,  0,  0,     0,  1,  0,  0,     1, 1 }  // 23	// 7
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

		SStaticMeshAsset asset;
		asset.AssetType = EAssetType::StaticMesh;
		asset.Name = "PrimitiveCube";
		asset.NameLength = static_cast<U32>(asset.Name.length());
		asset.NumberOfVertices = 24;
		asset.Vertices.assign(vertices, vertices + 24);
		asset.NumberOfIndices = 36;
		asset.Indices.assign(indices, indices + 36);

		char* data = new char[asset.GetSize()];

		switch (assetType)
		{
		case EAssetType::StaticMesh:
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
		default: ;
		}
	}

	void CRenderManager::LoadStaticMesh(const std::string& fileName, SStaticMeshComponent* /*outStaticMeshComponent*/)
	{
		const U64 fileSize = CEngine::GetInstance()->GetFileSystem()->GetFileSize(fileName);
		char* data = new char[fileSize];

		CEngine::GetInstance()->GetFileSystem()->DeSerialize(fileName, data, static_cast<U32>(fileSize));

		SStaticMeshAsset asset;
		asset.Deserialize(data);

		/*std::string vsData;
		UGraphicsUtils::CreateVertexShader("Shaders/Demo_VS.cso", Framework, &DemoVertexShader, vsData);
		UGraphicsUtils::CreatePixelShader("Shaders/Demo_PS.cso", Framework, &DemoPixelShader);
		VertexShaders.emplace_back(DemoVertexShader);
		PixelShaders.emplace_back(DemoPixelShader);

		D3D11_SAMPLER_DESC samplerDesc = {};
		samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
		samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.MinLOD = 0;
		samplerDesc.MaxLOD = 10;
		ENGINE_HR_MESSAGE(Framework->GetDevice()->CreateSamplerState(&samplerDesc, &SamplerState), "Sampler could not be created.");
		Samplers.emplace_back(SamplerState);

		

		D3D11_BUFFER_DESC demoVertexBufferDesc = { 0 };
		demoVertexBufferDesc.ByteWidth = sizeof(vertices);
		demoVertexBufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
		demoVertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

		D3D11_SUBRESOURCE_DATA demoSubVertexResourceData = { 0 };
		demoSubVertexResourceData.pSysMem = vertices;

		ENGINE_HR_MESSAGE(Framework->GetDevice()->CreateBuffer(&demoVertexBufferDesc, &demoSubVertexResourceData, &DemoVertexBuffer), "Demo Vertex Buffer could not be created.");
		VertexBuffers.emplace_back(DemoVertexBuffer);

		D3D11_BUFFER_DESC demoIndexBufferDesc = { 0 };
		demoIndexBufferDesc.ByteWidth = sizeof(indices);
		demoIndexBufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
		demoIndexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;

		D3D11_SUBRESOURCE_DATA demoIndexSubresourceData = { 0 };
		demoIndexSubresourceData.pSysMem = indices;

		ENGINE_HR_MESSAGE(Framework->GetDevice()->CreateBuffer(&demoIndexBufferDesc, &demoIndexSubresourceData, &DemoIndexBuffer), "Demo Index Buffer could not be created.");
		IndexBuffers.emplace_back(DemoIndexBuffer);

		DemoNumberOfVertices = static_cast<U32>(sizeof(vertices) / sizeof(SStaticMeshVertex));
		DemoNumberOfIndices = static_cast<U32>(sizeof(indices) / sizeof(U32));
		DemoStride = sizeof(SStaticMeshVertex);
		DemoOffset = 0;
		MeshVertexStrides.emplace_back(DemoStride);
		MeshVertexOffsets.emplace_back(DemoOffset);

		D3D11_INPUT_ELEMENT_DESC demoLayout[] =
		{
			{"POSITION"	,	0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
			{"NORMAL"   ,   0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
			{"TANGENT"  ,   0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
			{"BINORMAL" ,   0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
			{"UV"		,   0, DXGI_FORMAT_R32G32_FLOAT,	   0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0}
		};
		ENGINE_HR_MESSAGE(Framework->GetDevice()->CreateInputLayout(demoLayout, sizeof(demoLayout) / sizeof(D3D11_INPUT_ELEMENT_DESC), vsData.data(), vsData.size(), &DemoInputLayout), "Demo Input Layout could not be created.");
		InputLayouts.emplace_back(DemoInputLayout);

		DemoTopology = D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
		Topologies.emplace_back(DemoTopology);*/
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

	bool SRenderCommandComparer::operator()(const SRenderCommand& a, const SRenderCommand& b)
	{
		return 	static_cast<U16>(a.Type) > static_cast<U16>(b.Type);
	}
}
