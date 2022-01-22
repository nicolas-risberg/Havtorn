// Copyright 2022 Team Havtorn. All Rights Reserved.

#pragma once

#include "Core/CoreTypes.h"
#include "EngineMath.h"

namespace Havtorn 
{
	struct SVector;
	struct SVector4;
	struct SQuaternion;

	// Left-handed row-major
	struct SMatrix 
	{	
		union { __declspec(align(16)) F32 M[4][4]; F32 data[16]; };

		static const SMatrix Identity;
		static const SMatrix Zero;

		// Creates the identity matrix.
		inline SMatrix();
		// Copy Constructor.
		inline SMatrix(F32 diagonalElement);
		inline SMatrix(const SMatrix& matrix);
		// () operator for accessing element (row, column) for read/write or read, respectively.
		inline F32& operator()(const U8 row, const U8 column);
		inline const F32& operator()(const U8 row, const U8 column) const;
		// Static functions for creating rotation matrices.
		static SMatrix CreateRotationAroundX(F32 angleInRadians);
		static SMatrix CreateRotationAroundY(F32 angleInRadians);
		static SMatrix CreateRotationAroundZ(F32 angleInRadians);
		static SMatrix CreateRotationAroundAxis(F32 angleInRadians, SVector axis);
		static SMatrix CreateRotationFromEuler(F32 pitch, F32 yaw, F32 roll);
		static SMatrix CreateRotationFromQuaternion(SQuaternion quaternion);
		// Static function for creating a transpose of a matrix.
		static SMatrix Transpose(const SMatrix& matrixToTranspose);
		// Negates rotation and negates + rotates translation
		inline SMatrix FastInverse() const;
		// Based on XMMatrixInverse
		inline SMatrix Inverse() const;
		
		inline SMatrix GetRotationMatrix() const;
		inline void SetRotation(SMatrix matrix);
		inline SMatrix GetTranslationMatrix() const;
		inline F32 GetRotationMatrixTrace() const;

		inline SMatrix operator+(const SMatrix& matrix);
		inline SMatrix& operator+=(const SMatrix& matrix);
		inline SMatrix operator-(const SMatrix& matrix);
		inline SMatrix& operator-=(const SMatrix& matrix);
		inline SMatrix operator*(const SMatrix& matrix);
		inline SMatrix& operator*=(const SMatrix& matrix);
		inline SVector4 operator*(const SVector4& vector);
		inline SMatrix& operator=(const SMatrix& matrix);
		inline SMatrix operator*(F32 scalar);
		inline SMatrix& operator*=(F32 scalar);
		inline friend SVector4 operator*(SVector4 vector, SMatrix matrix);

		inline SVector4 Row(U8 index) const;
		inline SVector4 Column(U8 index) const;
		inline void Row(U8 index, const SVector4& vector);
		inline void Column(U8 index, const SVector4& vector);

		inline SVector Up() const;
		inline SVector Down() const;
		inline SVector Right() const;
		inline SVector Left() const;
		inline SVector Forward() const;
		inline SVector Backward() const;
		inline SVector Translation() const;
		inline SVector4 Translation4() const;

		inline void Up(const SVector& v);
		inline void Down(const SVector& v);
		inline void Right(const SVector& v);
		inline void Left(const SVector& v);
		inline void Forward(const SVector& v);
		inline void Backward(const SVector& v);
		inline void Translation(const SVector& v);
		inline void Translation(const SVector4& v);
		inline void Rotate(const SMatrix& rotationMatrix);
		inline void Rotate(const SVector& eulerAngles);
		inline void Translate(const SVector& v);
		inline void Translate(const SVector4& v);
		inline void Orbit(const SVector& point, const SMatrix& rotation);
		inline void Orbit(const SVector4& point, const SMatrix& rotation);

		inline bool operator==(const SMatrix& matrix) const;

		static SMatrix PerspectiveFovLH(F32 fovAngleY, F32 aspectRatio, F32 nearZ, F32 farZ);
		static SMatrix LookAtLH(SVector eyePosition, SVector focusPosition, SVector upDirection);
		static SMatrix LookToLH(SVector eyePosition, SVector eyeDirection, SVector upDirection);
	};

