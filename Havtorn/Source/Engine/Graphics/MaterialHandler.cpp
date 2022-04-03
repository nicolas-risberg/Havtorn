// Copyright 2022 Team Havtorn. All Rights Reserved.

#include "hvpch.h"
#include "MaterialHandler.h"
#include "Engine.h"
#include "GraphicsFramework.h"
#include "GraphicsUtilities.h"
#include <unordered_map>

namespace Havtorn
{
	std::array<ID3D11ShaderResourceView*, 3> CMaterialHandler::RequestMaterial(const I32 materialID, const std::string& modelPath)
	{
		std::string materialName;
		if (!TryGetMaterialName(materialID, materialName))
		{
			ENGINE_BOOL_POPUP(false, "Missing material on Model: %s", modelPath.c_str());
			return std::array<ID3D11ShaderResourceView*, 3>();
		}
		//TODO
		//Maybe the Textures Path thing should be Made into some kind of Material Struct with a _C, _M & _N strings.

		if (!Materials.contains(materialName)) //Setup Material
		{
			std::array<std::string, 3> texturePaths = {};
			if (!TryGetMaterialsPath(materialID, texturePaths))
				return std::array<ID3D11ShaderResourceView*, 3>();

			HV_LOG_INFO("Loading Material: %s", materialName.c_str());
			std::array<Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>, 3> newTextures;

			if (!texturePaths[0].empty())
				newTextures[0] = UGraphicsUtils::GetShaderResourceView(Device, ASSETPATH(texturePaths[0]));
			else
				newTextures[0] = UGraphicsUtils::GetShaderResourceView(Device, ASSETPATH("Assets/Textures/ErrorTextures/Checkboard_128x128_c.dds"));

			if (!texturePaths[1].empty())
				newTextures[1] = UGraphicsUtils::GetShaderResourceView(Device, ASSETPATH(texturePaths[1]));
			else
				newTextures[1] = UGraphicsUtils::GetShaderResourceView(Device, ASSETPATH("Assets/IronWrought/Texture/ErrorTextures/Checkboard_128x128_m.dds"));

			if (!texturePaths[2].empty())
				newTextures[2] = UGraphicsUtils::GetShaderResourceView(Device, ASSETPATH(texturePaths[2]));
			else
				newTextures[2] = UGraphicsUtils::GetShaderResourceView(Device, ASSETPATH("Assets/IronWrought/Texture/ErrorTextures/Checkboard_128x128_n.dds"));

			Materials.emplace(materialName, std::move(newTextures));
			MaterialReferences.emplace(materialName, 0);
			TransparentMaterialMap.emplace(materialID, IsMaterialTransparent(texturePaths));
		}

		MaterialReferences[materialName] += 1;
		std::array<ID3D11ShaderResourceView*, 3> textures;
		textures[0] = Materials[materialName][0].Get();
		textures[1] = Materials[materialName][1].Get();
		textures[2] = Materials[materialName][2].Get();
		return textures;
	}

	std::array<ID3D11ShaderResourceView*, 3> CMaterialHandler::RequestMaterial(const I32 materialID)
	{
		std::string materialName;
		if (!TryGetMaterialName(materialID, materialName))
		{
			return std::array<ID3D11ShaderResourceView*, 3>();
		}
		//TODO
		//Maybe the Textures Path thing should be Made into some kind of Material Struct with a _C, _M & _N strings.

		if (!Materials.contains(materialName)) //Setup Material
		{
			std::array<std::string, 3> texturePaths = {};
			if (!TryGetMaterialsPath(materialID, texturePaths))
				return std::array<ID3D11ShaderResourceView*, 3>();

			HV_LOG_INFO("Loading Material: %s", materialName.c_str());
			std::array<Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>, 3> newTextures;

			if (!texturePaths[0].empty())
				newTextures[0] = UGraphicsUtils::GetShaderResourceView(Device, ASSETPATH(texturePaths[0]));
			else
				newTextures[0] = UGraphicsUtils::GetShaderResourceView(Device, ASSETPATH("Assets/Textures/ErrorTextures/Checkboard_128x128_c.dds"));

			if (!texturePaths[1].empty())
				newTextures[1] = UGraphicsUtils::GetShaderResourceView(Device, ASSETPATH(texturePaths[1]));
			else
				newTextures[1] = UGraphicsUtils::GetShaderResourceView(Device, ASSETPATH("Assets/Textures/ErrorTextures/Checkboard_128x128_m.dds"));

			if (!texturePaths[2].empty())
				newTextures[2] = UGraphicsUtils::GetShaderResourceView(Device, ASSETPATH(texturePaths[2]));
			else
				newTextures[2] = UGraphicsUtils::GetShaderResourceView(Device, ASSETPATH("Assets/Textures/ErrorTextures/Checkboard_128x128_n.dds"));

			Materials.emplace(materialName, std::move(newTextures));
			MaterialReferences.emplace(materialName, 0);
			TransparentMaterialMap.emplace(materialID, IsMaterialTransparent(texturePaths));
		}

		MaterialReferences[materialName] += 1;
		std::array<ID3D11ShaderResourceView*, 3> textures;
		textures[0] = Materials[materialName][0].Get();
		textures[1] = Materials[materialName][1].Get();
		textures[2] = Materials[materialName][2].Get();
		return textures;
	}

