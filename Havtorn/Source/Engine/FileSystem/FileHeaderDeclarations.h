// Copyright 2022 Team Havtorn. All Rights Reserved.

#pragma once
#include "Graphics/VertexStructs.h"

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

	struct SStaticMeshFileHeader
	{
		EAssetType AssetType = EAssetType::StaticMesh;
		U32 NameLength = 0;
		std::string Name;
		U32 NumberOfVertices = 0;
		std::vector<SStaticMeshVertex> Vertices;
		U32 NumberOfIndices = 0;
		std::vector<U32> Indices;

		[[nodiscard]] U32 GetSize() const;
		void Serialize(char* toData) const;
		void Deserialize(const char* fromData);
	};

	inline U32 SStaticMeshFileHeader::GetSize() const
	{
		U32 size = sizeof(EAssetType);
		size += sizeof(U32);
		size += sizeof(char) * NameLength;
		size += sizeof(U32);
		size += sizeof(SStaticMeshVertex) * NumberOfVertices;
		size += sizeof(U32);
		size += sizeof(U32) * NumberOfIndices;
		return size;
	}

	inline void SStaticMeshFileHeader::Serialize(char* toData) const
	{
		U32 pointerPosition = 0;
		pointerPosition += SerializeSimple(AssetType, toData, pointerPosition);
		pointerPosition += SerializeSimple(NameLength, toData, pointerPosition);
		pointerPosition += SerializeString(Name, toData, pointerPosition);
		pointerPosition += SerializeSimple(NumberOfVertices, toData, pointerPosition);
		pointerPosition += SerializeVector(Vertices, toData, pointerPosition);
		pointerPosition += SerializeSimple(NumberOfIndices, toData, pointerPosition);
		SerializeVector(Indices, toData, pointerPosition);
	}

	inline void SStaticMeshFileHeader::Deserialize(const char* fromData)
	{
		U32 pointerPosition = 0;
		pointerPosition += DeserializeSimple(AssetType, fromData, pointerPosition);
		pointerPosition += DeserializeSimple(NameLength, fromData, pointerPosition);
		pointerPosition += DeserializeString(Name, fromData, NameLength, pointerPosition);
		pointerPosition += DeserializeSimple(NumberOfVertices, fromData, pointerPosition);
		pointerPosition += DeserializeVector(Vertices, fromData, NumberOfVertices, pointerPosition);
		pointerPosition += DeserializeSimple(NumberOfIndices, fromData, pointerPosition);
		DeserializeVector(Indices, fromData, NumberOfIndices, pointerPosition);
	}
}