	SMatrix::SMatrix()
	{
		for (U8 row = 0; row < 4; ++row)
		{
			for (U8 column = 0; column < 4; ++column)
			{
				M[row][column] = (row == column) ? 1.0f : 0.0f;
			}
		}
	}

	SMatrix::SMatrix(F32 diagonalElement)
	{
		for (U8 row = 0; row < 4; ++row)
		{
			for (U8 column = 0; column < 4; ++column)
			{
				M[row][column] = (row == column) ? diagonalElement : 0.0f;
			}
		}
	}

	SMatrix::SMatrix(const SMatrix& matrix)
	{
		for (U8 i = 0; i < 16; ++i)
		{
			data[i] = matrix.data[i];
		}
	}

	F32& SMatrix::operator()(const U8 row, const U8 column)
	{
		return M[row][column];
	}


	const F32& SMatrix::operator()(const U8 row, const U8 column) const
	{
		return M[row][column];
	}

	inline SMatrix SMatrix::CreateRotationAroundX(F32 angleInRadians)
	{
		SMatrix matrix = SMatrix();
		matrix(1, 1) = UMath::Cos(angleInRadians);
		matrix(1, 2) = UMath::Sin(angleInRadians);
		matrix(2, 1) = -UMath::Sin(angleInRadians);
		matrix(2, 2) = UMath::Cos(angleInRadians);
		return matrix;
	}

	inline SMatrix SMatrix::CreateRotationAroundY(F32 angleInRadians)
	{
		SMatrix matrix = SMatrix();
		matrix(0, 0) = UMath::Cos(angleInRadians);
		matrix(0, 2) = -UMath::Sin(angleInRadians);
		matrix(2, 0) = UMath::Sin(angleInRadians);
		matrix(2, 2) = UMath::Cos(angleInRadians);
		return matrix;
	}

	inline SMatrix SMatrix::CreateRotationAroundZ(F32 angleInRadians)
	{
		SMatrix matrix = SMatrix();
		matrix(0, 0) = UMath::Cos(angleInRadians);
		matrix(0, 1) = UMath::Sin(angleInRadians);
		matrix(1, 0) = -UMath::Sin(angleInRadians);
		matrix(1, 1) = UMath::Cos(angleInRadians);
		return matrix;
	}

	inline SMatrix SMatrix::CreateRotationAroundAxis(F32 angleInRadians, SVector axis)
	{
		F32 cosTerm = UMath::Cos(angleInRadians);
		F32 sinTerm = UMath::Sin(angleInRadians);
		F32 oneMinusCos = 1.0f - cosTerm;
		F32 x2 = axis.X * axis.X;
		F32 y2 = axis.Y * axis.Y;
		F32 z2 = axis.Z * axis.Z;
		F32 xy = axis.X * axis.Y;
		F32 xz = axis.X * axis.Z;
		F32 yz = axis.Y * axis.Z;

		SMatrix matrix;
		matrix(0, 0) = cosTerm + (x2 * oneMinusCos);
		matrix(0, 1) = (xy * oneMinusCos) - (axis.Z * sinTerm);
		matrix(0, 2) = (xz * oneMinusCos) + (axis.Y * sinTerm);
		matrix(1, 0) = (xy * oneMinusCos) + (axis.Z * sinTerm);
		matrix(1, 1) = cosTerm + (y2 * oneMinusCos);
		matrix(1, 2) = (yz * oneMinusCos) - (axis.X * sinTerm);
		matrix(2, 0) = (xz * oneMinusCos) - (axis.Y * sinTerm);
		matrix(2, 1) = (yz * oneMinusCos) + (axis.X * sinTerm);
		matrix(2, 2) = cosTerm + (z2 * oneMinusCos);
		return matrix;
	}

	// Only use this on matrices representing a 3x3 rotation and 3x1 translation
	SMatrix SMatrix::FastInverse() const
	{
		SMatrix rotation = Transpose(this->GetRotationMatrix());
		SVector4 translation = SVector4(this->Translation(), 1.0f);
		translation *= -1.0f;
		translation = translation * rotation;

		SMatrix result;
		result.SetRotation(rotation);
		result.Translation(translation);
		return result;
	}

	SMatrix SMatrix::GetRotationMatrix() const
	{
		SMatrix rotationMatrix = *this;
		rotationMatrix.Translation(SVector::Zero);
		return rotationMatrix;
	}