	std::array<ID3D11ShaderResourceView*, 3> CMaterialHandler::RequestMaterial(const std::string& aMaterialName)
	{
		I32 materialID = 0;
		TryGetMaterialID(aMaterialName, materialID);
		return RequestMaterial(materialID);
	}

	std::array<ID3D11ShaderResourceView*, 3> CMaterialHandler::RequestMaterial(const std::string& aMaterialName, I32& anOutMaterialID)
	{
		anOutMaterialID = 0;
		TryGetMaterialID(aMaterialName, anOutMaterialID);
		return RequestMaterial(anOutMaterialID);
	}

	std::array<ID3D11ShaderResourceView*, 3> CMaterialHandler::RequestDefualtMaterial(I32& anOutMaterialID)
	{
		return RequestMaterial("DefaultMaterial", anOutMaterialID);
	}

	//if (myMaterials.find(aMaterialName) == myMaterials.end())
	//{
	//	std::array<Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>, 3> newTextures;
	//	newTextures[0] = Graphics::GetShaderResourceView(myDevice, myMaterialPath + aMaterialName + "/" + aMaterialName + "_c.dds");
	//	newTextures[1] = Graphics::GetShaderResourceView(myDevice, myMaterialPath + aMaterialName + "/" + aMaterialName + "_m.dds");
	//	newTextures[2] = Graphics::GetShaderResourceView(myDevice, myMaterialPath + aMaterialName + "/" + aMaterialName + "_n.dds");

	//	myMaterials.emplace(aMaterialName, std::move(newTextures));
	//	myMaterialReferences.emplace(aMaterialName, 0);
	//}

	//myMaterialReferences[aMaterialName] += 1;
	//std::array<ID3D11ShaderResourceView*, 3> textures;
	//textures[0] = myMaterials[aMaterialName][0].Get();
	//textures[1] = myMaterials[aMaterialName][1].Get();
	//textures[2] = myMaterials[aMaterialName][2].Get();
	//return textures;

	//Copy Old RequestMateriall
	//Replace pre-defined TexturePaths and use these 3 texturePaths instead. 
	//Texture Paths is expected to contain a _C, _M and _N texture Paths.




	/*
	vector<string> textures;
	if(CJsonReader::TryRead(aMaterialID, textures))
		return RequestMaterial(textures);
	else
		return DefaultMaterial/ErrorMaterial
	*/





	std::array<ID3D11ShaderResourceView*, 3> CMaterialHandler::RequestDecal(const std::string& decalName)
	{
		if (!Materials.contains(decalName))
		{
			std::array<Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>, 3> newTextures;
			newTextures[0] = UGraphicsUtils::TryGetShaderResourceView(Device, DecalPath + decalName + "/" + decalName + "_c.dds");
			newTextures[1] = UGraphicsUtils::TryGetShaderResourceView(Device, DecalPath + decalName + "/" + decalName + "_m.dds");
			newTextures[2] = UGraphicsUtils::TryGetShaderResourceView(Device, DecalPath + decalName + "/" + decalName + "_n.dds");

			Materials.emplace(decalName, std::move(newTextures));
			MaterialReferences.emplace(decalName, 0);
		}

		MaterialReferences[decalName] += 1;
		std::array<ID3D11ShaderResourceView*, 3> textures;
		textures[0] = Materials[decalName][0].Get();
		textures[1] = Materials[decalName][1].Get();
		textures[2] = Materials[decalName][2].Get();
		return textures;
	}

	std::array<ID3D11ShaderResourceView*, 9> CMaterialHandler::GetVertexPaintMaterials(const std::vector<std::string>& materialNames)
	{
		std::array<ID3D11ShaderResourceView*, 9> textures = {};
		for (U16 i = 0; i < materialNames.size(); ++i)
		{
			textures[0 + (i * 3)] = Materials[materialNames[i]][0].Get();
			textures[1 + (i * 3)] = Materials[materialNames[i]][1].Get();
			textures[2 + (i * 3)] = Materials[materialNames[i]][2].Get();
		}

		return textures;
	}

