// Copyright 2022 Team Havtorn. All Rights Reserved.

#pragma once
#include "hvpch.h"
//#include "ForwardRenderer.h"
//#include "DeferredRenderer.h"
//#include "LightRenderer.h"
#include "GraphicsFramework.h"
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
#include "GBuffer.h"
#include <queue>

#include "Core/RuntimeAssetDeclarations.h"

namespace Havtorn
{
	enum class EShaderType
	{
		Vertex,
		Compute,
		Geometry,
		Pixel
	};

	enum class ESamplerType
	{
		Border,
		Clamp,
		Mirror,
		Wrap
	};

	enum class EMaterialConfiguration
	{
		AlbedoMaterialNormal_Packed
	};

	enum class EInputLayoutType
	{
		Pos3Nor3Tan3Bit3UV2
	};

	class CGraphicsFramework;
	class CWindowHandler;
	struct SRenderCommand;
	struct SStaticMeshComponent;
	struct SMaterialComponent;

	struct SRenderCommandComparer
	{
		bool operator()(const SRenderCommand& a, const SRenderCommand& b) const;
	};

	using CRenderCommandHeap = std::priority_queue<SRenderCommand, std::vector<SRenderCommand>, SRenderCommandComparer>;

	class CRenderManager
	{
	public:
		CRenderManager();
		~CRenderManager();
		bool Init(CGraphicsFramework* framework, CWindowHandler* windowHandler);
		bool ReInit(CGraphicsFramework* framework, CWindowHandler* windowHandler);
		void Render();

		void Release();

		void WriteAssetFile(const std::string& fileName, EAssetType assetType);
		void LoadStaticMeshComponent(const std::string& fileName, SStaticMeshComponent* outStaticMeshComponent);
		void LoadMaterialComponent(const std::vector<std::string>& materialNames, SMaterialComponent* outMaterialComponent);

	public:
		[[nodiscard]] const CFullscreenTexture& GetRenderedSceneTexture() const;
		void PushRenderCommand(SRenderCommand& command);
		void SwapRenderCommandBuffers();
		//void SetBrokenScreen(bool aShouldSetBrokenScreen);

		//const CFullscreenRenderer::SPostProcessingBufferData& GetPostProcessingBufferData() const;
		//void SetPostProcessingBufferData(const CFullscreenRenderer::SPostProcessingBufferData& someBufferData);

	public:
		static U32 NumberOfDrawCallsThisFrame;

	private:
		void Clear(SVector4 clearColor);
		void InitRenderTextures(CWindowHandler* windowHandler);
		void InitShadowmapAtlas(SVector2<F32> atlasResolution);
		void InitShadowmapLOD(SVector2<F32> topLeftCoordinate, const SVector2<F32>& widthAndHeight, const SVector2<F32>& depth, U16 mapsInLod, U16 startIndex);
		void LoadDemoSceneResources();

	private:
		void RenderBloom();
		void RenderWithoutBloom();
		void ToggleRenderPass(bool shouldToggleForwards = true);

	private:
		template<typename T>
		U16 AddVertexBuffer(const std::vector<T>& vertices);
		U16 AddIndexBuffer(const std::vector<U32>& indices);
		U16 AddMeshVertexStride(U32 stride);
		U16 AddMeshVertexOffset(U32 offset);

		std::string AddShader(const std::string& fileName, EShaderType shaderType);
		void AddInputLayout(const std::string& vsData, EInputLayoutType layoutType);
		void AddSampler(ESamplerType samplerType);
		void AddTopology(D3D11_PRIMITIVE_TOPOLOGY topology);
		void AddViewport(SVector2<F32> topLeftCoordinate, SVector2<F32> widthAndHeight, SVector2<F32> depth);

		std::vector<U16> AddMaterial(const std::string& materialName, EMaterialConfiguration configuration);

