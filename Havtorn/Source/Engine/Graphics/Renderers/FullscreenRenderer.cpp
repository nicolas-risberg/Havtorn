// Copyright 2022 Team Havtorn. All Rights Reserved.

#include "hvpch.h"
#include "FullscreenRenderer.h"
#include "Engine.h"
#include "Graphics/GraphicsFramework.h"
#include "Graphics/RenderManager.h"
#include "Graphics/GraphicsUtilities.h"
//#include "Scene.h"
//#include "CameraComponent.h"
//#include <fstream>

namespace Havtorn
{
	CFullscreenRenderer::CFullscreenRenderer()
		: Context(nullptr)
		, VertexShader(nullptr)
		, PixelShaders()
		, PostProcessingBufferData()
		, ClampSampler(nullptr)
		, WrapSampler(nullptr)
		, FullscreenDataBuffer(nullptr)
		, FrameBuffer(nullptr)
		, PostProcessingBuffer(nullptr)
		, NoiseTexture(nullptr)
	{
	}

	CFullscreenRenderer::~CFullscreenRenderer() 
	{}

	bool CFullscreenRenderer::Init(CDirectXFramework* framework) 
	{
		if (!framework) 
			return false;

		Context = framework->GetContext();
		if (!Context)
			return false;

		ID3D11Device* device = framework->GetDevice();

		D3D11_BUFFER_DESC bufferDescription = { 0 };
		bufferDescription.Usage = D3D11_USAGE_DYNAMIC;
		bufferDescription.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		bufferDescription.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

		bufferDescription.ByteWidth = sizeof(SFullscreenData);
		ENGINE_HR_BOOL_MESSAGE(device->CreateBuffer(&bufferDescription, nullptr, &FullscreenDataBuffer), "Fullscreen Data Buffer could not be created.");

		bufferDescription.ByteWidth = sizeof(SFrameBufferData);
		ENGINE_HR_BOOL_MESSAGE(device->CreateBuffer(&bufferDescription, nullptr, &FrameBuffer), "Frame Buffer could not be created.");

		bufferDescription.ByteWidth = sizeof(SPostProcessingBufferData);
		ENGINE_HR_BOOL_MESSAGE(device->CreateBuffer(&bufferDescription, nullptr, &PostProcessingBuffer), "Post Processing Buffer could not be created.");

		std::string vsData;
		ENGINE_ERROR_BOOL_MESSAGE(UGraphicsUtils::CreateVertexShader("Shaders/FullscreenVertexShader_VS.cso", framework, &VertexShader, vsData), "Could not create Vertex Shader from FullscreenVertexShader_VS.cso");

		std::array<std::string, static_cast<size_t>(EFullscreenShader::Count)> filepaths;
		filepaths[static_cast<size_t>(EFullscreenShader::Multiply)] = "Shaders/FullscreenMultiply_PS.cso";
		filepaths[static_cast<size_t>(EFullscreenShader::Copy)] = "Shaders/FullscreenCopy_PS.cso";
		filepaths[static_cast<size_t>(EFullscreenShader::CopyDepth)] = "Shaders/FullscreenCopyDepth_PS.cso";
		filepaths[static_cast<size_t>(EFullscreenShader::CopyGBuffer)] = "Shaders/FullscreenCopyGBuffer_PS.cso";
		filepaths[static_cast<size_t>(EFullscreenShader::Luminance)] = "Shaders/FullscreenLuminance_PS.cso";
		filepaths[static_cast<size_t>(EFullscreenShader::GaussianHorizontal)] = "Shaders/FullscreenGaussianBlurHorizontal_PS.cso";
		filepaths[static_cast<size_t>(EFullscreenShader::GaussianVertical)] = "Shaders/FullscreenGaussianBlurVertical_PS.cso";
		filepaths[static_cast<size_t>(EFullscreenShader::BilateralHorizontal)] = "Shaders/FullscreenBilateralBlurHorizontal_PS.cso";
		filepaths[static_cast<size_t>(EFullscreenShader::BilateralVertical)] = "Shaders/FullscreenBilateralBlurVertical_PS.cso";
		filepaths[static_cast<size_t>(EFullscreenShader::Bloom)] = "Shaders/FullscreenBloom_PS.cso";
		filepaths[static_cast<size_t>(EFullscreenShader::Vignette)] = "Shaders/FullscreenVignette_PS.cso";
		filepaths[static_cast<size_t>(EFullscreenShader::Tonemap)] = "Shaders/FullscreenTonemap_PS.cso";
		filepaths[static_cast<size_t>(EFullscreenShader::GammaCorrection)] = "Shaders/FullscreenGammaCorrection_PS.cso";
		//filepaths[static_cast<size_t>(EFullscreenShader::GammaCorrectionRenderPass)] = "Shaders/DeferredRenderPassFullscreenPixelShader_GammaCorrection.cso";
		filepaths[static_cast<size_t>(EFullscreenShader::FXAA)] = "Shaders/FullscreenFXAA_PS.cso";
		filepaths[static_cast<size_t>(EFullscreenShader::SSAO)] = "Shaders/FullscreenSSAO_PS.cso";
		filepaths[static_cast<size_t>(EFullscreenShader::SSAOBlur)] = "Shaders/FullscreenSSAOBlur_PS.cso";
		filepaths[static_cast<size_t>(EFullscreenShader::BrokenScreenEffect)] = "Shaders/FullscreenBrokenScreenEffect_PS.cso";
		filepaths[static_cast<size_t>(EFullscreenShader::DownsampleDepth)] = "Shaders/FullscreenDepthDownSample_PS.cso";
		filepaths[static_cast<size_t>(EFullscreenShader::DepthAwareUpsampling)] = "Shaders/FullscreenDepthAwareUpsample_PS.cso";
		//filepaths[static_cast<size_t>(EFullscreenShader::DeferredAlbedo)] = "Shaders/DeferredRenderPassShader_Albedo.cso";
		//filepaths[static_cast<size_t>(EFullscreenShader::DeferredNormals)] = "Shaders/DeferredRenderPassShader_Normal.cso";
		//filepaths[static_cast<size_t>(EFullscreenShader::DeferredRoughness)] = "Shaders/DeferredRenderPassShader_Roughness.cso";
		//filepaths[static_cast<size_t>(EFullscreenShader::DeferredMetalness)] = "Shaders/DeferredRenderPassShader_Metalness.cso";
		//filepaths[static_cast<size_t>(EFullscreenShader::DeferredAmbientOcclusion)] = "Shaders/DeferredRenderPassShader_AO.cso";
		//filepaths[static_cast<size_t>(EFullscreenShader::DeferredEmissive)] = "Shaders/DeferredRenderPassShader_Emissive.cso";

		for (UINT i = 0; i < static_cast<size_t>(EFullscreenShader::Count); i++) 
		{
			ENGINE_ERROR_BOOL_MESSAGE(UGraphicsUtils::CreatePixelShader(filepaths[i], framework, &PixelShaders[i]), "Could not create Pixel Shader from %s", filepaths[i].c_str());
		}

		//Start Samplers
		D3D11_SAMPLER_DESC samplerDesc = {};
		samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
		samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
		samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
		samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
		samplerDesc.MinLOD = 0;
		samplerDesc.MaxLOD = 10;
		ENGINE_HR_MESSAGE(device->CreateSamplerState(&samplerDesc, &ClampSampler), "Sampler could not be created.");

		samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
		ENGINE_HR_MESSAGE(device->CreateSamplerState(&samplerDesc, &WrapSampler), "Sampler could not be created.");
		//End Samplers

#pragma region SSAO Setup
	//for (unsigned int i = 0; i < KernelSize; ++i) 
	//{
	//	float r = 1.0f * sqrt(UMath::Random(0.0f, 1.0f));
	//	float theta = UMath::Random(F32(i) / F32(KernelSize), F32(i+1) / F32(KernelSize)) * UMath::Tau;
	//	float x = r * cosf(theta);
	//	float y = r * sinf(theta);
	//	float z = sqrt(1 - x * x - y * y);
	//	Kernel[i] = SVector4(
	//		x,
	//		y,
	//		z,
	//		1.0f);
	//		//Kernel[i].Normalize();
	//		Kernel[i] *= UMath::Random(0.0f, 1.0f);
	//		//F32 scale = F32(i) / F32(KernelSize);
	//		//scale = UMath::Lerp(0.1f, 1.0f, scale * scale);
	//		//Kernel[i] *= scale;
	//}

	// Hardcoded Kernel
		Kernel[0] = { 0.528985322f, 0.163332120f, 0.620016515f, 1.0f };
		Kernel[1] = { 0.573982120f, 0.378577918f, 0.470547318f, 1.0f };
		Kernel[2] = { 0.065050237f, 0.139410198f, 0.347815633f, 1.0f };
		Kernel[3] = { 0.041187014f, 0.130081877f, 0.164059237f, 1.0f };
		Kernel[4] = { -0.026605275f, 0.090929292f, 0.077286638f, 1.0f };
		Kernel[5] = { -0.113886870f, 0.154690191f, 0.197556734f, 1.0f };
		Kernel[6] = { -0.666800976f, 0.662895739f, 0.277599692f, 1.0f };
		Kernel[7] = { -0.399470448f, 0.096369371f, 0.417604893f, 1.0f };
		Kernel[8] = { -0.411310822f, -0.082451604f, 0.179119825f, 1.0f };
		Kernel[9] = { -0.117983297f, -0.095347963f, 0.374402136f, 1.0f };
		Kernel[10] = { -0.457335383f, -0.529036164f, 0.490310162f, 1.0f };
		Kernel[11] = { -0.119527563f, -0.291437626f, 0.206827655f, 1.0f };
		Kernel[12] = { 0.201868936f, -0.513456404f, 0.432056010f, 1.0f };
		Kernel[13] = { 0.096077450f, -0.107414119f, 0.527342558f, 1.0f };
		Kernel[14] = { 0.223280489f, -0.180109233f, 0.203371927f, 1.0f };
		Kernel[15] = { 0.163490131f, -0.039255358f, 0.532910645f, 1.0f };

		SVector4 noise[KernelSize];
		for (U16 i = 0; i < KernelSize; ++i)
		{
			noise[i] = SVector4(
				UMath::Random(-1.0f, 1.0f),
				UMath::Random(-1.0f, 1.0f),
				0.0f,
				0.0f
			);
			noise[i].Normalize();
		}

		U16 width = UMath::Sqrt(KernelSize);

		D3D11_TEXTURE2D_DESC noiseTextureDesc = { 0 };
		noiseTextureDesc.Width = width;
		noiseTextureDesc.Height = width;
		noiseTextureDesc.MipLevels = 1;
		noiseTextureDesc.ArraySize = 1;
		noiseTextureDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
		noiseTextureDesc.SampleDesc.Count = 1;
		noiseTextureDesc.SampleDesc.Quality = 0;
		noiseTextureDesc.Usage = D3D11_USAGE_DEFAULT;
		noiseTextureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
		noiseTextureDesc.CPUAccessFlags = 0;
		noiseTextureDesc.MiscFlags = 0;

		D3D11_SUBRESOURCE_DATA noiseTextureData = { 0 };
		noiseTextureData.pSysMem = &noise[0];
		noiseTextureData.SysMemPitch = width * sizeof(SVector4);
		noiseTextureData.SysMemSlicePitch = 0;

		D3D11_SHADER_RESOURCE_VIEW_DESC noiseSRVDesc;
		noiseSRVDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
		noiseSRVDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		noiseSRVDesc.Texture2D.MostDetailedMip = 0;
		noiseSRVDesc.Texture2D.MipLevels = 1;

		ID3D11Texture2D* noiseTextureBuffer;
		ENGINE_HR_MESSAGE(device->CreateTexture2D(&noiseTextureDesc, &noiseTextureData, &noiseTextureBuffer), "Noise Texture could not be created.");
		ENGINE_HR_MESSAGE(device->CreateShaderResourceView(noiseTextureBuffer, &noiseSRVDesc, &NoiseTexture), "Noise Shader Resource View could not be created.");
#pragma endregion

		PostProcessingBufferData.WhitePointColor = { 255.0f / 255.0f, 255.0f / 255.0f, 255.0f / 255.0f, 1.0f };
		PostProcessingBufferData.WhitePointIntensity = 1.0f;
		PostProcessingBufferData.Exposure = 1.1f;
		PostProcessingBufferData.IsReinhard = false;
		PostProcessingBufferData.IsUncharted = false;
		PostProcessingBufferData.IsACES = true;

		PostProcessingBufferData.SSAORadius = 0.6f;
		PostProcessingBufferData.SSAOSampleBias = 0.2420f;
		PostProcessingBufferData.SSAOMagnitude = 1.4f;
		PostProcessingBufferData.SSAOContrast = 0.6f;

		PostProcessingBufferData.EmissiveStrength = 2.1f;

		PostProcessingBufferData.VignetteStrength = 0.35f;
		PostProcessingBufferData.Padding = 0.0f;
		PostProcessingBufferData.VignetteColor = { 0.0f, 0.0f, 0.0f, 1.0f };

		return true;
	}

