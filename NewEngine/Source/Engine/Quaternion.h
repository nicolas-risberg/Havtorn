#pragma once
#include "CoreTypes.h"
#include "EngineMath.h"
#include "Vector.h"

namespace NewEngine
{
	struct SQuaternion
	{
		F32 X, Y, Z, W;

		static const SQuaternion Right;
		static const SQuaternion Up;
		static const SQuaternion Forward;
					 
		static const SQuaternion Left;
		static const SQuaternion Down;
		static const SQuaternion Backward;

		inline SQuaternion();
		inline SQuaternion(F32 x, F32 y, F32 z, F32 w);
		SQuaternion(const SVector& axis, F32 angleInDegrees);
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
}