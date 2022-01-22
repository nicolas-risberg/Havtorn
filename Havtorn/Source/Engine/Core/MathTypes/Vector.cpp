// Copyright 2022 Team Havtorn. All Rights Reserved.

#include "Vector.h"

namespace Havtorn
{
	const SVector SVector::Zero = SVector(0, 0, 0);

	const SVector SVector::Right = SVector(1, 0, 0);
	const SVector SVector::Up = SVector(0, 1, 0);
	const SVector SVector::Forward = SVector(0, 0, 1);

	const SVector SVector::Left = SVector(-1, 0, 0);;
	const SVector SVector::Down = SVector(0, -1, 0);
	const SVector SVector::Backward = SVector(0, 0, -1);
}

namespace Havtorn
{
	const SVector4 SVector4::Zero = SVector4(0, 0, 0, 0);

	const SVector4 SVector4::Right = SVector4(1, 0, 0, 0);
	const SVector4 SVector4::Up = SVector4(0, 1, 0, 0);
	const SVector4 SVector4::Forward = SVector4(0, 0, 1, 0);

	const SVector4 SVector4::Left = SVector4(-1, 0, 0, 0);
	const SVector4 SVector4::Down = SVector4(0, -1, 0, 0);
	const SVector4 SVector4::Backward = SVector4(0, 0, -1, 0);
}