	void CFullscreenRenderer::Render(EFullscreenShader effect)
	{
		FullscreenData.Resolution = CEngine::GetInstance()->GetWindowHandler()->GetResolution();
		FullscreenData.NoiseScale = { FullscreenData.Resolution.X / static_cast<F32>(UMath::Sqrt(KernelSize)), FullscreenData.Resolution.Y / static_cast<F32>(UMath::Sqrt(KernelSize)) };
		memcpy(&FullscreenData.SampleKernel[0], &Kernel[0], sizeof(Kernel));
		BindBuffer(FullscreenDataBuffer, FullscreenData, "Fullscreen Data Buffer");

		//CCameraComponent* camera = IRONWROUGHT->GetActiveScene().MainCamera();
		//SM::Matrix& cameraMatrix = camera->GameObject().myTransform->Transform();
		//FrameBufferData.CameraPosition = SM::Vector4{ cameraMatrix._41, cameraMatrix._42, cameraMatrix._43, 1.f };
		//FrameBufferData.ToCameraSpace = cameraMatrix.Invert();
		//FrameBufferData.ToWorldFromCamera = cameraMatrix;
		//FrameBufferData.ToProjectionSpace = camera->GetProjection();
		//FrameBufferData.ToCameraFromProjection = camera->GetProjection().Invert();
		//BindBuffer(FrameBuffer, FrameBufferData, "Frame Buffer");

		BindBuffer(PostProcessingBuffer, PostProcessingBufferData, "Post Processing Buffer");

		Context->VSSetConstantBuffers(0, 1, &FrameBuffer);

		Context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		Context->IASetInputLayout(nullptr);
		Context->IASetVertexBuffers(0, 0, nullptr, nullptr, nullptr);
		Context->IASetIndexBuffer(nullptr, DXGI_FORMAT_UNKNOWN, 0);

		Context->VSSetShader(VertexShader, nullptr, 0);
		Context->PSSetShader(PixelShaders[static_cast<size_t>(effect)], nullptr, 0);
		Context->PSSetSamplers(0, 1, &ClampSampler);
		Context->PSSetSamplers(1, 1, &WrapSampler);
		Context->PSSetConstantBuffers(0, 1, &FullscreenDataBuffer);
		Context->PSSetConstantBuffers(1, 1, &FrameBuffer);
		Context->PSSetConstantBuffers(2, 1, &PostProcessingBuffer);
		Context->PSSetShaderResources(23, 1, &NoiseTexture);

		Context->Draw(3, 0);
		CRenderManager::NumberOfDrawCallsThisFrame++;

		ID3D11ShaderResourceView* nullView = NULL;
		Context->PSSetShaderResources(0, 1, &nullView);
		Context->PSSetShaderResources(1, 1, &nullView);
		Context->PSSetShaderResources(2, 1, &nullView);
		Context->PSSetShaderResources(3, 1, &nullView);
		Context->PSSetShaderResources(4, 1, &nullView);
		Context->PSSetShaderResources(5, 1, &nullView);
		Context->PSSetShaderResources(6, 1, &nullView);
		Context->PSSetShaderResources(7, 1, &nullView);
		Context->PSSetShaderResources(8, 1, &nullView);
		Context->PSSetShaderResources(9, 1, &nullView);
		Context->PSSetShaderResources(21, 1, &nullView);
		Context->PSSetShaderResources(22, 1, &nullView);

		Context->GSSetShader(nullptr, nullptr, 0);
	}
}
