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
		: myContext(nullptr)
		, myVertexShader(nullptr)
		, myPixelShaders()
		, myPostProcessingBufferData()
		, myClampSampler(nullptr)
		, myWrapSampler(nullptr)
		, myFullscreenDataBuffer(nullptr)
		, myFrameBuffer(nullptr)
		, myPostProcessingBuffer(nullptr)
		, myNoiseTexture(nullptr)
	{
	}

	CFullscreenRenderer::~CFullscreenRenderer() {
	}

	float KernelLerp(float a, float b, float t)
	{
		return (a * (1.0f - t)) + (b * t);
	}

	bool CFullscreenRenderer::Init(CDirectXFramework* aFramework) 
	{
		if (!aFramework) {
			return false;
		}

		myContext = aFramework->GetContext();
		if (!myContext) {
			return false;
		}

		ID3D11Device* device = aFramework->GetDevice();
		//HRESULT result;

		D3D11_BUFFER_DESC bufferDescription = { 0 };
		bufferDescription.Usage = D3D11_USAGE_DYNAMIC;
		bufferDescription.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		bufferDescription.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

		bufferDescription.ByteWidth = sizeof(SFullscreenData);
		ENGINE_HR_BOOL_MESSAGE(device->CreateBuffer(&bufferDescription, nullptr, &myFullscreenDataBuffer), "Fullscreen Data Buffer could not be created.");

		bufferDescription.ByteWidth = sizeof(SFrameBufferData);
		ENGINE_HR_BOOL_MESSAGE(device->CreateBuffer(&bufferDescription, nullptr, &myFrameBuffer), "Frame Buffer could not be created.");

		bufferDescription.ByteWidth = sizeof(SPostProcessingBufferData);
		ENGINE_HR_BOOL_MESSAGE(device->CreateBuffer(&bufferDescription, nullptr, &myPostProcessingBuffer), "Post Processing Buffer could not be created.");

		std::string vsData;
		ENGINE_ERROR_BOOL_MESSAGE(UGraphicsUtils::CreateVertexShader("Shaders/FullscreenVertexShader_VS.cso", aFramework, &myVertexShader, vsData), "Could not create Vertex Shader from FullscreenVertexShader_VS.cso");
		//std::ifstream vsFile;
		//vsFile.open("Shaders/FullscreenVertexShader.cso", std::ios::binary);
		//std::string vsData = { std::istreambuf_iterator<char>(vsFile), std::istreambuf_iterator<char>() };
		//ID3D11VertexShader* vertexShader;
		//result = device->CreateVertexShader(vsData.data(), vsData.size(), nullptr, &vertexShader);
		//if (FAILED(result)) {
		//	return false;
		//}
		//vsFile.close();
		//myVertexShader = vertexShader;

		std::array<std::string, static_cast<size_t>(FullscreenShader::Count)> filepaths;
		filepaths[static_cast<size_t>(FullscreenShader::Multiply)] = "Shaders/FullscreenMultiply_PS.cso";
		filepaths[static_cast<size_t>(FullscreenShader::Copy)] = "Shaders/FullscreenCopy_PS.cso";
		filepaths[static_cast<size_t>(FullscreenShader::CopyDepth)] = "Shaders/FullscreenCopyDepth_PS.cso";
		filepaths[static_cast<size_t>(FullscreenShader::CopyGBuffer)] = "Shaders/FullscreenCopyGBuffer_PS.cso";
		filepaths[static_cast<size_t>(FullscreenShader::Luminance)] = "Shaders/FullscreenLuminance_PS.cso";
		filepaths[static_cast<size_t>(FullscreenShader::GaussianHorizontal)] = "Shaders/FullscreenGaussianBlurHorizontal_PS.cso";
		filepaths[static_cast<size_t>(FullscreenShader::GaussianVertical)] = "Shaders/FullscreenGaussianBlurVertical_PS.cso";
		filepaths[static_cast<size_t>(FullscreenShader::BilateralHorizontal)] = "Shaders/FullscreenBilateralBlurHorizontal_PS.cso";
		filepaths[static_cast<size_t>(FullscreenShader::BilateralVertical)] = "Shaders/FullscreenBilateralBlurVertical_PS.cso";
		filepaths[static_cast<size_t>(FullscreenShader::Bloom)] = "Shaders/FullscreenBloom_PS.cso";
		filepaths[static_cast<size_t>(FullscreenShader::Vignette)] = "Shaders/FullscreenVignette_PS.cso";
		filepaths[static_cast<size_t>(FullscreenShader::Tonemap)] = "Shaders/FullscreenTonemap_PS.cso";
		filepaths[static_cast<size_t>(FullscreenShader::GammaCorrection)] = "Shaders/FullscreenGammaCorrection_PS.cso";
		//filepaths[static_cast<size_t>(FullscreenShader::GammaCorrectionRenderPass)] = "Shaders/DeferredRenderPassFullscreenPixelShader_GammaCorrection.cso";
		filepaths[static_cast<size_t>(FullscreenShader::FXAA)] = "Shaders/FullscreenFXAA_PS.cso";
		filepaths[static_cast<size_t>(FullscreenShader::SSAO)] = "Shaders/FullscreenSSAO_PS.cso";
		filepaths[static_cast<size_t>(FullscreenShader::SSAOBlur)] = "Shaders/FullscreenSSAOBlur_PS.cso";
		filepaths[static_cast<size_t>(FullscreenShader::BrokenScreenEffect)] = "Shaders/FullscreenBrokenScreenEffect_PS.cso";
		filepaths[static_cast<size_t>(FullscreenShader::DownsampleDepth)] = "Shaders/FullscreenDepthDownSample_PS.cso";
		filepaths[static_cast<size_t>(FullscreenShader::DepthAwareUpsampling)] = "Shaders/FullscreenDepthAwareUpsample_PS.cso";
		//filepaths[static_cast<size_t>(FullscreenShader::DeferredAlbedo)] = "Shaders/DeferredRenderPassShader_Albedo.cso";
		//filepaths[static_cast<size_t>(FullscreenShader::DeferredNormals)] = "Shaders/DeferredRenderPassShader_Normal.cso";
		//filepaths[static_cast<size_t>(FullscreenShader::DeferredRoughness)] = "Shaders/DeferredRenderPassShader_Roughness.cso";
		//filepaths[static_cast<size_t>(FullscreenShader::DeferredMetalness)] = "Shaders/DeferredRenderPassShader_Metalness.cso";
		//filepaths[static_cast<size_t>(FullscreenShader::DeferredAmbientOcclusion)] = "Shaders/DeferredRenderPassShader_AO.cso";
		//filepaths[static_cast<size_t>(FullscreenShader::DeferredEmissive)] = "Shaders/DeferredRenderPassShader_Emissive.cso";

		for (UINT i = 0; i < static_cast<size_t>(FullscreenShader::Count); i++) 
		{
			//std::ifstream psFile;
			//psFile.open(filepaths[i], std::ios::binary);
			//std::string psData = { std::istreambuf_iterator<char>(psFile), std::istreambuf_iterator<char>() };
			//ID3D11PixelShader* pixelShader;
			//result = device->CreatePixelShader(psData.data(), psData.size(), nullptr, &pixelShader);
			//if (FAILED(result)) {
			//	return false;
			//}
			//psFile.close();
			//myPixelShaders[i] = pixelShader;
			ENGINE_ERROR_BOOL_MESSAGE(UGraphicsUtils::CreatePixelShader(filepaths[i], aFramework, &myPixelShaders[i]), "Could not create Pixel Shader from %s", filepaths[i].c_str());
		}

		//Start Samplers
		D3D11_SAMPLER_DESC samplerDesc = {};
		samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
		samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
		samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
		samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
		samplerDesc.MinLOD = 0;
		samplerDesc.MaxLOD = 10;
		ENGINE_HR_MESSAGE(device->CreateSamplerState(&samplerDesc, &myClampSampler), "Sampler could not be created.");

		samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
		ENGINE_HR_MESSAGE(device->CreateSamplerState(&samplerDesc, &myWrapSampler), "Sampler could not be created.");
		//End Samplers

#pragma region SSAO Setup
	//for (unsigned int i = 0; i < myKernelSize; ++i) {
	//	float r = 1.0f * sqrt(Random(0.0f, 1.0f));
	//	float theta = Random(float(i) / float(myKernelSize), float(i+1) / float(myKernelSize)) * 2.0f * 3.14159265f;
	//	float x = r * cosf(theta);
	//	float y = r * sinf(theta);
	//	float z = sqrt(1 - x * x - y * y);
	//	myKernel[i] = Vector4(
	//		x,
	//		y,
	//		z,
	//		1.0f);
	//		//myKernel[i].Normalize();
	//		myKernel[i] *= Random(0.0f, 1.0f);
	//		//float scale = float(i) / float(myKernelSize);
	//		//scale = KernelLerp(0.1f, 1.0f, scale * scale);
	//		//myKernel[i] *= scale;
	//}

	// Hardcoded Kernel
		myKernel[0] = { 0.528985322f, 0.163332120f, 0.620016515f, 1.0f };
		myKernel[1] = { 0.573982120f, 0.378577918f, 0.470547318f, 1.0f };
		myKernel[2] = { 0.065050237f, 0.139410198f, 0.347815633f, 1.0f };
		myKernel[3] = { 0.041187014f, 0.130081877f, 0.164059237f, 1.0f };
		myKernel[4] = { -0.026605275f, 0.090929292f, 0.077286638f, 1.0f };
		myKernel[5] = { -0.113886870f, 0.154690191f, 0.197556734f, 1.0f };
		myKernel[6] = { -0.666800976f, 0.662895739f, 0.277599692f, 1.0f };
		myKernel[7] = { -0.399470448f, 0.096369371f, 0.417604893f, 1.0f };
		myKernel[8] = { -0.411310822f, -0.082451604f, 0.179119825f, 1.0f };
		myKernel[9] = { -0.117983297f, -0.095347963f, 0.374402136f, 1.0f };
		myKernel[10] = { -0.457335383f, -0.529036164f, 0.490310162f, 1.0f };
		myKernel[11] = { -0.119527563f, -0.291437626f, 0.206827655f, 1.0f };
		myKernel[12] = { 0.201868936f, -0.513456404f, 0.432056010f, 1.0f };
		myKernel[13] = { 0.096077450f, -0.107414119f, 0.527342558f, 1.0f };
		myKernel[14] = { 0.223280489f, -0.180109233f, 0.203371927f, 1.0f };
		myKernel[15] = { 0.163490131f, -0.039255358f, 0.532910645f, 1.0f };


		SVector4 noise[myKernelSize];
		for (unsigned int i = 0; i < myKernelSize; ++i)
		{
			noise[i] = SVector4(
				UMath::Random(-1.0f, 1.0f),
				UMath::Random(-1.0f, 1.0f),
				0.0f,
				0.0f
			);
			noise[i].Normalize();
		}

		unsigned int width = static_cast<unsigned int>(sqrt(myKernelSize));

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
		ENGINE_HR_MESSAGE(device->CreateShaderResourceView(noiseTextureBuffer, &noiseSRVDesc, &myNoiseTexture), "Noise Shader Resource View could not be created.");

		//Level 1-1 & 1-2
		myPostProcessingBufferData.myWhitePointColor = { 255.0f / 255.0f, 255.0f / 255.0f, 255.0f / 255.0f, 1.0f };
		myPostProcessingBufferData.myWhitePointIntensity = 1.0f;
		myPostProcessingBufferData.myExposure = 1.1f;
		myPostProcessingBufferData.myIsReinhard = false;
		myPostProcessingBufferData.myIsUncharted = false;
		myPostProcessingBufferData.myIsACES = true;

		//Level 2-1 & 2-2
		//myPostProcessingBufferData.myWhitePointColor = { 128.0f/255.0f, 170.0f/255.0f, 1.0f, 1.0f };
		//myPostProcessingBufferData.myWhitePointIntensity = 0.1f;
		//myPostProcessingBufferData.myExposure = 0.1f;
		//myPostProcessingBufferData.myIsReinhard = false;
		//myPostProcessingBufferData.myIsUncharted = true;
		//myPostProcessingBufferData.myIsACES = false;

		myPostProcessingBufferData.mySSAORadius = 0.6f;
		myPostProcessingBufferData.mySSAOSampleBias = 0.2420f;
		myPostProcessingBufferData.mySSAOMagnitude = 1.4f;
		myPostProcessingBufferData.mySSAOContrast = 0.6f;

		myPostProcessingBufferData.myEmissiveStrength = 2.1f;

		myPostProcessingBufferData.myVignetteStrength = 0.35f;
		myPostProcessingBufferData.myPadding = 0.0f;
		myPostProcessingBufferData.myVignetteColor = { 0.0f, 0.0f, 0.0f, 1.0f };

#pragma endregion
		return true;
	}

	void CFullscreenRenderer::Render(FullscreenShader anEffect)
	{
		myFullscreenData.myResolution = CEngine::GetInstance()->GetWindowHandler()->GetResolution();
		myFullscreenData.myNoiseScale = { myFullscreenData.myResolution.X / static_cast<float>(UMath::Sqrt(myKernelSize)), myFullscreenData.myResolution.Y / static_cast<float>(UMath::Sqrt(myKernelSize)) };
		memcpy(&myFullscreenData.mySampleKernel[0], &myKernel[0], sizeof(myKernel));
		BindBuffer(myFullscreenDataBuffer, myFullscreenData, "Fullscreen Data Buffer");

		//CCameraComponent* camera = IRONWROUGHT->GetActiveScene().MainCamera();
		//SM::Matrix& cameraMatrix = camera->GameObject().myTransform->Transform();
		//myFrameBufferData.myCameraPosition = SM::Vector4{ cameraMatrix._41, cameraMatrix._42, cameraMatrix._43, 1.f };
		//myFrameBufferData.myToCameraSpace = cameraMatrix.Invert();
		//myFrameBufferData.myToWorldFromCamera = cameraMatrix;
		//myFrameBufferData.myToProjectionSpace = camera->GetProjection();
		//myFrameBufferData.myToCameraFromProjection = camera->GetProjection().Invert();
		//BindBuffer(myFrameBuffer, myFrameBufferData, "Frame Buffer");

		BindBuffer(myPostProcessingBuffer, myPostProcessingBufferData, "Post Processing Buffer");

		myContext->VSSetConstantBuffers(0, 1, &myFrameBuffer);

		myContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		myContext->IASetInputLayout(nullptr);
		myContext->IASetVertexBuffers(0, 0, nullptr, nullptr, nullptr);
		myContext->IASetIndexBuffer(nullptr, DXGI_FORMAT_UNKNOWN, 0);

		myContext->VSSetShader(myVertexShader, nullptr, 0);
		myContext->PSSetShader(myPixelShaders[static_cast<size_t>(anEffect)], nullptr, 0);
		myContext->PSSetSamplers(0, 1, &myClampSampler);
		myContext->PSSetSamplers(1, 1, &myWrapSampler);
		myContext->PSSetConstantBuffers(0, 1, &myFullscreenDataBuffer);
		myContext->PSSetConstantBuffers(1, 1, &myFrameBuffer);
		myContext->PSSetConstantBuffers(2, 1, &myPostProcessingBuffer);
		myContext->PSSetShaderResources(23, 1, &myNoiseTexture);

		myContext->Draw(3, 0);
		CRenderManager::NumberOfDrawCallsThisFrame++;

		ID3D11ShaderResourceView* nullView = NULL;
		myContext->PSSetShaderResources(0, 1, &nullView);
		myContext->PSSetShaderResources(1, 1, &nullView);
		myContext->PSSetShaderResources(2, 1, &nullView);
		myContext->PSSetShaderResources(3, 1, &nullView);
		myContext->PSSetShaderResources(4, 1, &nullView);
		myContext->PSSetShaderResources(5, 1, &nullView);
		myContext->PSSetShaderResources(6, 1, &nullView);
		myContext->PSSetShaderResources(7, 1, &nullView);
		myContext->PSSetShaderResources(8, 1, &nullView);
		myContext->PSSetShaderResources(9, 1, &nullView);
		myContext->PSSetShaderResources(21, 1, &nullView);
		myContext->PSSetShaderResources(22, 1, &nullView);

		myContext->GSSetShader(nullptr, nullptr, 0);
	}
}
