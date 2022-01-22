// Copyright 2022 Team Havtorn. All Rights Reserved.

#include "hvpch.h"
#include "FullscreenTextureFactory.h"
#include "FullscreenTexture.h"
#include "GraphicsFramework.h"
#include "GBuffer.h"

namespace Havtorn
{
	CFullscreenTextureFactory::CFullscreenTextureFactory() : Framework(nullptr)
	{}

	CFullscreenTextureFactory::~CFullscreenTextureFactory()
	{}

	bool CFullscreenTextureFactory::Init(CDirectXFramework* framework)
	{
		Framework = framework;
		return true;
	}

	CFullscreenTexture CFullscreenTextureFactory::CreateTexture(SVector2<F32> size, DXGI_FORMAT format)
	{
		D3D11_TEXTURE2D_DESC textureDesc = { 0 };
		textureDesc.Width = static_cast<U16>(size.X);
		textureDesc.Height = static_cast<U16>(size.Y);
		textureDesc.MipLevels = 1;
		textureDesc.ArraySize = 1;
		textureDesc.Format = format;
		textureDesc.SampleDesc.Count = 1;
		textureDesc.SampleDesc.Quality = 0;
		textureDesc.Usage = D3D11_USAGE_DEFAULT;
		textureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
		textureDesc.CPUAccessFlags = 0;
		textureDesc.MiscFlags = 0;

		ID3D11Texture2D* texture;
		ENGINE_HR_MESSAGE(Framework->GetDevice()->CreateTexture2D(&textureDesc, nullptr, &texture), "Could not create Fullscreen Texture2D");

		CFullscreenTexture returnTexture;
		returnTexture = CreateTexture(texture);

		ID3D11ShaderResourceView* shaderResource;
		ENGINE_HR_MESSAGE(Framework->GetDevice()->CreateShaderResourceView(texture, nullptr, &shaderResource), "Could not create Fullscreen Shader Resource View.");

		returnTexture.myShaderResource = shaderResource;
		return returnTexture;
	}

	CFullscreenTexture CFullscreenTextureFactory::CreateTexture(ID3D11Texture2D* texture)
	{
		ID3D11RenderTargetView* renderTarget;
		ENGINE_HR_MESSAGE(Framework->GetDevice()->CreateRenderTargetView(texture, nullptr, &renderTarget), "Could not create Fullcreen Render Target View.");

		D3D11_VIEWPORT* viewport = nullptr;
		if (texture) 
		{
			D3D11_TEXTURE2D_DESC textureDescription;
			texture->GetDesc(&textureDescription);
			viewport = new D3D11_VIEWPORT({ 0.0f, 0.0f, static_cast<F32>(textureDescription.Width), static_cast<F32>(textureDescription.Height), 0.0f, 1.0f });
		}

		CFullscreenTexture returnTexture;
		returnTexture.Context = Framework->GetContext();
		returnTexture.myTexture = texture;
		returnTexture.myRenderTarget = renderTarget;
		returnTexture.Viewport = viewport;

		// Temporary test for ImguiManager viewport shenanigans - Nico 28/12-2021
		//ID3D11ShaderResourceView* shaderResource;
		//ENGINE_HR_MESSAGE(Framework->GetDevice()->CreateShaderResourceView(returnTexture.myTexture, nullptr, &shaderResource), "Could not create Fullscreen Shader Resource View.");

		//returnTexture.myShaderResource = shaderResource;
		// Temporary test for ImguiManager viewport shenanigans - Nico 28/12-2021

		return returnTexture;
	}

	CFullscreenTexture CFullscreenTextureFactory::CreateTexture(SVector2<F32> size, DXGI_FORMAT format, const std::string& filePath)
	{
		D3D11_TEXTURE2D_DESC textureDesc = { 0 };
		textureDesc.Width = static_cast<U16>(size.X);
		textureDesc.Height = static_cast<U16>(size.Y);
		textureDesc.MipLevels = 1;
		textureDesc.ArraySize = 1;
		textureDesc.Format = format;
		textureDesc.SampleDesc.Count = 1;
		textureDesc.SampleDesc.Quality = 0;
		textureDesc.Usage = D3D11_USAGE_DEFAULT;
		textureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
		textureDesc.CPUAccessFlags = 0;
		textureDesc.MiscFlags = 0;

		ID3D11Texture2D* texture;
		ENGINE_HR_MESSAGE(Framework->GetDevice()->CreateTexture2D(&textureDesc, nullptr, &texture), "Could not create Fullscreen Texture2D");

		CFullscreenTexture returnTexture;
		returnTexture = CreateTexture(texture);

		ID3D11ShaderResourceView* shaderResource = GetShaderResourceView(Framework->GetDevice(), filePath);

		returnTexture.myShaderResource = shaderResource;
		return returnTexture;
	}

