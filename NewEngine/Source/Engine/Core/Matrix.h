#pragma once
#include "CoreTypes.h"
#include "EngineMath.h"

namespace NewEngine
{
	struct SMatrix
	{
		union { __declspec(align(16)) F32 M[4][4]; };
	};
}