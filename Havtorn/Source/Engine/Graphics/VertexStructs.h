// Copyright 2022 Team Havtorn. All Rights Reserved.

#pragma once

namespace Havtorn
{
	struct SStaticMeshVertex
	{
		// Position
		F32 x, y, z, w;

		// Normal
		F32 nx, ny, nz, nw;

		// Tangent
		F32 tx, ty, tz, tw;

		// Bitangent
		F32 bx, by, bz, bw;

		// UV
		F32 u, v;
	};
}