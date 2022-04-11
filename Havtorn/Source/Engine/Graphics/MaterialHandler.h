// Copyright 2022 Team Havtorn. All Rights Reserved.

#pragma once
#include <array>
#include <map>


namespace Havtorn
{
	class CGraphicsFramework;
	struct SVertexPaintColorData;

	struct SVertexPaintData
	{
		std::vector<std::string> RGBMaterialNames;
		I32 VertexColorID;
	};

	struct SMaterialInstance
	{
		SMaterialInstance(const U16 startSlot, const U16 numViews, const std::array<WinComPtr<ID3D11ShaderResourceView>, 3>& shaderResourceViews)
			: StartSlot(startSlot)
			, NumViews(numViews)
			, ShaderResourceViews(shaderResourceViews)
		{
		}

		const U16 StartSlot;
		const U16 NumViews;
		const std::array<WinComPtr<ID3D11ShaderResourceView>, 3>& ShaderResourceViews;
	};

	class CMaterialHandler
	{
		friend class CEngine;

	public:
		std::array<ID3D11ShaderResourceView*, 3> RequestMaterial(const I32 materialID, const std::string& modelPath);
		std::array<ID3D11ShaderResourceView*, 3> RequestMaterial(const I32 materialID);
		//std::array<ID3D11ShaderResourceView*, 3> RequestMaterial(const std::string& materialName);
		std::array<ID3D11ShaderResourceView*, 3> RequestMaterial(const std::string& materialName, I32& outMaterialID);
		std::array<ID3D11ShaderResourceView*, 3> RequestDefaultMaterial(I32& outMaterialID);

		std::array<ID3D11ShaderResourceView*, 3> RequestMaterial(const std::string& materialName);
		std::array<ID3D11ShaderResourceView*, 3> RequestDecal(const std::string& decalName);
		std::array<ID3D11ShaderResourceView*, 9> GetVertexPaintMaterials(const std::vector<std::string>& materialNames);
		void ReleaseMaterial(const std::string& materialName);

		SVertexPaintData RequestVertexColorID(const std::vector<SVertexPaintColorData>::const_iterator& it, const std::string& modelPath, const std::vector<std::string>& materials);
		const std::vector<SVector>& GetVertexColors(I32 vertexColorID);
		ID3D11Buffer* GetVertexColorBuffer(I32 vertexColorID);
		void ReleaseVertexColors(I32 vertexColorID);

		const std::array<WinComPtr<ID3D11ShaderResourceView>, 3>& GetMaterial(const I32 materialID) const;

		SMaterialInstance GetMaterialInstance(I32 materialID) const;
		bool IsMaterialTransparent(I32 materialID) const;

		bool TryGetMaterialName(const I32 aMaterialID, std::string& outMaterialName) const;
		bool TryGetMaterialsPath(const I32 aMaterialID, std::array<std::string, 3>& outTexturePaths) const;
		bool TryGetMaterialID(const std::string& aMaterialName, I32& outMaterialID) const;

	protected:
		bool Init(const CGraphicsFramework* framework);

	private:
		inline bool IsMaterialTransparent(const std::array<std::string, 3>& texturePaths);

		std::unordered_map<I32, std::string> PathsMap;
		std::unordered_map<I32, std::string> MaterialsNameMap;
		std::unordered_map<I32, std::vector<I32>> MaterialsMap;
		std::unordered_map<std::string, I32> MaterialsIDMap;
		std::map<I32, bool> TransparentMaterialMap;
		std::map<std::string, std::array<WinComPtr<ID3D11ShaderResourceView>, 3>> Materials;
		std::map<std::string, std::array<WinComPtr<ID3D11ShaderResourceView>, 9>> VertexPaintMaterials;

		std::map<I32, std::vector<SVector>> VertexColors;
		std::map<I32, ID3D11Buffer*> VertexColorBuffers;
		std::map<std::string, U32> MaterialReferences;
		std::map<I32, U32> VertexColorReferences;

		ID3D11Device* Device;
		const std::string MaterialPath;
		const std::string DecalPath;
		const std::string VertexLinksPath;

	private:
		CMaterialHandler();
		~CMaterialHandler() = default;

	private:
		struct VertexPositionComparer 
		{
			const F32 epsilon = 0.0001f;
			bool operator()(const SVector& a, const SVector& b) const 
			{
				const bool xIsEqual = UMath::Abs(a.X - b.X) <= ((UMath::Abs(a.X) < UMath::Abs(b.X) ? UMath::Abs(b.X) : UMath::Abs(a.X)) * epsilon);
				const bool yIsEqual = UMath::Abs(a.Y - b.Y) <= ((UMath::Abs(a.Y) < UMath::Abs(b.Y) ? UMath::Abs(b.Y) : UMath::Abs(a.Y)) * epsilon);
				const bool zIsEqual = UMath::Abs(a.Z - b.Z) <= ((UMath::Abs(a.Z) < UMath::Abs(b.Z) ? UMath::Abs(b.Z) : UMath::Abs(a.Z)) * epsilon);
				return xIsEqual && yIsEqual && zIsEqual;
			}

			int friend operator<(const SVector& a, const SVector& b)
			{
				constexpr F32 epsilon = 0.0001f;
				const F32 a0 = a.X;
				const F32 b0 = b.X;

				const F32 a1 = a.Y;
				const F32 b1 = b.Y;

				const F32 a2 = a.Z;
				const F32 b2 = b.Z;

				if ((b0 - a0) > ((UMath::Abs(a0) < UMath::Abs(b0) ? UMath::Abs(b0) : UMath::Abs(a0)) * epsilon) &&
					(b1 - a1) > ((UMath::Abs(a1) < UMath::Abs(b1) ? UMath::Abs(b1) : UMath::Abs(a1)) * epsilon) &&
					(b2 - a2) > ((UMath::Abs(a2) < UMath::Abs(b2) ? UMath::Abs(b2) : UMath::Abs(a2)) * epsilon)) {
					return -1;
				}
				if ((a0 - b0) > ((UMath::Abs(b0) < UMath::Abs(a0) ? UMath::Abs(a0) : UMath::Abs(b0)) * epsilon) &&
					(a1 - b1) > ((UMath::Abs(b1) < UMath::Abs(a1) ? UMath::Abs(a1) : UMath::Abs(b1)) * epsilon) &&
					(a2 - b2) > ((UMath::Abs(b2) < UMath::Abs(a2) ? UMath::Abs(a2) : UMath::Abs(b2)) * epsilon)) {
					return 1;
				}

				return 0;
			}
		};

		struct SVectorHasher
		{
			std::size_t operator()(const SVector& vector) const 
			{
				const size_t xHash = std::hash<F32>()(vector.X);
				const size_t yHash = std::hash<F32>()(vector.Y);
				const size_t zHash = std::hash<F32>()(vector.Z);
				return xHash + yHash + zHash;
			}
		};

	};
}