	inline void SMatrix::SetRotation(SMatrix matrix)
	{
		for (U8 row = 0; row < 3; ++row)
		{
			for (U8 column = 0; column < 3; ++column)
			{
				M[row][column] = matrix(row, column);
			}
		}
	}

	SMatrix SMatrix::GetTranslationMatrix() const
	{
		SMatrix translationMatrix = SMatrix();
		translationMatrix.Translation(this->Translation());
		return translationMatrix;
	}

	inline F32 SMatrix::GetRotationMatrixTrace() const
	{
		return M[0][0] + M[1][1] + M[2][2];
	}

	SMatrix SMatrix::operator+(const SMatrix& matrix)
	{
		SMatrix result = SMatrix();
		for (U8 row = 0; row < 4; ++row)
		{
			for (U8 column = 0; column < 4; ++column)
			{
				result(row, column) = M[row][column] + matrix(row, column);
			}
		}
		return result;
	}


	SMatrix& SMatrix::operator+=(const SMatrix& matrix)
	{
		for (U8 row = 0; row < 4; ++row)
		{
			for (U8 column = 0; column < 4; ++column)
			{
				M[row][column] += matrix(row, column);
			}
		}
		return *this;
	}


	SMatrix SMatrix::operator-(const SMatrix& matrix)
	{
		SMatrix result = SMatrix();
		for (U8 row = 0; row < 4; ++row)
		{
			for (U8 column = 0; column < 4; ++column)
			{
				result(row, column) = M[row][column] - matrix(row, column);
			}
		}
		return result;
	}

	SMatrix& SMatrix::operator-=(const SMatrix& matrix)
	{
		for (U8 row = 0; row < 4; ++row)
		{
			for (U8 column = 0; column < 4; ++column)
			{
				M[row][column] -= matrix(row, column);
			}
		}
		return *this;
	}


	SMatrix SMatrix::operator*(const SMatrix& matrix)
	{
		SMatrix result = SMatrix();
		for (U8 row = 0; row < 4; ++row)
		{
			for (U8 column = 0; column < 4; ++column)
			{
				result(row, column) = M[row][0] * matrix(0, column);
				result(row, column) += M[row][1] * matrix(1, column);
				result(row, column) += M[row][2] * matrix(2, column);
				result(row, column) += M[row][3] * matrix(3, column);
			}
		}
		return result;
	}


	SMatrix& SMatrix::operator*=(const SMatrix& matrix)
	{
		SMatrix result = SMatrix();
		for (U8 row = 0; row < 4; ++row)
		{
			for (U8 column = 0; column < 4; ++column)
			{
				result(row, column) = M[row][0] * matrix(0, column);
				result(row, column) += M[row][1] * matrix(1, column);
				result(row, column) += M[row][2] * matrix(2, column);
				result(row, column) += M[row][3] * matrix(3, column);
			}
		}
		*this = result;
		return *this;
	}

	SVector4 SMatrix::operator*(const SVector4& vector)
	{
		SVector4 result;
		SVector4 temp1 = SVector4(M[0][0], M[1][0], M[2][0], M[3][0]);
		SVector4 temp2 = SVector4(M[0][1], M[1][1], M[2][1], M[3][1]);
		SVector4 temp3 = SVector4(M[0][2], M[1][2], M[2][2], M[3][2]);
		SVector4 temp4 = SVector4(M[0][3], M[1][3], M[2][3], M[3][3]);
		result.X = vector.Dot(temp1);
		result.Y = vector.Dot(temp2);
		result.Z = vector.Dot(temp3);
		result.W = vector.Dot(temp4);
		return result;
	}

	SVector4 operator*(SMatrix matrix, SVector4 vector)
	{
		SVector4 result;
		SVector4 temp1 = SVector4(matrix(0, 0), matrix(1, 0), matrix(2, 0), matrix(3, 0));
		SVector4 temp2 = SVector4(matrix(0, 1), matrix(1, 1), matrix(2, 1), matrix(3, 1));
		SVector4 temp3 = SVector4(matrix(0, 2), matrix(1, 2), matrix(2, 2), matrix(3, 2));
		SVector4 temp4 = SVector4(matrix(0, 3), matrix(1, 3), matrix(2, 3), matrix(3, 3));
		result.X = vector.Dot(temp1);
		result.Y = vector.Dot(temp2);
		result.Z = vector.Dot(temp3);
		result.W = vector.Dot(temp4);
		return result;
	}

