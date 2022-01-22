// Copyright 2022 Team Havtorn. All Rights Reserved.

#include "hvpch.h"
#include "MaterialHandler.h"
#include "Engine.h"
#include "GraphicsFramework.h"
//#include "BinReader.h"
//#include "JsonReader.h"
//#include "ModelFactory.h"
#include "GraphicsUtilities.h"
#include <unordered_map>
//#include "FolderUtility.h"
//#include <BinReader.h>

namespace Havtorn
{
//	std::array<ID3D11ShaderResourceView*, 3> CMaterialHandler::RequestMaterial(const I32 aMaterialID, const std::string& aModelPath)
//	{
//		std::string materialName = "";
//		if (!CJsonReader::Get()->TryGetMaterialName(aMaterialID, materialName))
//		{
//			ENGINE_BOOL_POPUP(false, "Missing material on FBX: %s", aModelPath.c_str());
//			return std::array<ID3D11ShaderResourceView*, 3>();
//		}
//		//TODO
//		//Maybe the Textures Path thing should be Made into some kind of Material Struct with a _C, _M & _N strings.
//		//This could make life a little bit easier!<3
//
//		if (myMaterials.find(materialName) == myMaterials.end()) //Setup Material
//		{
//			std::array<std::string, 3> texturePaths = {};
//			if (!CJsonReader::Get()->TryGetMaterialsPath(aMaterialID, texturePaths))
//				return std::array<ID3D11ShaderResourceView*, 3>();
//
//			std::cout << "Loading Material: " << materialName << "\n";
//			std::array<Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>, 3> newTextures;
//
//			if (!texturePaths[0].empty())
//				newTextures[0] = Graphics::GetShaderResourceView(myDevice, ASSETPATH(texturePaths[0]));
//			else
//				newTextures[0] = Graphics::GetShaderResourceView(myDevice, ASSETPATH("Assets/IronWrought/Texture/ErrorTextures/Checkboard_128x128_c"));
//
//			if (!texturePaths[1].empty())
//				newTextures[1] = Graphics::GetShaderResourceView(myDevice, ASSETPATH(texturePaths[1]));
//			else
//				newTextures[1] = Graphics::GetShaderResourceView(myDevice, ASSETPATH("Assets/IronWrought/Texture/ErrorTextures/Checkboard_128x128_m"));
//
//			if (!texturePaths[2].empty())
//				newTextures[2] = Graphics::GetShaderResourceView(myDevice, ASSETPATH(texturePaths[2]));
//			else
//				newTextures[2] = Graphics::GetShaderResourceView(myDevice, ASSETPATH("Assets/IronWrought/Texture/ErrorTextures/Checkboard_128x128_n"));
//
//			myMaterials.emplace(materialName, std::move(newTextures));
//			myMaterialReferences.emplace(materialName, 0);
//			myMaterialIsAlphaMap.emplace(aMaterialID, MaterialIsAlpha(texturePaths));
//
//		}
//
//		myMaterialReferences[materialName] += 1;
//		std::array<ID3D11ShaderResourceView*, 3> textures;
//		textures[0] = myMaterials[materialName][0].Get();
//		textures[1] = myMaterials[materialName][1].Get();
//		textures[2] = myMaterials[materialName][2].Get();
//		return textures;
//
//		//std::cout << "No Material Found! " << std::endl;
//
//	}
//
//	std::array<ID3D11ShaderResourceView*, 3> CMaterialHandler::RequestMaterial(const int aMaterialID)
//	{
//		std::string materialName = "";
//		if (!CJsonReader::Get()->TryGetMaterialName(aMaterialID, materialName))
//		{
//			return std::array<ID3D11ShaderResourceView*, 3>();
//		}
//		//TODO
//		//Maybe the Textures Path thing should be Made into some kind of Material Struct with a _C, _M & _N strings.
//		//This could make life a little bit easier!<3
//
//		if (myMaterials.find(materialName) == myMaterials.end()) //Setup Material
//		{
//			std::array<std::string, 3> texturePaths = {};
//			if (!CJsonReader::Get()->TryGetMaterialsPath(aMaterialID, texturePaths))
//				return std::array<ID3D11ShaderResourceView*, 3>();
//
//			std::cout << "Loading Material: " << materialName << "\n";
//			std::array<Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>, 3> newTextures;
//
//			if (!texturePaths[0].empty())
//				newTextures[0] = Graphics::GetShaderResourceView(myDevice, ASSETPATH(texturePaths[0]));
//			else
//				newTextures[0] = Graphics::GetShaderResourceView(myDevice, ASSETPATH("Assets/IronWrought/Texture/ErrorTextures/Checkboard_128x128_c"));
//
//			if (!texturePaths[1].empty())
//				newTextures[1] = Graphics::GetShaderResourceView(myDevice, ASSETPATH(texturePaths[1]));
//			else
//				newTextures[1] = Graphics::GetShaderResourceView(myDevice, ASSETPATH("Assets/IronWrought/Texture/ErrorTextures/Checkboard_128x128_m"));
//
//			if (!texturePaths[2].empty())
//				newTextures[2] = Graphics::GetShaderResourceView(myDevice, ASSETPATH(texturePaths[2]));
//			else
//				newTextures[2] = Graphics::GetShaderResourceView(myDevice, ASSETPATH("Assets/IronWrought/Texture/ErrorTextures/Checkboard_128x128_n"));
//
//			myMaterials.emplace(materialName, std::move(newTextures));
//			myMaterialReferences.emplace(materialName, 0);
//			myMaterialIsAlphaMap.emplace(aMaterialID, MaterialIsAlpha(texturePaths));
//		}
//
//		myMaterialReferences[materialName] += 1;
//		std::array<ID3D11ShaderResourceView*, 3> textures;
//		textures[0] = myMaterials[materialName][0].Get();
//		textures[1] = myMaterials[materialName][1].Get();
//		textures[2] = myMaterials[materialName][2].Get();
//		return textures;
//
//		//std::cout << "No Material Found! " << std::endl;
//
//	}
//
//	std::array<ID3D11ShaderResourceView*, 3> CMaterialHandler::RequestMaterial(const std::string& aMaterialName)
//	{
//		int materialID = 0;
//		CJsonReader::Get()->TryGetMaterialID(aMaterialName, materialID);
//		return RequestMaterial(materialID);
//	}
//
//	std::array<ID3D11ShaderResourceView*, 3> CMaterialHandler::RequestMaterial(const std::string& aMaterialName, int& anOutMaterialID)
//	{
//		anOutMaterialID = 0;
//		CJsonReader::Get()->TryGetMaterialID(aMaterialName, anOutMaterialID);
//		return RequestMaterial(anOutMaterialID);
//	}
//
//	std::array<ID3D11ShaderResourceView*, 3> CMaterialHandler::RequestDefualtMaterial(int& anOutMaterialID)
//	{
//		return RequestMaterial("DefaultMaterial", anOutMaterialID);
//	}
//
//	//if (myMaterials.find(aMaterialName) == myMaterials.end())
//	//{
//	//	std::array<Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>, 3> newTextures;
//	//	newTextures[0] = Graphics::GetShaderResourceView(myDevice, myMaterialPath + aMaterialName + "/" + aMaterialName + "_c.dds");
//	//	newTextures[1] = Graphics::GetShaderResourceView(myDevice, myMaterialPath + aMaterialName + "/" + aMaterialName + "_m.dds");
//	//	newTextures[2] = Graphics::GetShaderResourceView(myDevice, myMaterialPath + aMaterialName + "/" + aMaterialName + "_n.dds");
//
//	//	myMaterials.emplace(aMaterialName, std::move(newTextures));
//	//	myMaterialReferences.emplace(aMaterialName, 0);
//	//}
//
//	//myMaterialReferences[aMaterialName] += 1;
//	//std::array<ID3D11ShaderResourceView*, 3> textures;
//	//textures[0] = myMaterials[aMaterialName][0].Get();
//	//textures[1] = myMaterials[aMaterialName][1].Get();
//	//textures[2] = myMaterials[aMaterialName][2].Get();
//	//return textures;
//
//	//Copy Old RequestMateriall
//	//Replace pre-defined TexturePaths and use these 3 texturePaths instead. 
//	//Texture Paths is expected to contain a _C, _M and _N texture Paths.
//
//
//
//
//	/*
//	vector<string> textures;
//	if(CJsonReader::TryRead(aMaterialID, textures))
//		return RequestMaterial(textures);
//	else
//		return DefaultMaterial/ErrorMaterial
//	*/
//
//
//
//
//
//	std::array<ID3D11ShaderResourceView*, 3> CMaterialHandler::RequestDecal(const std::string& aDecalName)
//	{
//		if (myMaterials.find(aDecalName) == myMaterials.end())
//		{
//			std::array<Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>, 3> newTextures;
//			newTextures[0] = Graphics::TryGetShaderResourceView(myDevice, myDecalPath + aDecalName + "/" + aDecalName + "_c.dds");
//			newTextures[1] = Graphics::TryGetShaderResourceView(myDevice, myDecalPath + aDecalName + "/" + aDecalName + "_m.dds");
//			newTextures[2] = Graphics::TryGetShaderResourceView(myDevice, myDecalPath + aDecalName + "/" + aDecalName + "_n.dds");
//
//			myMaterials.emplace(aDecalName, std::move(newTextures));
//			myMaterialReferences.emplace(aDecalName, 0);
//		}
//
//		myMaterialReferences[aDecalName] += 1;
//		std::array<ID3D11ShaderResourceView*, 3> textures;
//		textures[0] = myMaterials[aDecalName][0].Get();
//		textures[1] = myMaterials[aDecalName][1].Get();
//		textures[2] = myMaterials[aDecalName][2].Get();
//		return textures;
//	}
//
//	std::array<ID3D11ShaderResourceView*, 9> CMaterialHandler::GetVertexPaintMaterials(const std::vector<std::string>& someMaterialNames)
//	{
//		std::array<ID3D11ShaderResourceView*, 9> textures = {};
//		for (unsigned int i = 0; i < someMaterialNames.size(); ++i)
//		{
//			textures[0 + (i * 3)] = myMaterials[someMaterialNames[i]][0].Get();
//			textures[1 + (i * 3)] = myMaterials[someMaterialNames[i]][1].Get();
//			textures[2 + (i * 3)] = myMaterials[someMaterialNames[i]][2].Get();
//		}
//
//		return textures;
//	}
//
//	void CMaterialHandler::ReleaseMaterial(const std::string& aMaterialName)
//	{
//		if (myMaterials.find(aMaterialName) != myMaterials.end())
//		{
//			myMaterialReferences[aMaterialName] -= 1;
//
//			if (myMaterialReferences[aMaterialName] <= 0)
//			{
//				ULONG remainingRefs = 0;
//				do
//				{
//					if (myMaterials[aMaterialName][0].Get())
//						myMaterials[aMaterialName][0].Get()->Release();
//					if (myMaterials[aMaterialName][1].Get())
//						myMaterials[aMaterialName][1].Get()->Release();
//					if (myMaterials[aMaterialName][2].Get())
//						remainingRefs = myMaterials[aMaterialName][2].Get()->Release();
//					else
//						remainingRefs = 0;
//				} while (remainingRefs > 1);
//
//				myMaterials.erase(aMaterialName);
//				myMaterialReferences.erase(aMaterialName);
//			}
//		}
//	}
//
//	SVertexPaintData CMaterialHandler::RequestVertexColorID(std::vector<SVertexPaintColorData>::const_iterator& it, const std::string& aFbxModelPath, const std::vector<std::string>& someMatrials)
//	{
//		//const std::string& vertexColorsPath = CJsonReader::Get()->GetAssetPath(aVertexColorsID);
//		//SVertexPaintColorData colorData = CBinReader::LoadVertexColorData(aVertexColorsID);
//		SVertexPaintColorData colorData = *it;
//		std::vector<Vector3>& fbxVertexPositions = CModelFactory::GetInstance()->GetVertexPositions(aFbxModelPath);
//		std::unordered_map<Vector3, Vector3, CMaterialHandler::VectorHasher, VertexPositionComparer> vertexPositionToColorMap;
//
//		const float epsilon = 0.001f;
//		for (const auto& ourFBXVertexPosition : fbxVertexPositions)
//		{
//			vertexPositionToColorMap[ourFBXVertexPosition] = { .0f, .0f, .0f };
//		}
//
//		for (unsigned int j = 0; j < colorData.myColors.size(); ++j)
//		{
//			vertexPositionToColorMap[{ -colorData.myVertexPositions[j].x, colorData.myVertexPositions[j].y, -colorData.myVertexPositions[j].z}] = colorData.myColors[j];
//			//vertexPositionToColorMap[{ colorData.myVertexPositions[j].z, colorData.myVertexPositions[j].x, colorData.myVertexPositions[j].y}] = colorData.myColors[j];
//
//		}
//
//		std::vector<Vector3> rgbColorData = {};
//		rgbColorData.resize(fbxVertexPositions.size());
//
//		for (int i = 0; i < fbxVertexPositions.size(); ++i)
//		{
//			rgbColorData[i] = vertexPositionToColorMap[fbxVertexPositions[i]];
//		}
//
//		//for (auto& ourFBXVertexPositionValue : fbxVertexPositions)
//		//{
//		//	rgbColorData.push_back(vertexPositionToColorMap[ourFBXVertexPositionValue]);
//		//}
//
//		if (myVertexColorBuffers.find(colorData.myVertexMeshID) == myVertexColorBuffers.end())
//		{
//			D3D11_BUFFER_DESC vertexColorBufferDesc;
//			vertexColorBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
//			vertexColorBufferDesc.ByteWidth = sizeof(rgbColorData[0]) * static_cast<UINT>(rgbColorData.size());
//
//			vertexColorBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
//			vertexColorBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
//			vertexColorBufferDesc.MiscFlags = 0;
//			vertexColorBufferDesc.StructureByteStride = 0;
//			D3D11_SUBRESOURCE_DATA subResourceData = { 0 };
//			subResourceData.pSysMem = rgbColorData.data();
//
//			ID3D11Buffer* vertexColorBuffer;
//			ENGINE_HR_MESSAGE(CEngine::GetInstance()->myFramework->GetDevice()->CreateBuffer(&vertexColorBufferDesc, &subResourceData, &vertexColorBuffer), "Vertex Color Buffer could not be created.");
//
//			myVertexColorBuffers.emplace(colorData.myVertexMeshID, std::move(vertexColorBuffer));
//			myVertexColorReferences.emplace(colorData.myVertexMeshID, 0);
//		}
//		myVertexColorReferences[colorData.myVertexMeshID] += 1;
//
//		for (const auto& materialName : someMatrials)
//		{
//			RequestMaterial(materialName);
//		}
//		return { someMatrials, colorData.myVertexMeshID };
//	}
//
//	std::vector<DirectX::SimpleMath::Vector3>& CMaterialHandler::GetVertexColors(unsigned int aVertexColorID)
//	{
//		return myVertexColors[aVertexColorID];
//	}
//
//	ID3D11Buffer* CMaterialHandler::GetVertexColorBuffer(unsigned int aVertexColorID)
//	{
//		return myVertexColorBuffers[aVertexColorID];
//	}
//
//	void CMaterialHandler::ReleaseVertexColors(unsigned int aVertexColorID)
//	{
//		if (myVertexColorBuffers.find(aVertexColorID) != myVertexColorBuffers.end())
//		{
//			myVertexColorReferences[aVertexColorID] -= 1;
//
//			if (myVertexColorReferences[aVertexColorID] <= 0)
//			{
//				ULONG remainingRefs = 0;
//				do
//				{
//					remainingRefs = myVertexColorBuffers[aVertexColorID]->Release();
//				} while (remainingRefs > 1);
//
//				myVertexColorBuffers.erase(aVertexColorID);
//				myVertexColorReferences.erase(aVertexColorID);
//			}
//		}
//	}
//
//	const std::array<Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>, 3>& CMaterialHandler::GetMaterial(const int aMaterialID) const
//	{
//		std::string materialName = {};
//		//CJsonReader::Get()->TryGetMaterialName(aMaterialID, materialName);
//		//return myMaterials.at(materialName);
//
//		if (CJsonReader::Get()->TryGetMaterialName(aMaterialID, materialName))
//		{
//			return myMaterials.at(materialName);
//		}
//		else
//		{
//			return myMaterials.at("DefaultMaterial");
//		}
//	}
//
	const SMaterialInstance CMaterialHandler::GetMaterialInstance(const int /*aMaterialID*/) const
	{
		std::string materialName = {};
		//CJsonReader::Get()->TryGetMaterialName(aMaterialID, materialName);
		//return SMaterialInstance{ 5, 3, myMaterials.at(materialName) };

		//if (CJsonReader::Get()->TryGetMaterialName(aMaterialID, materialName))
		//{
		//	return SMaterialInstance{ 5, 3, myMaterials.at(materialName) };
		//}
		//else
		{
			return SMaterialInstance{ 5, 3, myMaterials.at("DefaultMaterial") };
		}
	}
//	const bool CMaterialHandler::IsMaterialAlpha(const int aMaterialID) const
//	{
//		if (myMaterialIsAlphaMap.find(aMaterialID) == myMaterialIsAlphaMap.end())
//		{
//#ifdef _DEBUG
//			//std::string id = std::to_string(aMaterialID);
//			//id.append(": Material ID does not exist in myMaterialIsAlphaMap!");
//			//ENGINE_ERROR_BOOL_MESSAGE(false, id.c_str());
//			//std::cout << __FUNCTION__ << " " << id << std::endl;
//#endif
//			return false;
//		}
//
//		return myMaterialIsAlphaMap.at(aMaterialID);
//	}
//
//	bool CMaterialHandler::Init(CDirectXFramework* aFramwork)
//	{
//		myDevice = aFramwork->GetDevice();
//
//		if (!myDevice)
//		{
//			return false;
//		}
//		material defaultMatInit = RequestMaterial("DefaultMaterial");
//		defaultMatInit[0] = nullptr;
//		defaultMatInit[1] = nullptr;
//		defaultMatInit[2] = nullptr;
//		return true;
//	}
//
//	inline bool CMaterialHandler::MaterialIsAlpha(const std::array<std::string, 3>& someTexturePaths)
//	{
//		for (const auto& materialName : someTexturePaths)
//			if (materialName.size() > 6)
//				if (materialName.substr(materialName.size() - 6, 2) == "AL")
//					return true;
//
//		return false;
//	}
//
//	CMaterialHandler::CMaterialHandler()
//		: myDevice(nullptr)
//		, myMaterialPath(ASSETPATH("Assets/IronWrought/Texture/"))
//		, myDecalPath(ASSETPATH("Assets/IronWrought/Texture/Decal_texture/"))
//		, myVertexLinksPath(ASSETPATH("Assets/Generated/"))
//	{
//	}
//
//	CMaterialHandler::~CMaterialHandler()
//	{
//	}
}