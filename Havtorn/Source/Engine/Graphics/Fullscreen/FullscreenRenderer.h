#pragma once
#include "hvpch.h"
#include <array>
//#include "EngineException.h"
//#include <d3d11.h>
//#include <SimpleMath.h>

struct ID3D11DeviceContext;
struct ID3D11VertexShader;
struct ID3D11PixelShader;
struct ID3D11Buffer;

//namespace DirectX
//{
//	namespace SimpleMath
//	{
//		struct Vector2;
//		struct Vector4;
//	}
//}

namespace Havtorn
{
	class CDirectXFramework;
	class CRenderManager;

	class CFullscreenRenderer {
	public:
		struct SPostProcessingBufferData
		{
			SVector4 myWhitePointColor = SVector4::Zero;
			F32 myWhitePointIntensity = 1.0f;
			F32 myExposure = 1.0f;
			F32 mySSAORadius = 0.25f;
			F32 mySSAOSampleBias = 0.2f;
			F32 mySSAOMagnitude = 0.2f;
			F32 mySSAOContrast = 1.0f;

			int myIsReinhard = 0;
			int myIsUncharted = 1;
			int myIsACES = 1;

			F32 myEmissiveStrength = 20.0f;

			F32 myVignetteStrength = 0.25f;
			F32 myPadding = FLT_MAX;

			SVector4 myVignetteColor = SVector4::Zero;
		};

		static_assert((sizeof(SPostProcessingBufferData) % 16) == 0, "CB size not padded correctly");

	public:
		enum class FullscreenShader 
		{
			Multiply,
			Copy,
			CopyDepth,
			CopyGBuffer,
			Luminance,
			GaussianHorizontal,
			GaussianVertical,
			BilateralHorizontal,
			BilateralVertical,
			Bloom,
			Vignette,
			Tonemap,
			GammaCorrection,
			GammaCorrectionRenderPass,
			FXAA,
			SSAO,
			SSAOBlur,
			BrokenScreenEffect,
			DownsampleDepth,
			DepthAwareUpsampling,
			DeferredAlbedo,
			DeferredNormals,
			DeferredRoughness,
			DeferredMetalness,
			DeferredAmbientOcclusion,
			DeferredEmissive,
			Count
		};

		friend CRenderManager;

	private:
		static const unsigned int myKernelSize = 16;

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

		struct SFullscreenData 
		{
			SVector2<F32> myResolution;
			SVector2<F32> myNoiseScale;
			SVector4 mySampleKernel[myKernelSize];
		} myFullscreenData;

		static_assert((sizeof(SFullscreenData) % 16) == 0, "CB size not padded correctly");

		struct SFrameBufferData
		{
			SMatrix myToCameraSpace;
			SMatrix myToWorldFromCamera;
			SMatrix myToProjectionSpace;
			SMatrix myToCameraFromProjection;
			SVector4 myCameraPosition;
		} myFrameBufferData;

		static_assert((sizeof(SFrameBufferData) % 16) == 0, "CB size not padded correctly");

	private:
		CFullscreenRenderer();
		~CFullscreenRenderer();
		bool Init(CDirectXFramework* aFramework);
		void Render(FullscreenShader aEffect);

		SPostProcessingBufferData myPostProcessingBufferData;

		ID3D11DeviceContext* myContext;
		ID3D11Buffer* myFullscreenDataBuffer;
		ID3D11Buffer* myFrameBuffer;
		ID3D11Buffer* myPostProcessingBuffer;
		ID3D11VertexShader* myVertexShader;
		ID3D11SamplerState* myClampSampler;
		ID3D11SamplerState* myWrapSampler;

		std::array<ID3D11PixelShader*, static_cast<size_t>(FullscreenShader::Count)> myPixelShaders;

		ID3D11ShaderResourceView* myNoiseTexture;
		SVector4 myKernel[myKernelSize];
	};
}