	SVector4 operator*(SVector4 vector, SMatrix matrix)
	{
		SVector4 result;
		SVector4 temp1 = SVector4(matrix(0, 0), matrix(1, 0), matrix(2, 0), matrix(3, 0));
		SVector4 temp2 = SVector4(matrix(0, 1), matrix(1, 1), matrix(2, 1), matrix(3, 1));
		SVector4 temp3 = SVector4(matrix(0, 2), matrix(1, 2), matrix(2, 2), matrix(3, 2));
		SVector4 temp4 = SVector4(matrix(0, 3), matrix(1, 3), matrix(2, 3), matrix(3, 3));
		result.X = vector.Dot(temp1);
		result.Y = vector.Dot(temp2);
		result.Z = vector.Dot(temp3);
		result.W = vector.Dot(temp4);
		return result;
	}


	SMatrix& SMatrix::operator=(const SMatrix& matrix)
	{
		for (U8 row = 0; row < 4; ++row)
		{
			for (U8 column = 0; column < 4; ++column)
			{
				M[row][column] = matrix(row, column);
			}
		}
		return *this;
	}

	SMatrix SMatrix::operator*(F32 scalar)
	{
		SMatrix matrix = *this;
		for (U8 row = 0; row < 4; ++row)
		{
			for (U8 column = 0; column < 4; ++column)
			{
				matrix(row, column) *= scalar;
			}
		}
		return matrix;
	}

	SMatrix& SMatrix::operator*=(F32 scalar)
	{
		for (U8 row = 0; row < 4; ++row)
		{
			for (U8 column = 0; column < 4; ++column)
			{
				M[row][column] *= scalar;
			}
		}
		return *this;
	}


	bool SMatrix::operator==(const SMatrix& matrix) const
	{
		bool equal = true;
		for (U8 row = 0; row < 4; ++row)
		{
			for (U8 column = 0; column < 4; ++column)
			{
				if (M[row][column] != matrix(row, column))
				{
					equal = false;
				}
			}
		}
		return equal;
	}

	SVector4 SMatrix::Row(U8 index) const
	{
		return SVector4(M[index][0], M[index][1], M[index][2], M[index][3]);
	}

	SVector4 SMatrix::Column(U8 index) const
	{
		return SVector4(M[0][index], M[1][index], M[2][index], M[3][index]);
	}

	inline void SMatrix::Row(U8 index, const SVector4& vector)
	{
		M[index][0] = vector.X;
		M[index][1] = vector.Y;
		M[index][2] = vector.Z;
		M[index][3] = vector.W;
	}

	inline void SMatrix::Column(U8 index, const SVector4& vector)
	{
		M[0][index] = vector.X;
		M[1][index] = vector.Y;
		M[2][index] = vector.Z;
		M[3][index] = vector.W;
	}

	inline SVector SMatrix::Up() const
	{
		return SVector(M[1][0], M[1][1], M[1][2]);
	}

	inline SVector SMatrix::Down() const
	{
		return SVector(-M[1][0], -M[1][1], -M[1][2]);
	}

	inline SVector SMatrix::Right() const
	{
		return SVector(M[0][0], M[0][1], M[0][2]);
	}

	inline SVector SMatrix::Left() const
	{
		return SVector(-M[0][0], -M[0][1], -M[0][2]);
	}

	inline SVector SMatrix::Forward() const
	{
		return SVector(M[2][0], M[2][1], M[2][2]);
	}

	inline SVector SMatrix::Backward() const
	{
		return SVector(-M[2][0], -M[2][1], -M[2][2]);
	}

	inline SVector SMatrix::Translation() const
	{
		return SVector(M[3][0], M[3][1], M[3][2]);
	}

	inline SVector4 SMatrix::Translation4() const
	{
		return Row(3);
	}

	inline void SMatrix::Up(const SVector& v)
	{
		M[1][0] = v.X;
		M[1][1] = v.Y;
		M[1][2] = v.Z;
	}

	inline void SMatrix::Down(const SVector& v)
	{
		M[1][0] = -v.X;
		M[1][1] = -v.Y;
		M[1][2] = -v.Z;
	}

