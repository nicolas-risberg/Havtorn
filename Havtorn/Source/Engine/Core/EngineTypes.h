// Copyright 2022 Team Havtorn. All Rights Reserved.

#pragma once

#include "CoreTypes.h"
#include "MathTypes/Vector.h"
#include "MathTypes/Quaternion.h"
#include "MathTypes/Matrix.h"
#include "MathTypes/Transform.h"
#include "MathTypes/Curve.h"

namespace Havtorn
{
	enum class EAssetType
	{
		None,
		StaticMesh,
		SkeletalMesh,
		Texture,
		Animation,
		AudioOneShot,
		AudioCollection,
		VisualFX
	};
}