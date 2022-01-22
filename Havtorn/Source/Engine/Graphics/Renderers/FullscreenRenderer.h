// Copyright 2022 Team Havtorn. All Rights Reserved.

#pragma once
#include "hvpch.h"
#include <array>

struct ID3D11DeviceContext;
struct ID3D11VertexShader;
struct ID3D11PixelShader;
struct ID3D11Buffer;

namespace Havtorn
{
	class CDirectXFramework;
	class CRenderManager;

	class CFullscreenRenderer 
	{
	public:
		struct SPostProcessingBufferData
		{
			SVector4 WhitePointColor = SVector4::Zero;
			F32 WhitePointIntensity = 1.0f;
			F32 Exposure = 1.0f;
			F32 SSAORadius = 0.25f;
			F32 SSAOSampleBias = 0.2f;
			F32 SSAOMagnitude = 0.2f;
			F32 SSAOContrast = 1.0f;

			I32 IsReinhard = 0;
			I32 IsUncharted = 1;
			I32 IsACES = 1;

			F32 EmissiveStrength = 20.0f;

			F32 VignetteStrength = 0.25f;
			F32 Padding = FLT_MAX;

			SVector4 VignetteColor = SVector4::Zero;
		};
		HV_ASSERT_BUFFER(SPostProcessingBufferData)

	public:
		enum class EFullscreenShader 
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
			//GammaCorrectionRenderPass,
			FXAA,
			SSAO,
			SSAOBlur,
			BrokenScreenEffect,
			DownsampleDepth,
			DepthAwareUpsampling,
			//DeferredAlbedo,
			//DeferredNormals,
			//DeferredRoughness,
			//DeferredMetalness,
			//DeferredAmbientOcclusion,
			//DeferredEmissive,
			Count
		};

		friend CRenderManager;

	private:
		static const U16 KernelSize = 16;

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

		struct SFullscreenData 
		{
			SVector2<F32> Resolution;
			SVector2<F32> NoiseScale;
			SVector4 SampleKernel[KernelSize];
		} FullscreenData;
		HV_ASSERT_BUFFER(SFullscreenData)

		struct SFrameBufferData
		{
			SMatrix ToCameraSpace;
			SMatrix ToWorldFromCamera;
			SMatrix ToProjectionSpace;
			SMatrix ToCameraFromProjection;
			SVector4 CameraPosition;
		} FrameBufferData;
		HV_ASSERT_BUFFER(SFrameBufferData)

	private:
		CFullscreenRenderer();
		~CFullscreenRenderer();
		bool Init(CDirectXFramework* framework);
		void Render(EFullscreenShader effect);

		SPostProcessingBufferData PostProcessingBufferData;

		ID3D11DeviceContext* Context;
		ID3D11Buffer* FullscreenDataBuffer;
		ID3D11Buffer* FrameBuffer;
		ID3D11Buffer* PostProcessingBuffer;
		ID3D11VertexShader* VertexShader;
		ID3D11SamplerState* ClampSampler;
		ID3D11SamplerState* WrapSampler;

		std::array<ID3D11PixelShader*, static_cast<size_t>(EFullscreenShader::Count)> PixelShaders;

		ID3D11ShaderResourceView* NoiseTexture;
		SVector4 Kernel[KernelSize];
	};
}
