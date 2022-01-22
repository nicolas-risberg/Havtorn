// Copyright 2022 Team Havtorn. All Rights Reserved.

#pragma once
#include <array>
#include <map>

class CDirectXFramework;
struct SVertexPaintColorData;
typedef std::array<ID3D11ShaderResourceView*, 3> material;

namespace Havtorn
{
	struct SVertexPaintData
	{
		std::vector<std::string> myRGBMaterialNames;
		I32 myVertexColorID;
	};

	struct SMaterialInstance {

		SMaterialInstance(const U16 aStartSlot, const U16 aNumViews, const std::array<WinComPtr<ID3D11ShaderResourceView>, 3>& someShaderResourceViews)
			: myStartSlot(aStartSlot)
			, myNumViews(aNumViews)
			, myShaderResourceView(someShaderResourceViews)
		{
		}

		const U16 myStartSlot;
		const U16 myNumViews;
		const std::array<WinComPtr<ID3D11ShaderResourceView>, 3>& myShaderResourceView;
	};

	class CMaterialHandler
	{
		friend class CMainSingleton;
		friend class CEngine;

	public:
		std::array<ID3D11ShaderResourceView*, 3> RequestMaterial(const I32 aMaterialID, const std::string& aModelPath);
		std::array<ID3D11ShaderResourceView*, 3> RequestMaterial(const I32 aMaterialID);
		std::array<ID3D11ShaderResourceView*, 3> RequestMaterial(const std::string& aMaterialName);
		std::array<ID3D11ShaderResourceView*, 3> RequestMaterial(const std::string& aMaterialName, I32& anOutMaterialID);
		std::array<ID3D11ShaderResourceView*, 3> RequestDefualtMaterial(I32& anOutMaterialID);

		//std::array<ID3D11ShaderResourceView*, 3> RequestMaterial(const std::string& aMaterialName);
		std::array<ID3D11ShaderResourceView*, 3> RequestDecal(const std::string& aDecalName);
		std::array<ID3D11ShaderResourceView*, 9> GetVertexPaintMaterials(const std::vector<std::string>& someMaterialNames);
		void ReleaseMaterial(const std::string& aMaterialName);

		SVertexPaintData RequestVertexColorID(std::vector<SVertexPaintColorData>::const_iterator& it, const std::string& aFbxModelPath, const std::vector<std::string>& someMatrials);
		std::vector<SVector>& GetVertexColors(I32 aVertexColorID);
		ID3D11Buffer* GetVertexColorBuffer(I32 aVertexColorID);
		void ReleaseVertexColors(I32 aVertexColorID);

		const std::array<WinComPtr<ID3D11ShaderResourceView>, 3>& GetMaterial(const I32 aMaterialID) const;

		const SMaterialInstance GetMaterialInstance(const I32 aMaterialID) const;
		const bool IsMaterialAlpha(const I32 aMaterialID) const;


	protected:
		bool Init(CDirectXFramework* aFramwork);

	private:
		inline bool MaterialIsAlpha(const std::array<std::string, 3>& someTexturePaths);

		std::map<std::string, std::array<WinComPtr<ID3D11ShaderResourceView>, 3>> myMaterials;
		std::map<std::string, std::array<WinComPtr<ID3D11ShaderResourceView>, 9>> myVertexPaintMaterials;

		std::map<I32, bool> myMaterialIsAlphaMap;

		std::map<I32, std::vector<SVector>> myVertexColors;
		std::map<I32, ID3D11Buffer*> myVertexColorBuffers;
		std::map<std::string, U32> myMaterialReferences;
		std::map<I32, U32> myVertexColorReferences;

		ID3D11Device* myDevice;
		const std::string myMaterialPath;
		const std::string myDecalPath;
		const std::string myVertexLinksPath;

	private:
		CMaterialHandler();
		~CMaterialHandler();

	private:
		struct VertexPositionComparer 
		{
			const F32 epsilon = 0.0001f;
			bool operator()(const SVector& a, const SVector& b) const 
			{
				bool xIsEqual = UMath::Abs(a.X - b.X) <= ((UMath::Abs(a.X) < UMath::Abs(b.X) ? UMath::Abs(b.X) : UMath::Abs(a.X)) * epsilon);
				bool yIsEqual = UMath::Abs(a.Y - b.Y) <= ((UMath::Abs(a.Y) < UMath::Abs(b.Y) ? UMath::Abs(b.Y) : UMath::Abs(a.Y)) * epsilon);
				bool zIsEqual = UMath::Abs(a.Z - b.Z) <= ((UMath::Abs(a.Z) < UMath::Abs(b.Z) ? UMath::Abs(b.Z) : UMath::Abs(a.Z)) * epsilon);
				return xIsEqual && yIsEqual && zIsEqual;
			}

			int friend operator<(const SVector& a, const SVector& b) {
				const float epsi = 0.0001f;
				float a0 = a.X;
				float b0 = b.X;

				float a1 = a.Y;
				float b1 = b.Y;

				float a2 = a.Z;
				float b2 = b.Z;

				if ((b0 - a0) > ((UMath::Abs(a0) < UMath::Abs(b0) ? UMath::Abs(b0) : UMath::Abs(a0)) * epsi) &&
					(b1 - a1) > ((UMath::Abs(a1) < UMath::Abs(b1) ? UMath::Abs(b1) : UMath::Abs(a1)) * epsi) &&
					(b2 - a2) > ((UMath::Abs(a2) < UMath::Abs(b2) ? UMath::Abs(b2) : UMath::Abs(a2)) * epsi)) {
					return -1;
				}
				else if ((a0 - b0) > ((UMath::Abs(b0) < UMath::Abs(a0) ? UMath::Abs(a0) : UMath::Abs(b0)) * epsi) &&
					(a1 - b1) > ((UMath::Abs(b1) < UMath::Abs(a1) ? UMath::Abs(a1) : UMath::Abs(b1)) * epsi) &&
					(a2 - b2) > ((UMath::Abs(b2) < UMath::Abs(a2) ? UMath::Abs(a2) : UMath::Abs(b2)) * epsi)) {
					return 1;
				}
				else {
					return 0;
				}
			}
		};

		class VectorHasher {
		public:
			std::size_t operator()(const SVector& aVector) const 
			{
				size_t xHash = std::hash<F32>()(aVector.X);
				size_t yHash = std::hash<F32>()(aVector.Y);
				size_t zHash = std::hash<F32>()(aVector.Z);
				return xHash + yHash + zHash;
			}
		};

	};
}
