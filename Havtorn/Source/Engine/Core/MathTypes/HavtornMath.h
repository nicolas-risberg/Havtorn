// Copyright 2022 Team Havtorn. All Rights Reserved.

// Use this for Math involving more than one MathType
#pragma once

#include "EngineMath.h"
#include "Vector.h"
#include "Quaternion.h"
#include "Matrix.h"

namespace Havtorn
{
	inline SVector SVector4::ToVector3() const
	{
		return { X, Y, Z };
	}

	SVector4 SVector4::operator*=(const SMatrix& other)
	{
		const SVector4 temp = other * (*this);
		(*this) = temp;
		return temp;
	}

	inline SMatrix SMatrix::CreateRotationFromEuler(F32 pitch, F32 yaw, F32 roll)
	{
		const auto quaternion = SQuaternion(pitch, yaw, roll);
		return CreateRotationFromQuaternion(quaternion);
	}

	inline SMatrix SMatrix::CreateRotationFromQuaternion(SQuaternion quaternion)
	{
		SMatrix result;
		const F32 xx2 = 2.0f * quaternion.X * quaternion.X;
		const F32 yy2 = 2.0f * quaternion.Y * quaternion.Y;
		const F32 zz2 = 2.0f * quaternion.Z * quaternion.Z;
		const F32 xy2 = 2.0f * quaternion.X * quaternion.Y;
		const F32 xz2 = 2.0f * quaternion.X * quaternion.Z;
		const F32 xw2 = 2.0f * quaternion.X * quaternion.W;
		const F32 yz2 = 2.0f * quaternion.Y * quaternion.Z;
		const F32 yw2 = 2.0f * quaternion.Y * quaternion.W;
		const F32 zw2 = 2.0f * quaternion.Z * quaternion.W;

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

	inline SMatrix SMatrix::CreateRotationFromAxisAngle(const SVector& axis, F32 angleInRadians)
	{
		const SVector4 normal = SVector4(axis.X, axis.Y, axis.Z, 0.0f);

		// Map Value to y in [-pi,pi], x = 2*pi*quotient + remainder.
		F32 quotient = UMath::TauReciprocal * angleInRadians;
		if (angleInRadians >= 0.0f)
		{
			quotient = static_cast<F32>(static_cast<I32>(quotient + 0.5f));
		}
		else
		{
			quotient = static_cast<F32>(static_cast<I32>(quotient - 0.5f));
		}
		F32 y = angleInRadians - (UMath::Tau * quotient);

		// Map y to [-pi/2,pi/2] with sin(y) = sin(Value).
		F32 sign;
		if (y > UMath::Pi * 0.5f)
		{
			y = UMath::Pi - y;
			sign = -1.0f;
		}
		else if (y < -(UMath::Pi * 0.5f))
		{
			y = -UMath::Pi - y;
			sign = -1.0f;
		}
		else
		{
			sign = +1.0f;
		}

		const F32 y2 = y * y;

		// 11-degree minimax approximation
		const F32 sinAngle = (((((-2.3889859e-08f * y2 + 2.7525562e-06f) * y2 - 0.00019840874f) * y2 + 0.0083333310f) * y2 -
			0.16666667f) * y2 + 1.0f) * y;

		// 10-degree minimax approximation
		F32 p = ((((-2.6051615e-07f * y2 + 2.4760495e-05f) * y2 - 0.0013888378f) * y2 + 0.041666638f) * y2 - 0.5f) * y2 + 1.0f;
		const F32 cosAngle = sign * p;

		const SVector4 a = { sinAngle, cosAngle, 1.0f - cosAngle, 0.0f };

		auto c2 = SVector4(a.Z); c2.W = a.Z;
		auto c1 = SVector4(a.Y); c1.W = a.Y;
		auto c0 = SVector4(a.X); c0.W = a.X;

		const auto n0 = SVector4(normal.Y, normal.Z, normal.X, normal.W);
		const auto n1 = SVector4(normal.Z, normal.X, normal.Y, normal.W);
		
		SVector4 v0 = c2 * n0;
		v0 = v0 * n1;

		SVector4 r0 = c2 * normal;
		r0 = SVector4(r0.X * normal.X + c1.X, r0.Y * normal.Y + c1.Y, r0.Z * normal.Z + c1.Z, r0.W * normal.W + c1.W);

		const auto r1 = SVector4(c0.X * normal.X + v0.X, c0.Y * normal.Y + v0.Y, c0.Z * normal.Z + v0.Z, c0.W * normal.W + v0.W);
		const auto r2 = SVector4(v0.X - c0.X * normal.X, v0.Y - c0.Y * normal.Y, v0.Z - c0.Z * normal.Z, v0.W - c0.W * normal.W);

		v0 = SVector4(r0.X, r0.Y, r0.Z, a.W);
		const auto v1 = SVector4(r1.Z, r2.Y, r2.Z, r1.X);
		const auto v2 = SVector4(r1.Y, r2.X, r1.Y, r2.X);

		SMatrix matrix;
		matrix.Row(0) = SVector4(v0.X, v1.X, v1.Y, v0.W);
		matrix.Row(1) = SVector4(v1.Z, v0.Y, v1.W, v0.W);
		matrix.Row(2) = SVector4(v2.X, v2.Y, v0.Z, v0.W);
		matrix.Row(3) = SVector4(0.0f, 0.0f, 0.0f, 1.0f);
		return matrix;
	}
}
