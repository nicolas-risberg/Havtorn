// Copyright 2022 Team Havtorn. All Rights Reserved.

#pragma once
#include <fstream>
#include <DirectXTex/DirectXTex.h>
#include "GraphicsFramework.h"
#include "GraphicsEnums.h"

#define ASSETPATH(path) std::string(path).c_str()

namespace Havtorn
{
	namespace UGraphicsUtils
	{
		static bool CreateVertexShader(const std::string& filepath, const CGraphicsFramework* framework, ID3D11VertexShader** outVertexShader, std::string& outShaderData)
		{
			std::ifstream vsFile;
			vsFile.open(filepath, std::ios::binary);
			const std::string vsData = { std::istreambuf_iterator<char>(vsFile), std::istreambuf_iterator<char>() };
			outShaderData = vsData;
			ENGINE_HR_BOOL_MESSAGE(framework->GetDevice()->CreateVertexShader(vsData.data(), vsData.size(), nullptr, outVertexShader), "Vertex Shader could not be created.");
			vsFile.close();
			return true;
		}

		static bool CreatePixelShader(const std::string& filepath, const CGraphicsFramework* framework, ID3D11PixelShader** outPixelShader)
		{
			std::ifstream psFile;
			psFile.open(filepath, std::ios::binary);
			const std::string psData = { std::istreambuf_iterator<char>(psFile), std::istreambuf_iterator<char>() };
			ENGINE_HR_BOOL_MESSAGE(framework->GetDevice()->CreatePixelShader(psData.data(), psData.size(), nullptr, outPixelShader), "Pixel Shader could not be created.");
			psFile.close();
			return true;
		}

		static bool CreateGeometryShader(const std::string& filepath, const CGraphicsFramework* framework, ID3D11GeometryShader** outGeometryShader)
		{
			std::ifstream gsFile;
			gsFile.open(filepath, std::ios::binary);
			const std::string gsData = { std::istreambuf_iterator<char>(gsFile), std::istreambuf_iterator<char>() };
			ENGINE_HR_BOOL_MESSAGE(framework->GetDevice()->CreateGeometryShader(gsData.data(), gsData.size(), nullptr, outGeometryShader), "Geometry Shader could not be created.");
			gsFile.close();
			return true;
		}

		static HRESULT CreateShaderResourceView(ID3D11Device* device, const std::string& filePath, ID3D11ShaderResourceView** outShaderResourceView)
		{
			DirectX::ScratchImage scratchImage;
			DirectX::TexMetadata metaData = {};

			const auto widePath = new wchar_t[filePath.length() + 1];
			std::ranges::copy(filePath, widePath);
			widePath[filePath.length()] = 0;

			ETextureFormat format = {};
			if (const std::string extension = filePath.substr(filePath.size() - 4); extension == ".dds")
				format = ETextureFormat::DDS;
			else if (extension == ".tga")
				format = ETextureFormat::TGA;

			switch (format)
			{
			case ETextureFormat::DDS:
				GetMetadataFromDDSFile(widePath, DirectX::DDS_FLAGS_NONE, metaData);
				LoadFromDDSFile(widePath, DirectX::DDS_FLAGS_NONE, &metaData, scratchImage);
				
				break;
			case ETextureFormat::TGA:
				GetMetadataFromTGAFile(widePath, DirectX::TGA_FLAGS_NONE, metaData);
				LoadFromTGAFile(widePath, DirectX::TGA_FLAGS_NONE, &metaData, scratchImage);
				break;
			}
			delete[] widePath;
			const DirectX::Image* image = scratchImage.GetImage(0, 0, 0);

			return DirectX::CreateShaderResourceView(device, image, scratchImage.GetImageCount(), metaData, outShaderResourceView);
		}

		static HRESULT CreateShaderResourceView(ID3D11Device* device, const void* data, U64 size, ETextureFormat format, ID3D11ShaderResourceView** outShaderResourceView)
		{
			DirectX::ScratchImage scratchImage;
			DirectX::TexMetadata metaData = {};

			switch (format)
			{
			case ETextureFormat::DDS:
				GetMetadataFromDDSMemory(data, size, DirectX::DDS_FLAGS_NONE, metaData);
				LoadFromDDSMemory(data, size, DirectX::DDS_FLAGS_NONE, &metaData, scratchImage);

				break;
			case ETextureFormat::TGA:
				GetMetadataFromTGAMemory(data, size, DirectX::TGA_FLAGS_NONE, metaData);
				LoadFromTGAMemory(data, size, DirectX::TGA_FLAGS_NONE, &metaData, scratchImage);
				break;
			}

			const DirectX::Image* image = scratchImage.GetImage(0, 0, 0);

			return DirectX::CreateShaderResourceView(device, image, scratchImage.GetImageCount(), metaData, outShaderResourceView);
		}

		static ID3D11ShaderResourceView* TryGetShaderResourceView(ID3D11Device* device, const std::string& texturePath)
		{
			ID3D11ShaderResourceView* shaderResourceView;

			const std::string errorString = "Texture with the following path could not be loaded: " + texturePath;
			ENGINE_HR_MESSAGE(CreateShaderResourceView(device, texturePath, &shaderResourceView), errorString.c_str());
			
			return shaderResourceView;
		}

		static ID3D11ShaderResourceView* TryGetShaderResourceView(ID3D11Device* device, const void* data, U64 size, ETextureFormat format)
		{
			ID3D11ShaderResourceView* shaderResourceView;

			std::string formatString = "";
			switch (format)
			{
			case Havtorn::ETextureFormat::DDS:
				formatString = "DDS";
				break;
			case Havtorn::ETextureFormat::TGA:
				formatString = "TGA";
				break;
			}

			const std::string errorString = formatString + " texture could not be loaded from memory!";
			ENGINE_HR_MESSAGE(CreateShaderResourceView(device, data, size, format, &shaderResourceView), errorString.c_str());

			return shaderResourceView;
		}

		static ID3D11ShaderResourceView* GetShaderResourceView(ID3D11Device* device, const std::string& texturePath)
		{
			ID3D11ShaderResourceView* shaderResourceView;

			////==ENABLE FOR TEXTURE CHECKING==
			//ENGINE_HR_MESSAGE(CreateShaderResourceView(device, texturePath, &shaderResourceView), texturePath.append(" could not be found.").c_str());
			////===============================

			const HRESULT result = CreateShaderResourceView(device, texturePath, &shaderResourceView);
			if (FAILED(result))
			{
				std::string errorTexturePath = texturePath.substr(texturePath.length() - 6);
				errorTexturePath = "Assets/ErrorTextures/Checkboard_128x128" + errorTexturePath;

				CreateShaderResourceView(device, errorTexturePath, &shaderResourceView);
			}

			return shaderResourceView;
		}
	}
}