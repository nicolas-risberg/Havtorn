// Copyright 2022 Team Havtorn. All Rights Reserved.

#pragma once

#include "Core/Core.h"
#include "Core/CoreTypes.h"
#include "EngineMath.h"
#include "Vector.h"
#include "Rotator.h"
#include "Matrix.h"

namespace Havtorn
{
	struct SQuaternion
	{
		F32 X, Y, Z, W;

		static const SQuaternion Identity;

		inline SQuaternion();
		inline SQuaternion(F32 x, F32 y, F32 z, F32 w);
		inline SQuaternion(F32 pitch, F32 yaw, F32 roll);
		inline SQuaternion(const SVector& axis, F32 angleInDegrees);
		explicit SQuaternion(const SMatrix& M);
		//explicit SQuaternion(const SRotator& R);
		inline F32& operator[](U8 index);
		inline const F32& operator[](U8 index) const;
		SQuaternion(const SQuaternion& other) = default;
		~SQuaternion() = default;
	};

	inline SQuaternion::SQuaternion()
		: X(0.0f), Y(0.0f), Z(0.0f), W(1.0f)
	{}

	inline SQuaternion::SQuaternion(F32 x, F32 y, F32 z, F32 w)
		: X(x), Y(y), Z(z), W(w)
	{}

	inline SQuaternion::SQuaternion(F32 pitch, F32 yaw, F32 roll)
	{
		F32 cosYaw = UMath::Cos(yaw * 0.5f);
		F32 sinYaw = UMath::Sin(yaw * 0.5f);
		F32 cosPitch = UMath::Cos(pitch * 0.5f);
		F32 sinPitch = UMath::Sin(pitch * 0.5f);
		F32 cosRoll = UMath::Cos(roll * 0.5f);
		F32 sinRoll = UMath::Sin(roll * 0.5f);

		X = cosRoll * sinPitch * cosYaw + sinRoll * cosPitch * sinYaw;
		Y = cosRoll * cosPitch * sinYaw - sinRoll * sinPitch * cosYaw;
		Z = sinRoll * cosPitch * cosYaw - cosRoll * sinPitch * sinYaw;
		W = cosRoll * cosPitch * cosYaw + sinRoll * sinPitch * sinYaw;
	}

	SQuaternion::SQuaternion(const SVector& axis, F32 angleInDegrees)
	{
		F32 halfAngleInRadians = UMath::DegToRad(angleInDegrees) * 0.5f;
		F32 sine = UMath::Sin(halfAngleInRadians);
		X = axis.X * sine;
		Y = axis.Y * sine;
		Z = axis.Z * sine;
		W = UMath::Cos(halfAngleInRadians);
	}

	inline SQuaternion::SQuaternion(const SMatrix& M)
	{
		F32 trace = M.GetRotationMatrixTrace();

		if (trace > 0.0f)
		{
			F32 s = UMath::Sqrt(trace + 1.0f);
			W = 0.5f * s;

			F32 t = 0.5f / s;
			X = (M(2, 1) - M(1, 2)) * t;
			Y = (M(0, 2) - M(2, 0)) * t;
			Z = (M(1, 0) - M(0, 1)) * t;
		}
		else
		{
			U8 i = 0;
			if (M(1, 1) > M(0, 0)) i = 1;
			if (M(2, 2) > M(i, i)) i = 2;

			static const U8 next[3] = { 1, 2, 0 };
			U8 j = next[i];
			U8 k = next[j];

			F32 s = UMath::Sqrt((M(i, j) - (M(j, j) + M(k, k))) + 1.0f);
			this->operator[](i) = 0.5f * s;

			F32 t = (s != 0.0f) ? 0.5f / s : s;

			this->operator[](3) = (M(k, j) - M(j, k)) * t;
			this->operator[](j) = (M(j, i) + M(i, j)) * t;
			this->operator[](k) = (M(k, i) + M(i, k)) * t;
		}
	}

	inline F32& SQuaternion::operator[](U8 index)
	{
		assert((index >= 0 && index < 4));
		
		switch (index)
		{
		case 0:
			return X;
		case 1:
			return Y;
		case 2:
			return Z;
		default:
			return W;
		}
	}

	inline const F32& SQuaternion::operator[](U8 index) const
	{
		assert((index >= 0 && index < 4));

		switch (index)
		{
		case 0:
			return X;
		case 1:
			return Y;
		case 2:
			return Z;
		default:
			return W;
		}
	}

	//inline SQuaternion::SQuaternion(const SRotator& R)
	//{
	//}
}