	void CMaterialHandler::ReleaseMaterial(const std::string& materialName)
	{
		if (Materials.contains(materialName))
		{
			MaterialReferences[materialName] -= 1;

			if (MaterialReferences[materialName] <= 0)
			{
				ULONG remainingRefs = 0;
				do
				{
					if (Materials[materialName][0].Get())
						Materials[materialName][0].Get()->Release();
					if (Materials[materialName][1].Get())
						Materials[materialName][1].Get()->Release();
					if (Materials[materialName][2].Get())
						remainingRefs = Materials[materialName][2].Get()->Release();
					else
						remainingRefs = 0;
				} while (remainingRefs > 1);

				Materials.erase(materialName);
				MaterialReferences.erase(materialName);
			}
		}
	}

	SVertexPaintData CMaterialHandler::RequestVertexColorID(const std::vector<SVertexPaintColorData>::const_iterator& /*it*/, const std::string& /*modelPath*/, const std::vector<std::string>& /*materials*/)
	{
		////const std::string& vertexColorsPath = CJsonReader::Get()->GetAssetPath(aVertexColorsID);
		////SVertexPaintColorData colorData = CBinReader::LoadVertexColorData(aVertexColorsID);
		//SVertexPaintColorData colorData = *it;
		//std::vector<Vector3>& fbxVertexPositions = CModelFactory::GetInstance()->GetVertexPositions(aFbxModelPath);
		//std::unordered_map<Vector3, Vector3, CMaterialHandler::VectorHasher, VertexPositionComparer> vertexPositionToColorMap;

		//const float epsilon = 0.001f;
		//for (const auto& ourFBXVertexPosition : fbxVertexPositions)
		//{
		//	vertexPositionToColorMap[ourFBXVertexPosition] = { .0f, .0f, .0f };
		//}

		//for (unsigned int j = 0; j < colorData.myColors.size(); ++j)
		//{
		//	vertexPositionToColorMap[{ -colorData.myVertexPositions[j].x, colorData.myVertexPositions[j].y, -colorData.myVertexPositions[j].z}] = colorData.myColors[j];
		//	//vertexPositionToColorMap[{ colorData.myVertexPositions[j].z, colorData.myVertexPositions[j].x, colorData.myVertexPositions[j].y}] = colorData.myColors[j];

		//}

		//std::vector<Vector3> rgbColorData = {};
		//rgbColorData.resize(fbxVertexPositions.size());

		//for (int i = 0; i < fbxVertexPositions.size(); ++i)
		//{
		//	rgbColorData[i] = vertexPositionToColorMap[fbxVertexPositions[i]];
		//}

		////for (auto& ourFBXVertexPositionValue : fbxVertexPositions)
		////{
		////	rgbColorData.push_back(vertexPositionToColorMap[ourFBXVertexPositionValue]);
		////}

		//if (myVertexColorBuffers.find(colorData.myVertexMeshID) == myVertexColorBuffers.end())
		//{
		//	D3D11_BUFFER_DESC vertexColorBufferDesc;
		//	vertexColorBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
		//	vertexColorBufferDesc.ByteWidth = sizeof(rgbColorData[0]) * static_cast<UINT>(rgbColorData.size());

		//	vertexColorBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		//	vertexColorBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		//	vertexColorBufferDesc.MiscFlags = 0;
		//	vertexColorBufferDesc.StructureByteStride = 0;
		//	D3D11_SUBRESOURCE_DATA subResourceData = { 0 };
		//	subResourceData.pSysMem = rgbColorData.data();

		//	ID3D11Buffer* vertexColorBuffer;
		//	ENGINE_HR_MESSAGE(CEngine::GetInstance()->myFramework->GetDevice()->CreateBuffer(&vertexColorBufferDesc, &subResourceData, &vertexColorBuffer), "Vertex Color Buffer could not be created.");

		//	myVertexColorBuffers.emplace(colorData.myVertexMeshID, std::move(vertexColorBuffer));
		//	myVertexColorReferences.emplace(colorData.myVertexMeshID, 0);
		//}
		//myVertexColorReferences[colorData.myVertexMeshID] += 1;

		//for (const auto& materialName : someMatrials)
		//{
		//	RequestMaterial(materialName);
		//}
		//return { someMatrials, colorData.myVertexMeshID };
		return {};
	}

	const std::vector<SVector>& CMaterialHandler::GetVertexColors(I32 vertexColorID)
	{
		return VertexColors[vertexColorID];
	}

	ID3D11Buffer* CMaterialHandler::GetVertexColorBuffer(I32 vertexColorID)
	{
		return VertexColorBuffers[vertexColorID];
	}

