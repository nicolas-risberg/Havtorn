// Copyright 2022 Team Havtorn. All Rights Reserved.

#pragma once

namespace Havtorn
{
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
}