	private:
		template<class T>
		void BindBuffer(ID3D11Buffer* buffer, T& bufferData, std::string bufferType)
		{
			D3D11_MAPPED_SUBRESOURCE localBufferData;
			ZeroMemory(&localBufferData, sizeof(D3D11_MAPPED_SUBRESOURCE));
			const std::string errorMessage = bufferType + " could not be bound.";
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

		struct SDirectionalLightBufferData
		{
			SMatrix ToDirectionalLightView;
			SMatrix ToDirectionalLightProjection;
			SVector4 DirectionalLightPosition;
			SVector4 DirectionalLightDirection;
			SVector4 DirectionalLightColor;
			SVector2<F32> DirectionalLightShadowMapResolution;
			SVector2<F32> Padding;
		} DirectionalLightBufferData;
		static_assert((sizeof(SDirectionalLightBufferData) % 16) == 0, "CB size not padded correctly");
		HV_ASSERT_BUFFER(SDirectionalLightBufferData)

	private:
		CGraphicsFramework* Framework;
		ID3D11DeviceContext* Context;
		ID3D11Buffer* FrameBuffer;
		ID3D11Buffer* ObjectBuffer;
		ID3D11Buffer* LightBuffer;
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
		CFullscreenTexture IntermediateTexture;
		CFullscreenTexture IntermediateDepth;
		CFullscreenTexture ShadowAtlasDepth;
		//CFullscreenTexture myBoxLightShadowDepth;
		CFullscreenTexture DepthCopy;
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
		CFullscreenTexture SSAOBuffer;
		CFullscreenTexture SSAOBlurTexture;
		//CFullscreenTexture myDownsampledDepth;
		CFullscreenTexture TonemappedTexture;
		CFullscreenTexture AntiAliasedTexture;
		CGBuffer GBuffer;
		//CGBuffer myGBufferCopy;
		CRenderCommandHeap RenderCommandsA;
		CRenderCommandHeap RenderCommandsB;

		CRenderCommandHeap* PushToCommands;
		CRenderCommandHeap* PopFromCommands;

		SVector4 ClearColor;

		I8 RenderPassIndex;
		// Effectively used to toggle renderpasses and bloom. True == enable bloom, full render. False == disable bloom, isolated render pass
		bool DoFullRender;
		bool UseAntiAliasing;
		bool UseBrokenScreenPass;

		std::vector<std::string> MaterialNames;
		std::vector<ID3D11ShaderResourceView*> Textures;
		std::vector<ID3D11VertexShader*> VertexShaders;
		std::vector<ID3D11PixelShader*> PixelShaders;
		std::vector<ID3D11SamplerState*> Samplers;
		std::vector<ID3D11Buffer*> VertexBuffers;
		std::vector<ID3D11Buffer*> IndexBuffers;
		std::vector<ID3D11InputLayout*> InputLayouts;
		std::vector<D3D11_PRIMITIVE_TOPOLOGY> Topologies;
		std::vector<D3D11_VIEWPORT> Viewports;
		std::vector<U32> MeshVertexStrides;
		std::vector<U32> MeshVertexOffsets;

		std::unordered_map<std::string, SStaticMeshAsset> LoadedStaticMeshes;

		ID3D11ShaderResourceView* DefaultAlbedoTexture = nullptr;
		ID3D11ShaderResourceView* DefaultNormalTexture = nullptr;
		ID3D11ShaderResourceView* DefaultMaterialTexture = nullptr;

		ID3D11ShaderResourceView* DefaultCubemap = nullptr;
		
		EMaterialConfiguration MaterialConfiguration = EMaterialConfiguration::AlbedoMaterialNormal_Packed;
		U8 TexturesPerMaterial = 3;
	};

	template <typename T>
	U16 CRenderManager::AddVertexBuffer(const std::vector<T>& vertices)
	{
		D3D11_BUFFER_DESC vertexBufferDesc = { 0 };
		vertexBufferDesc.ByteWidth = sizeof(T) * static_cast<U32>(vertices.size());
		vertexBufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
		vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

		D3D11_SUBRESOURCE_DATA subVertexResourceData = { nullptr };
		subVertexResourceData.pSysMem = vertices.data();

		ID3D11Buffer* vertexBuffer;
		ENGINE_HR_MESSAGE(Framework->GetDevice()->CreateBuffer(&vertexBufferDesc, &subVertexResourceData, &vertexBuffer), "Vertex Buffer could not be created.");
		VertexBuffers.emplace_back(vertexBuffer);

		return static_cast<U16>(VertexBuffers.size() - 1);
	}
}
