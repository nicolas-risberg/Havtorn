// Copyright 2022 Team Havtorn. All Rights Reserved.

// Use this for Math involving more than one MathType
#pragma once

#include "EngineMath.h"
#include "Vector.h"
#include "Quaternion.h"
#include "Matrix.h"

namespace Havtorn
{
	SVector4 SVector4::operator*=(const SMatrix& other)
	{
		SVector4 temp = other * (*this);
		(*this) = temp;
		return temp;
	}

	inline SMatrix SMatrix::CreateRotationFromEuler(F32 pitch, F32 yaw, F32 roll)
	{
		SQuaternion quaternion = SQuaternion(pitch, yaw, roll);
		return CreateRotationFromQuaternion(quaternion);
	}

	inline SMatrix SMatrix::CreateRotationFromQuaternion(SQuaternion quaternion)
	{
		SMatrix result;
		F32 xx2 = 2.0f * quaternion.X * quaternion.X;
		F32 yy2 = 2.0f * quaternion.Y * quaternion.Y;
		F32 zz2 = 2.0f * quaternion.Z * quaternion.Z;
		F32 xy2 = 2.0f * quaternion.X * quaternion.Y;
		F32 xz2 = 2.0f * quaternion.X * quaternion.Z;
		F32 xw2 = 2.0f * quaternion.X * quaternion.W;
		F32 yz2 = 2.0f * quaternion.Y * quaternion.Z;
		F32 yw2 = 2.0f * quaternion.Y * quaternion.W;
		F32 zw2 = 2.0f * quaternion.Z * quaternion.W;

		result(0, 0) = 1.0f - yy2 - zz2;
		result(0, 1) = xy2 + zw2;
		result(0, 2) = xz2 - yw2;

		result(1, 0) = xy2 - zw2;
		result(1, 1) = 1.0f - xx2 - zz2;
		result(1, 2) = yz2 + xw2;

		result(2, 0) = xz2 + yw2;
		result(2, 1) = yz2 - xw2;
		result(2, 2) = 1.0f - xx2 - yy2;
		return result;
	}
}