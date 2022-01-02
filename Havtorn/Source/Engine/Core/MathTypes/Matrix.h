#pragma once

#include "Core/CoreTypes.h"
#include "EngineMath.h"
#include "Vector.h"
#include "Quaternion.h"

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
		// Negates rotation and negates + rotates translation
		inline SMatrix FastInverse() const;
		
		inline SMatrix RotationMatrix() const;
		inline SMatrix TranslationMatrix() const;

		SMatrix operator+(const SMatrix& matrix);
		SMatrix& operator+=(const SMatrix& matrix);
		SMatrix operator-(const SMatrix& matrix);
		SMatrix& operator-=(const SMatrix& matrix);
		SMatrix operator*(const SMatrix& matrix);
		SMatrix& operator*=(const SMatrix& matrix);
		SVector4 operator*(const SVector4& vector);
		SMatrix& operator=(const SMatrix& matrix);
		SMatrix operator*(F32 scalar);
		SMatrix& operator*=(F32 scalar);
		friend SVector4 operator*(SVector4 vector, SMatrix matrix);

		inline SVector4 Row(U8 index) const;
		inline SVector4 Column(U8 index) const;

		inline SVector Up() const;
		inline SVector Down() const;
		inline SVector Right() const;
		inline SVector Left() const;
		inline SVector Forward() const;
		inline SVector Backward() const;
		inline SVector Translation() const;

		inline void Up(const SVector& v);
		inline void Down(const SVector& v);
		inline void Right(const SVector& v);
		inline void Left(const SVector& v);
		inline void Forward(const SVector& v);
		inline void Backward(const SVector& v);
		inline void Translation(const SVector& v);
		inline void Translation(const SVector4& v);

		bool operator==(const SMatrix& matrix) const;

		SMatrix PerspectiveFovLH(F32 fovAngleY, F32 aspectRatio, F32 nearZ, F32 farZ);
	};
}
