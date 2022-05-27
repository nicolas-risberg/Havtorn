// Copyright 2022 Team Havtorn. All Rights Reserved.

#pragma once

namespace Havtorn
{
	struct SPositionVertex
	{
		F32 x, y, z, w;
	};

	struct SStaticMeshVertex
	{
		// Position
		F32 x, y, z;

		// Normal
		F32 nx, ny, nz;

		// Tangent
		F32 tx, ty, tz;

		// Bitangent
		F32 bx, by, bz;

		// UV
		F32 u, v;
	};

	struct SStaticMesh
	{
		U32 NameLength = 0;
		std::string Name;
		U32 NumberOfVertices = 0;
		std::vector<SStaticMeshVertex> Vertices;
		U32 NumberOfIndices = 0;
		std::vector<U32> Indices;
	};

	struct SDrawCallData
	{
		U32 IndexCount = 0;
		U16 VertexBufferIndex = 0;
		U16 IndexBufferIndex = 0;
		U16 VertexStrideIndex = 0;
		U16 VertexOffsetIndex = 0;
	};

	struct SShadowmapViewData
	{
		SMatrix ShadowViewMatrix = SMatrix::Identity;
		SMatrix ShadowProjectionMatrix = SMatrix::Identity;

		// TODO.NR: Replace with transform translation
		SVector4 ShadowPosition = SVector4::Zero;

		U16 ShadowmapViewportIndex = 0;
	};
}