// Copyright 2022 Team Havtorn. All Rights Reserved.

#pragma once
#include "Graphics/GraphicsStructs.h"

namespace Havtorn
{
	template<typename T>
	U32 SerializeSimple(const T& source, char* destination, U32 bufferPosition)
	{
		const U32 size = sizeof(T);
		memcpy(&destination[bufferPosition], &source, size);
		return size;
	}

	template<typename T>
	U32 SerializeVector(const std::vector<T>& source, char* destination, U32 bufferPosition)
	{
		const U32 size = sizeof(T) * static_cast<U32>(source.size());
		memcpy(&destination[bufferPosition], source.data(), size);
		return size;
	}

	inline U32 SerializeString(const std::string& source, char* destination, U32 bufferPosition)
	{
		const U32 size = sizeof(char) * static_cast<U32>(source.length());
		memcpy(&destination[bufferPosition], source.data(), size);
		return size;
	}

	template<typename T>
	U32 DeserializeSimple(T& destination, const char* source, U32 bufferPosition)
	{
		const U32 size = sizeof(T);
		memcpy(&destination, &source[bufferPosition], size);
		return size;
	}

	template<typename T>
	U32 DeserializeVector(std::vector<T>& destination, const char* source, U32 numberOfElements, U32 bufferPosition)
	{
		const U32 size = sizeof(T) * numberOfElements;
		const auto intermediateVector = new T[numberOfElements];
		memcpy(&intermediateVector[0], &source[bufferPosition], size);
		destination.assign(&intermediateVector[0], &intermediateVector[0] + numberOfElements);
		return size;
	}

	inline U32 DeserializeString(std::string& destination, const char* source, U32 numberOfElements, U32 bufferPosition)
	{
		const U32 size = sizeof(char) * numberOfElements;
		destination = std::string(&source[bufferPosition], size);
		return size;
	}

	struct SStaticModelFileHeader
	{
		EAssetType AssetType = EAssetType::StaticModel;
		U32 NameLength = 0;
		std::string Name;
		U32 NumberOfMeshes = 0;
		std::vector<SStaticMesh> Meshes;

		[[nodiscard]] U32 GetSize() const;
		void Serialize(char* toData) const;
		void Deserialize(const char* fromData);
	};

	inline U32 SStaticModelFileHeader::GetSize() const
	{
		U32 size = sizeof(EAssetType);
		size += sizeof(U32);
		size += sizeof(char) * NameLength;
		size += sizeof(U32);

		for (auto& mesh : Meshes)
		{
			size += sizeof(U32);
			size += sizeof(char) * mesh.NameLength;
			size += sizeof(U32);
			size += sizeof(SStaticMeshVertex) * mesh.NumberOfVertices;
			size += sizeof(U32);
			size += sizeof(U32) * mesh.NumberOfIndices;
		}
		return size;
	}

	inline void SStaticModelFileHeader::Serialize(char* toData) const
	{
		U32 pointerPosition = 0;
		pointerPosition += SerializeSimple(AssetType, toData, pointerPosition);
		pointerPosition += SerializeSimple(NumberOfMeshes, toData, pointerPosition);

		for (auto& mesh : Meshes)
		{
			pointerPosition += SerializeSimple(mesh.NameLength, toData, pointerPosition);
			pointerPosition += SerializeString(mesh.Name, toData, pointerPosition);
			pointerPosition += SerializeSimple(mesh.NumberOfVertices, toData, pointerPosition);
			pointerPosition += SerializeVector(mesh.Vertices, toData, pointerPosition);
			pointerPosition += SerializeSimple(mesh.NumberOfIndices, toData, pointerPosition);
			pointerPosition += SerializeVector(mesh.Indices, toData, pointerPosition);
		}
	}

	inline void SStaticModelFileHeader::Deserialize(const char* fromData)
	{
		U32 pointerPosition = 0;
		pointerPosition += DeserializeSimple(AssetType, fromData, pointerPosition);
		pointerPosition += DeserializeSimple(NumberOfMeshes, fromData, pointerPosition);

		Meshes.reserve(NumberOfMeshes);
		for (U16 i = 0; i < NumberOfMeshes; i++)
		{
			Meshes.emplace_back();
			pointerPosition += DeserializeSimple(Meshes.back().NameLength, fromData, pointerPosition);
			pointerPosition += DeserializeString(Meshes.back().Name, fromData, Meshes.back().NameLength, pointerPosition);
			pointerPosition += DeserializeSimple(Meshes.back().NumberOfVertices, fromData, pointerPosition);
			pointerPosition += DeserializeVector(Meshes.back().Vertices, fromData, Meshes.back().NumberOfVertices, pointerPosition);
			pointerPosition += DeserializeSimple(Meshes.back().NumberOfIndices, fromData, pointerPosition);
			pointerPosition += DeserializeVector(Meshes.back().Indices, fromData, Meshes.back().NumberOfIndices, pointerPosition);
		}
	}
}