	CFullscreenTexture CFullscreenTextureFactory::CreateDepth(SVector2<F32> size, DXGI_FORMAT format)
	{
		DXGI_FORMAT stencilViewFormat = DXGI_FORMAT_UNKNOWN;
		DXGI_FORMAT shaderResourceViewFormat = DXGI_FORMAT_UNKNOWN;

		switch (format)
		{
		case DXGI_FORMAT_R24G8_TYPELESS:
			stencilViewFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;
			shaderResourceViewFormat = DXGI_FORMAT_R24_UNORM_X8_TYPELESS;
			break;
		case DXGI_FORMAT_R32_TYPELESS:
			stencilViewFormat = DXGI_FORMAT_D32_FLOAT;
			shaderResourceViewFormat = DXGI_FORMAT_R32_FLOAT;
			break;
		default:
			break;
		}

		D3D11_TEXTURE2D_DESC depthStencilDesc = { 0 };
		depthStencilDesc.Width = static_cast<U16>(size.X);
		depthStencilDesc.Height = static_cast<U16>(size.Y);
		depthStencilDesc.MipLevels = 1;
		depthStencilDesc.ArraySize = 1;
		depthStencilDesc.Format = format;
		depthStencilDesc.SampleDesc.Count = 1;
		depthStencilDesc.SampleDesc.Quality = 0;
		depthStencilDesc.Usage = D3D11_USAGE_DEFAULT;
		depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;
		depthStencilDesc.CPUAccessFlags = 0;
		depthStencilDesc.MiscFlags = 0;

		D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;
		depthStencilViewDesc.Format = stencilViewFormat;
		depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
		depthStencilViewDesc.Texture2D.MipSlice = 0;
		depthStencilViewDesc.Flags = 0;

		D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc;
		shaderResourceViewDesc.Format = shaderResourceViewFormat;
		shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		shaderResourceViewDesc.Texture2D.MostDetailedMip = 0;
		shaderResourceViewDesc.Texture2D.MipLevels = 1;

		ID3D11Texture2D* depthStencilBuffer;
		ENGINE_HR_MESSAGE(Framework->GetDevice()->CreateTexture2D(&depthStencilDesc, nullptr, &depthStencilBuffer), "Texture could not be created.");
		ID3D11DepthStencilView* depthStencilView;
		ENGINE_HR_MESSAGE(Framework->GetDevice()->CreateDepthStencilView(depthStencilBuffer, &depthStencilViewDesc, &depthStencilView), "Depth could not be created.");
		ID3D11ShaderResourceView* shaderResource;
		ENGINE_HR_MESSAGE(Framework->GetDevice()->CreateShaderResourceView(depthStencilBuffer, &shaderResourceViewDesc, &shaderResource), "Depth Shader Resource could not be created.");

		D3D11_VIEWPORT* viewport = new D3D11_VIEWPORT({ 0.0f, 0.0f, size.X, size.Y, 0.0f, 1.0f });

		CFullscreenTexture returnDepth;
		returnDepth.Context = Framework->GetContext();
		returnDepth.myTexture = depthStencilBuffer;
		returnDepth.myDepth = depthStencilView;
		returnDepth.myShaderResource = shaderResource;
		returnDepth.Viewport = viewport;
		return returnDepth;
	}

	CGBuffer CFullscreenTextureFactory::CreateGBuffer(SVector2<F32> size)
	{
		std::array<DXGI_FORMAT, static_cast<size_t>(CGBuffer::EGBufferTextures::Count)> textureFormats =
		{
			DXGI_FORMAT_R8G8B8A8_UNORM,
			DXGI_FORMAT_R16G16B16A16_SNORM,
			DXGI_FORMAT_R16G16B16A16_SNORM,
			DXGI_FORMAT_R8G8B8A8_UNORM,
		};

		//Creating textures, rendertargets, shaderresources and a viewport
		std::array<ID3D11Texture2D*, static_cast<size_t>(CGBuffer::EGBufferTextures::Count)> textures;
		std::array<ID3D11RenderTargetView*, static_cast<size_t>(CGBuffer::EGBufferTextures::Count)> renderTargets;
		std::array<ID3D11ShaderResourceView*, static_cast<size_t>(CGBuffer::EGBufferTextures::Count)> shaderResources;
		
		for (UINT i = 0; i < static_cast<size_t>(CGBuffer::EGBufferTextures::Count); ++i) 
		{
			CFullscreenTexture texture = CreateTexture(size, textureFormats[i]);
			textures[i] = texture.myTexture;
			renderTargets[i] = texture.myRenderTarget;
			shaderResources[i] = texture.myShaderResource;
		}
		D3D11_VIEWPORT* viewport = new D3D11_VIEWPORT({ 0.0f, 0.0f, size.X, size.Y, 0.0f, 1.0f });

		CGBuffer returnGBuffer;
		returnGBuffer.Context = Framework->GetContext();
		returnGBuffer.Textures = std::move(textures);
		returnGBuffer.RenderTargets = std::move(renderTargets);
		returnGBuffer.ShaderResources = std::move(shaderResources);
		returnGBuffer.Viewport = viewport;
		return returnGBuffer;
	}

	ID3D11ShaderResourceView* CFullscreenTextureFactory::GetShaderResourceView(ID3D11Device* /*aDevice*/, std::string /*aTexturePath*/)
	{
		//ID3D11ShaderResourceView* shaderResourceView;

		//wchar_t* widePath = new wchar_t[aTexturePath.length() + 1];
		//std::copy(aTexturePath.begin(), aTexturePath.end(), widePath);
		//widePath[aTexturePath.length()] = 0;

		////==ENABLE FOR TEXTURE CHECKING==
		//ENGINE_HR_MESSAGE(DirectX::CreateDDSTextureFromFile(aDevice, widePath, nullptr, &shaderResourceView), aTexturePath.append(" could not be found.").c_str());
		////===============================

		//==DISABLE FOR TEXTURE CHECKING==
		//HRESULT result;
		//result = DirectX::CreateDDSTextureFromFile(aDevice, widePath, nullptr, &shaderResourceView);
		//if (FAILED(result))
		//	DirectX::CreateDDSTextureFromFile(aDevice, L"ErrorTexture.dds", nullptr, &shaderResourceView);
		//================================

		//delete[] widePath;
		//return shaderResourceView;
		return nullptr;
	}
}