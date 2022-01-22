// Copyright 2022 Team Havtorn. All Rights Reserved.

#pragma once
#include <vector>

struct ID3D11DeviceContext;
struct ID3D11Buffer;

namespace Havtorn
{
	class CDirectXFramework;
	class CModel;
	class CEnvironmentLight;
	class CPointLight;
	class CGameObject;
	class CCameraComponent;
	class CMaterialHandler;
	
	class CDeferredRenderer
	{
	public:
		CDeferredRenderer();
		~CDeferredRenderer();

		bool Init(CDirectXFramework* framework, CMaterialHandler* materialHandler);

		void GenerateGBuffer(CCameraComponent* aCamera, std::vector<CGameObject*>& aGameObjectList, std::vector<CGameObject*>& aInstancedGameObjectList);
		void RenderSkybox(CCameraComponent* aCamera, CEnvironmentLight* anEnvironmentLight);

		bool ToggleRenderPass();

	private:
		void SetShaderResource(const int aMaterialID);

	private:
		template<class T>
		void BindBuffer(ID3D11Buffer* aBuffer, T& someBufferData, std::string aBufferType)
		{
			D3D11_MAPPED_SUBRESOURCE bufferData;
			ZeroMemory(&bufferData, sizeof(D3D11_MAPPED_SUBRESOURCE));
			std::string errorMessage = aBufferType + " could not be bound.";
			ENGINE_HR_MESSAGE(myContext->Map(aBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &bufferData), errorMessage.c_str());

			memcpy(bufferData.pData, &someBufferData, sizeof(T));
			myContext->Unmap(aBuffer, 0);
		}

	private:
		struct SFrameBufferData
		{
			SMatrix myToCameraSpace;
			SMatrix myToWorldFromCamera;
			SMatrix myToProjectionSpace;
			SMatrix myToCameraFromProjection;
			SVector4 myCameraPosition;
		} myFrameBufferData;
		HV_ASSERT_BUFFER(SFrameBufferData)

		struct SObjectBufferData
		{
			SMatrix myToWorld;
			U16 myNumberOfDetailNormals;
			U16 myNumberOfTextureSets;
		} myObjectBufferData;
		//static_assert((sizeof(SObjectBufferData) % 16) == 0, "CB size not padded correctly");
		HV_ASSERT_BUFFER(SObjectBufferData)

		struct SBoneBufferData 
		{
			SMatrix myBones[64];
		} myBoneBufferData;
		HV_ASSERT_BUFFER(SBoneBufferData)

		struct SSkyboxTransformData 
		{
			SMatrix myCameraViewProjection;
		} mySkyboxTransformData;
		HV_ASSERT_BUFFER(SSkyboxTransformData)

	private:
		bool LoadRenderPassPixelShaders(CDirectXFramework* aFramework);

		ID3D11DeviceContext* myContext;
		CMaterialHandler* myMaterialHandler;
		
		// Buffers;
		ID3D11Buffer* myFrameBuffer;
		ID3D11Buffer* myObjectBuffer;
		ID3D11Buffer* myBoneBuffer;
		ID3D11Buffer* mySkyboxTransformBuffer;

		ID3D11InputLayout* myVertexPaintInputLayout;

		// Vertex shaders.
		ID3D11VertexShader* myFullscreenShader;
		ID3D11VertexShader* myModelVertexShader;
		ID3D11VertexShader* myAnimationVertexShader;
		ID3D11VertexShader* myVertexPaintModelVertexShader;
		ID3D11VertexShader* myInstancedModelVertexShader;
		ID3D11VertexShader* mySkyboxVertexShader;

		// Pixel shaders.
		ID3D11PixelShader* myGBufferPixelShader;
		ID3D11PixelShader* myVertexPaintPixelShader;
		ID3D11PixelShader* mySkyboxPixelShader;
		// Samplers.
		ID3D11SamplerState* mySamplerState;
		ID3D11SamplerState* myShadowSampler;

		std::vector<ID3D11PixelShader*> myRenderPassShaders;
		ID3D11PixelShader* myCurrentRenderPassShader;
		// Switches between myRenderPassGBuffer and myGBufferPixelShader
		ID3D11PixelShader* myCurrentGBufferPixelShader;
		ID3D11PixelShader* myRenderPassGBuffer;
		U8 myRenderPassIndex;

		ID3D11Buffer* mySkyboxVertexBuffer;
		ID3D11Buffer* mySkyboxIndexBuffer;
		ID3D11InputLayout* mySkyboxInputLayout;
		U32 mySkyboxNumberOfVertices;
		U32 mySkyboxNumberOfIndices;
		U32 mySkyboxStride;
		U32 mySkyboxOffset;
	};
}