	void CMaterialHandler::ReleaseVertexColors(I32 aVertexColorID)
	{
		if (VertexColorBuffers.contains(aVertexColorID))
		{
			VertexColorReferences[aVertexColorID] -= 1;

			if (VertexColorReferences[aVertexColorID] <= 0)
			{
				ULONG remainingRefs = 0;
				do
				{
					remainingRefs = VertexColorBuffers[aVertexColorID]->Release();
				} while (remainingRefs > 1);

				VertexColorBuffers.erase(aVertexColorID);
				VertexColorReferences.erase(aVertexColorID);
			}
		}
	}

	const std::array<Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>, 3>& CMaterialHandler::GetMaterial(const I32 materialID) const
	{
		std::string materialName = {};
		//CJsonReader::Get()->TryGetMaterialName(aMaterialID, materialName);
		//return myMaterials.at(materialName);

		if (TryGetMaterialName(materialID, materialName))
		{
			return Materials.at(materialName);
		}
		else
		{
			return Materials.at("DefaultMaterial");
		}
	}

	SMaterialInstance CMaterialHandler::GetMaterialInstance(const I32 materialID) const
	{
		std::string materialName = {};
		//TryGetMaterialName(materialID, materialName);
		//return SMaterialInstance{ 5, 3, Materials.at(materialName) };

		if (TryGetMaterialName(materialID, materialName))
		{
			return SMaterialInstance{ 5, 3, Materials.at(materialName) };
		}
		else
		{
			return SMaterialInstance{ 5, 3, Materials.at("DefaultMaterial") };
		}
	}

	bool CMaterialHandler::TryGetMaterialName(const I32 materialID, std::string& outMaterialName) const
	{
		if (!MaterialsNameMap.contains(materialID))
		{
			outMaterialName = "";
			return false;
		}
		outMaterialName = MaterialsNameMap.at(materialID);
		return true;
	}

	bool CMaterialHandler::TryGetMaterialsPath(const I32 materialID, std::array<std::string, 3>& outTexturePaths) const
	{
		if (!MaterialsMap.contains(materialID))
		{
			outTexturePaths = {};
			return false;
		}

		const std::vector<I32>& textureIds = MaterialsMap.at(materialID);
		for (const auto& textureID : textureIds)
		{
			const std::string& texturePath = PathsMap.at(textureID);
			const size_t indexOfType = texturePath.find_last_of('_');
			char textureType = texturePath[indexOfType + static_cast<size_t>(1)];

			textureType = static_cast<char>(std::toupper(textureType));

			if (textureType == 'A')
			{
				textureType = texturePath[indexOfType - static_cast<size_t>(1)];
			}

			switch (textureType)
			{
			case 'C':
				outTexturePaths[0] = texturePath;
				break;
			case 'M':
				outTexturePaths[1] = texturePath;
				break;
			case 'N':
				outTexturePaths[2] = texturePath;
				break;
			default:
				//Ping Error here - Invalid Texture Type has been loaded. This should be caught earlier in the pipeline.
				break;
			}
		}
		return true;
	}

	bool CMaterialHandler::TryGetMaterialID(const std::string& materialName, I32& outMaterialID) const
	{
		if (!MaterialsIDMap.contains(materialName))
		{
			outMaterialID = 0;
			return false;
		}
		outMaterialID = MaterialsIDMap.at(materialName);
		return true;
	}

	bool CMaterialHandler::IsMaterialTransparent(const I32 materialID) const
	{
		if (!TransparentMaterialMap.contains(materialID))
		{
#ifdef _DEBUG
			//std::string id = std::to_string(aMaterialID);
			//id.append(": Material ID does not exist in myMaterialIsAlphaMap!");
			//ENGINE_ERROR_BOOL_MESSAGE(false, id.c_str());
			//std::cout << __FUNCTION__ << " " << id << std::endl;
#endif
			return false;
		}

		return TransparentMaterialMap.at(materialID);
	}

	bool CMaterialHandler::Init(const CGraphicsFramework* framework)
	{
		Device = framework->GetDevice();

		if (!Device)
		{
			return false;
		}
		std::array<ID3D11ShaderResourceView*, 3> defaultMatInit = RequestMaterial("DefaultMaterial");
		defaultMatInit[0] = nullptr;
		defaultMatInit[1] = nullptr;
		defaultMatInit[2] = nullptr;
		return true;
	}

	inline bool CMaterialHandler::IsMaterialTransparent(const std::array<std::string, 3>& texturePaths)
	{
		for (const auto& materialName : texturePaths)
			if (materialName.size() > 6)
				if (materialName.substr(materialName.size() - 6, 2) == "AL")
					return true;

		return false;
	}

	CMaterialHandler::CMaterialHandler()
		: Device(nullptr)
		, MaterialPath(ASSETPATH("Assets/IronWrought/Texture/"))
		, DecalPath(ASSETPATH("Assets/IronWrought/Texture/Decal_texture/"))
		, VertexLinksPath(ASSETPATH("Assets/Generated/"))
	{
	}
}