	inline void SMatrix::Right(const SVector& v)
	{
		M[0][0] = v.X;
		M[0][1] = v.Y;
		M[0][2] = v.Z;
	}

	inline void SMatrix::Left(const SVector& v)
	{
		M[0][0] = -v.X;
		M[0][1] = -v.Y;
		M[0][2] = -v.Z;
	}

	inline void SMatrix::Forward(const SVector& v)
	{
		M[2][0] = v.X;
		M[2][1] = v.Y;
		M[2][2] = v.Z;
	}

	inline void SMatrix::Backward(const SVector& v)
	{
		M[2][0] = -v.X;
		M[2][1] = -v.Y;
		M[2][2] = -v.Z;
	}

	inline void SMatrix::Translation(const SVector& v)
	{
		M[3][0] = v.X;
		M[3][1] = v.Y;
		M[3][2] = v.Z;
	}

	inline void SMatrix::Translation(const SVector4& v)
	{
		M[3][0] = v.X;
		M[3][1] = v.Y;
		M[3][2] = v.Z;
		M[3][3] = v.W;
	}

	// TODO.NR: Fix this
	inline void SMatrix::Rotate(const SMatrix& rotationMatrix)
	{
		SMatrix copy = rotationMatrix;
		copy *= GetRotationMatrix();
		SetRotation(GetRotationMatrix() * copy);
	}

	inline void SMatrix::Rotate(const SVector& eulerAngles)
	{
		if (eulerAngles.IsEqual(SVector::Zero))
			return;

		SMatrix rightRotation = SMatrix::CreateRotationAroundAxis(eulerAngles.X, Right());
		SMatrix upRotation = SMatrix::CreateRotationAroundAxis(eulerAngles.Y, Up());
		SMatrix forwardRotation = SMatrix::CreateRotationAroundAxis(eulerAngles.Z, Forward());
		SMatrix finalRotation = GetRotationMatrix();
		finalRotation *= rightRotation;
		finalRotation *= upRotation;
		finalRotation *= forwardRotation;
		SetRotation(finalRotation);
	}
	
	inline void SMatrix::Translate(const SVector& v)
	{
		SVector localMove = Right() * v.X;
		M[3][0] += localMove.X;
		M[3][1] += localMove.Y;
		M[3][2] += localMove.Z;
		localMove = Up() * v.Y;
		M[3][0] += localMove.X;
		M[3][1] += localMove.Y;
		M[3][2] += localMove.Z;
		localMove = Forward() * v.Z;
		M[3][0] += localMove.X;
		M[3][1] += localMove.Y;
		M[3][2] += localMove.Z;
	}
	
	inline void SMatrix::Translate(const SVector4& v)
	{
		SVector localMove = Right() * v.X;
		M[3][0] += localMove.X;
		M[3][1] += localMove.Y;
		M[3][2] += localMove.Z;
		localMove = Up() * v.Y;
		M[3][0] += localMove.X;
		M[3][1] += localMove.Y;
		M[3][2] += localMove.Z;
		localMove = Forward() * v.Z;
		M[3][0] += localMove.X;
		M[3][1] += localMove.Y;
		M[3][2] += localMove.Z;
	}

	// TODO.NR: Make transform struct which can store local transform data and make parent from point argument
	inline void SMatrix::Orbit(const SVector& /*point*/, const SMatrix& rotation)
	{
		SMatrix finalRotation = rotation;
		//finalRotation.Translation(point);
		//SMatrix parentTransform = SMatrix();
		//parentTransform.Translation(point);
		//Translate(-point);
		(*this) *= finalRotation;
		//(*this) *= parentTransform;
		//Translate(point);
	}

	inline void SMatrix::Orbit(const SVector4& /*point*/, const SMatrix& rotation)
	{
		SMatrix finalRotation = rotation;
		//finalRotation.Translation(point);
		(*this) *= finalRotation;
	}

	// Static function for creating a transpose of a matrix.
	inline SMatrix SMatrix::Transpose(const SMatrix& matrixToTranspose)
	{
		SMatrix matrix = SMatrix();
		for (U8 row = 0; row < 4; ++row)
		{
			for (U8 column = 0; column < 4; ++column)
			{
				matrix(row, column) = matrixToTranspose(column, row);
			}
		}
		return matrix;
	}

