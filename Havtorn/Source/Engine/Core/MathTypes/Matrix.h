#pragma once

#include "Core/CoreTypes.h"
#include "EngineMath.h"
#include "Vector.h"

namespace Havtorn 
{
	// Left-handed row-major
	struct SMatrix {
		
		union { __declspec(align(16)) F32 M[4][4]; F32 data[16]; };

		static const SMatrix Identity;
		static const SMatrix Zero;

		// Creates the identity matrix.
		SMatrix();
		// Copy Constructor.
		SMatrix(const SMatrix& matrix);
		// () operator for accessing element (row, column) for read/write or read, respectively.
		F32& operator()(const U8 row, const U8 column);
		const F32& operator()(const U8 row, const U8 column) const;
		// Static functions for creating rotation matrices.
		static SMatrix CreateRotationAroundX(F32 angleInRadians);
		static SMatrix CreateRotationAroundY(F32 angleInRadians);
		static SMatrix CreateRotationAroundZ(F32 angleInRadians);
		// Static function for creating a transpose of a matrix.
		static SMatrix Transpose(const SMatrix& matrixToTranspose);
		SMatrix operator+(const SMatrix& matrix);
		SMatrix &operator+=(const SMatrix& matrix);
		SMatrix operator-(const SMatrix& matrix);
		SMatrix &operator-=(const SMatrix& matrix);
		SMatrix operator*(const SMatrix& matrix);
		SMatrix &operator*=(const SMatrix& matrix);
		SVector4 operator*(const SVector4& vector);
		SMatrix &operator=(const SMatrix& matrix);
		bool operator==(const SMatrix& matrix) const;

		SMatrix SMatrix::PerspectiveFovLH(F32 fovAngleY, F32 aspectRatio, F32 nearZ, F32 farZ);
	};
}
