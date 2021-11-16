#pragma once
#include "Core/CoreTypes.h"
#include "EngineMath.h"
#include "Vector.h"
#include "Rotator.h"
#include "Matrix.h"

namespace NewEngine
{
	struct SQuaternion
	{
		F32 X, Y, Z, W;

		static const SQuaternion Identity;

		inline SQuaternion();
		inline SQuaternion(F32 x, F32 y, F32 z, F32 w);
		SQuaternion(const SVector& axis, F32 angleInDegrees);
		explicit SQuaternion(const SMatrix& M);
		explicit SQuaternion(const SRotator& R);
		SQuaternion(const SQuaternion& other) = default;
		~SQuaternion() = default;
	};

	inline SQuaternion::SQuaternion()
		: X(0.0f), Y(0.0f), Z(0.0f), W(1.0f)
	{}

	inline SQuaternion::SQuaternion(F32 x, F32 y, F32 z, F32 w)
		: X(x), Y(y), Z(z), W(w)
	{}

	SQuaternion::SQuaternion(const SVector& axis, F32 angleInDegrees)
	{
		F32 halfAngleInRadians = UMath::DegToRad(angleInDegrees) * 0.5f;
		F32 sine = UMath::Sin(halfAngleInRadians);
		X = axis.X * sine;
		Y = axis.Y * sine;
		Z = axis.Z * sine;
		W = UMath::Cos(halfAngleInRadians);
	}

	//inline SQuaternion::SQuaternion(const SMatrix& M)
	//{
	//}

	//inline SQuaternion::SQuaternion(const SRotator& R)
	//{
	//}
}