	inline SMatrix SMatrix::Inverse() const
	{
		SMatrix matrixTranspose = Transpose(*this);

		SVector4 V0[4], V1[4];
		SVector4 row0 = matrixTranspose.Row(0);
		SVector4 row1 = matrixTranspose.Row(1);
		SVector4 row2 = matrixTranspose.Row(2);
		SVector4 row3 = matrixTranspose.Row(3);

		V0[0] = SVector4(row2.X, row2.X, row2.Y, row2.Y);
		V1[0] = SVector4(row3.Z, row3.W, row3.Z, row3.W);
		V0[1] = SVector4(row0.X, row0.X, row0.Y, row0.Y);
		V1[1] = SVector4(row1.Z, row1.W, row1.Z, row1.W);
		V0[2] = SVector4(row2.X, row2.Z, row0.X, row0.Z);
		V1[2] = SVector4(row3.Y, row3.W, row1.Y, row1.W);

		SVector4 D0 = V0[0] * V1[0];
		SVector4 D1 = V0[1] * V1[1];
		SVector4 D2 = V0[2] * V1[2];

		V0[0] = SVector4(row2.Z, row2.W, row2.Z, row2.W);
		V1[0] = SVector4(row3.X, row3.X, row3.Y, row3.Y);
		V0[1] = SVector4(row0.Z, row0.W, row0.Z, row0.W);
		V1[1] = SVector4(row1.X, row1.X, row1.Y, row1.Y);
		V0[2] = SVector4(row2.Y, row2.W, row0.Y, row0.W);
		V1[2] = SVector4(row3.X, row3.Z, row1.X, row1.Z);

		D0 = D0 - (V0[0] * V1[0]);
		D1 = D1 - (V0[1] * V1[1]);
		D2 = D2 - (V0[2] * V1[2]);

		V0[0] = SVector4(row1.Y, row1.Z, row1.X, row1.Y);
		V1[0] = SVector4(D2.Y, D0.Y, D0.W, D0.X);
		V0[1] = SVector4(row0.Z, row0.X, row0.Y, row0.X);
		V1[1] = SVector4(D0.W, D2.Y, D0.Y, D0.Z);
		V0[2] = SVector4(row3.Y, row3.Z, row3.X, row3.Y);
		V1[2] = SVector4(D2.W, D1.Y, D1.W, D1.X);
		V0[3] = SVector4(row2.Z, row2.X, row2.Y, row2.X);
		V1[3] = SVector4(D1.W, D2.W, D1.Y, D1.Z);

		SVector4 C0 = V0[0] * V1[0];
		SVector4 C2 = V0[1] * V1[1];
		SVector4 C4 = V0[2] * V1[2];
		SVector4 C6 = V0[3] * V1[3];

		V0[0] = SVector4(row1.Z, row1.W, row1.Y, row1.Z);
		V1[0] = SVector4(D0.W, D0.X, D0.Y, D2.X);
		V0[1] = SVector4(row0.W, row0.Z, row0.W, row0.Y);
		V1[1] = SVector4(D0.Z, D0.Y, D2.X, D0.X);
		V0[2] = SVector4(row3.Z, row3.W, row3.Y, row3.Z);
		V1[2] = SVector4(D1.W, D1.X, D1.Y, D2.Z);
		V0[3] = SVector4(row2.W, row2.Z, row2.W, row2.Y);
		V1[3] = SVector4(D1.Z, D1.Y, D2.Z, D1.X);

		C0 = C0 - (V0[0] * V1[0]);
		C2 = C2 - (V0[1] * V1[1]);
		C4 = C4 - (V0[2] * V1[2]);
		C6 = C6 - (V0[3] * V1[3]);

		V0[0] = SVector4(row1.W, row1.X, row1.W, row1.X);
		V1[0] = SVector4(D0.Z, D2.Y, D2.X, D0.Z);
		V0[1] = SVector4(row0.Y, row0.W, row0.X, row0.Z);
		V1[1] = SVector4(D2.Y, D0.X, D0.W, D2.X);
		V0[2] = SVector4(row3.W, row3.X, row3.W, row3.X);
		V1[2] = SVector4(D1.Z, D2.W, D2.Z, D1.Z);
		V0[3] = SVector4(row2.Y, row2.W, row2.X, row2.Z);
		V1[3] = SVector4(D2.W, D1.X, D1.W, D2.Z);

		SVector4 C1 = C0 - (V0[0] * V1[0]);
		C0 = C0 - (V0[0] * V1[0]);
		SVector4 C3 = C2 - (V0[1] * V1[1]);
		C2 = C2 - (V0[1] * V1[1]);
		SVector4 C5 = C4 - (V0[2] * V1[2]);
		C4 = C4 - (V0[2] * V1[2]);
		SVector4 C7 = C6 - (V0[3] * V1[3]);
		C6 = C6 - (V0[3], V1[3]);

		SMatrix R;
		R.Row(0, { C0.X, C1.Y, C0.Z, C1.W });
		R.Row(1, { C2.X, C3.Y, C2.Z, C3.W });
		R.Row(2, { C4.X, C5.Y, C4.Z, C5.W });
		R.Row(3, { C6.X, C7.Y, C6.Z, C7.W });

		F32 determinant = R.Row(0).Dot(matrixTranspose.Row(0));
		SVector4 reciprocal = SVector4(1.0f / determinant);

		SMatrix result;
		result.Row(0) = R.Row(0) * reciprocal;
		result.Row(1) = R.Row(1) * reciprocal;
		result.Row(2) = R.Row(2) * reciprocal;
		result.Row(3) = R.Row(3) * reciprocal;
		return result;
	}

