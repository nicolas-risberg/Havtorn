#pragma once

#include "Core/CoreTypes.h"
#include "EngineMath.h"

namespace Havtorn
{
	struct SMatrix
	{
		union { __declspec(align(16)) F32 M[4][4]; };
	};
}