	inline SMatrix SMatrix::LookAtLH(SVector eyePosition, SVector focusPosition, SVector upDirection)
	{
		SVector eyeDirection = focusPosition - eyePosition;
		return LookToLH(eyePosition, eyeDirection, upDirection);
	}

	inline SMatrix SMatrix::LookToLH(SVector eyePosition, SVector eyeDirection, SVector upDirection)
	{
		assert(!eyeDirection.IsEqual(SVector::Zero));
		assert(!upDirection.IsEqual(SVector::Zero));

		SVector r2 = eyeDirection.GetNormalized();
		SVector r0 = upDirection.Cross(r2).GetNormalized();
		SVector r1 = r2.Cross(r0);

		SVector negEyePosition = -eyePosition;

		F32 d0 = r0.Dot(negEyePosition);
		F32 d1 = r1.Dot(negEyePosition);
		F32 d2 = r2.Dot(negEyePosition);

		SMatrix M;
		M.Right({ r0.X, r0.Y, r0.Z });
		M.Up({ r1.X, r1.Y, r1.Z });
		M.Forward({ r2.X, r2.Y, r2.Z });
		M(0, 3) = d0;
		M(1, 3) = d1;
		M(2, 3) = d2;

		return Transpose(M);
	}

	inline SMatrix SMatrix::PerspectiveFovLH(F32 fovAngleY, F32 aspectRatio, F32 nearZ, F32 farZ)
	{
		assert(nearZ > 0.f && farZ > 0.f);
		assert(!UMath::NearlyEqual(fovAngleY, 0.0f));
		assert(!UMath::NearlyEqual(aspectRatio, 0.0f));
		assert(!UMath::NearlyEqual(farZ, nearZ));

		F32 sinFov;
		F32 cosFov;
		UMath::MapFov(sinFov, cosFov, 0.5f * fovAngleY);

		F32 height = cosFov / sinFov;
		F32 width = height / aspectRatio;
		F32 fRange = farZ / (farZ - nearZ);

		SMatrix matrix;
		matrix(0, 0) = width;
		matrix(0, 1) = 0.0f;
		matrix(0, 2) = 0.0f;
		matrix(0, 3) = 0.0f;

		matrix(1, 0) = 0.0f;
		matrix(1, 1) = height;
		matrix(1, 2) = 0.0f;
		matrix(1, 3) = 0.0f;

		matrix(2, 0) = 0.0f;
		matrix(2, 1) = 0.0f;
		matrix(2, 2) = fRange;
		matrix(2, 3) = 1.0f;

		matrix(3, 0) = 0.0f;
		matrix(3, 1) = 0.0f;
		matrix(3, 2) = fRange * nearZ * -1.0f;
		matrix(3, 3) = 0.0f;
		return matrix;
	}
}
