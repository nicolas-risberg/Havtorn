// Copyright 2022 Team Havtorn. All Rights Reserved.

#pragma once

#include "EngineMath.h"
#include "Vector.h"
#include "Matrix4x4.h"

namespace Havtorn 
{
	class Matrix3x3 
	{
	public:
		// Creates the identity matrix.
		Matrix3x3();
		// Copy Constructor.
		Matrix3x3(const Matrix3x3& matrix);
		// Copies the top left 3x3 part of the Matrix4x4.
		Matrix3x3(const Matrix4x4& matrix);
		// () operator for accessing element (row, column) for read/write or read, respectively.
		F32& operator()(const int row, const int column);
		const F32& operator()(const int row, const int column) const;
		// Static functions for creating rotation matrices.
		static Matrix3x3 CreateRotationAroundX(F32 angleInRadians);
		static Matrix3x3 CreateRotationAroundY(F32 angleInRadians);
		static Matrix3x3 CreateRotationAroundZ(F32 angleInRadians);
		// Static function for creating a transpose of a matrix.
		static Matrix3x3 Transpose(const Matrix3x3& matrix_to_transpose);
		// Operator overloads
		Matrix3x3 operator+(const Matrix3x3& matrix);
		Matrix3x3& operator+=(const Matrix3x3& matrix);
		Matrix3x3 operator-(const Matrix3x3& matrix);
		Matrix3x3& operator-=(const Matrix3x3& matrix);
		Matrix3x3 operator*(const Matrix3x3& matrix);
		Matrix3x3& operator*=(const Matrix3x3& matrix);
		Matrix3x3& operator=(const Matrix3x3& matrix);
		bool &operator==(const Matrix3x3& matrix) const;

	private:
		F32 Matrix[3][3];
	};

	Matrix3x3::Matrix3x3() 
	{
		for (unsigned int row = 0; row < 3; ++row) 
		{
			for (unsigned int column = 0; column < 3; ++column) 
			{
				if (row == column) 
				{
					Matrix[row][column] = 1;
				} 
				else 
				{
					Matrix[row][column] = 0;
				}
			}
		}
	}

	Matrix3x3::Matrix3x3(const Matrix3x3& matrix) 
	{
		for (unsigned int row = 0; row < 3; ++row) 
		{
			for (unsigned int column = 0; column < 3; ++column) 
			{
				Matrix[row][column] = matrix(row, column);
			}
		}
	}

	Matrix3x3::Matrix3x3(const Matrix4x4& matrix) 
	{
		for (unsigned int row = 0; row < 3; ++row) 
		{
			for (unsigned int column = 0; column < 3; ++column) 
			{
				Matrix[row][column] = matrix(row, column);
			}
		}
	}

	F32& Matrix3x3::operator()(const int row, const int column) 
	{
		//return const_cast<T&>(static_cast<const T[][]&>(*this->_matrix)[row][column]);
		assert(row >= 0 && column >= 0 && row < 3 && column < 3);
		return Matrix[row][column];
	}

	const F32 &Matrix3x3::operator()(const int row, const int column) const 
	{
		assert(row >= 0 && column >= 0 && row < 3 && column < 3);
		return Matrix[row][column];
	}

	Matrix3x3 Matrix3x3::CreateRotationAroundX(F32 angleInRadians) 
	{
		Matrix3x3 matrix = Matrix3x3();
		matrix(1, 1) = UMath::Cos(angleInRadians);
		matrix(1, 2) = UMath::Sin(angleInRadians);
		matrix(2, 1) = -UMath::Sin(angleInRadians);
		matrix(2, 2) = UMath::Cos(angleInRadians);
		return matrix;
	}

	Matrix3x3 Matrix3x3::CreateRotationAroundY(F32 angleInRadians) 
	{
		Matrix3x3 matrix = Matrix3x3();
		matrix(0, 0) = UMath::Cos(angleInRadians);
		matrix(0, 2) = -UMath::Sin(angleInRadians);
		matrix(2, 0) = UMath::Sin(angleInRadians);
		matrix(2, 2) = UMath::Cos(angleInRadians);
		return matrix;
	}

	Matrix3x3 Matrix3x3::CreateRotationAroundZ(F32 angleInRadians) 
	{
		Matrix3x3 matrix = Matrix3x3();
		matrix(0, 0) = UMath::Cos(angleInRadians);
		matrix(0, 1) = UMath::Sin(angleInRadians);
		matrix(1, 0) = -UMath::Sin(angleInRadians);
		matrix(1, 1) = UMath::Cos(angleInRadians);
		return matrix;
	}

	Matrix3x3 Matrix3x3::Transpose(const Matrix3x3& matrixToTranspose) 
	{
		Matrix3x3 matrix = Matrix3x3();
		for (unsigned int row = 0; row < 3; ++row) 
		{
			for (unsigned int column = 0; column < 3; ++column) 
			{
				matrix(row, column) = matrixToTranspose(column, row);
			}
		}
		return matrix;
	}

	Matrix3x3 Matrix3x3::operator+(const Matrix3x3& matrix) 
	{
		Matrix3x3 matrixResult = Matrix3x3();
		for (unsigned int row = 0; row < 3; ++row) 
		{
			for (unsigned int column = 0; column < 3; ++column) 
			{
				matrixResult(row, column) = Matrix[row][column] + matrix(row, column);
			}
		}
		return matrixResult;
	}

	Matrix3x3& Matrix3x3::operator+=(const Matrix3x3& matrix) 
	{
		for (unsigned int row = 0; row < 3; ++row) 
		{
			for (unsigned int column = 0; column < 3; ++column) 
			{
				Matrix[row][column] += matrix(row, column);
			}
		}
		return *this;
	}

	Matrix3x3 Matrix3x3::operator-(const Matrix3x3& matrix) 
	{
		Matrix3x3 matrixResult = Matrix3x3();
		for (unsigned int row = 0; row < 3; ++row) 
		{
			for (unsigned int column = 0; column < 3; ++column) 
			{
				matrixResult(row, column) = Matrix[row][column] - matrix(row, column);
			}
		}
		return matrixResult;
	}

	Matrix3x3& Matrix3x3::operator-=(const Matrix3x3& matrix) 
	{
		for (unsigned int row = 0; row < 3; ++row) 
		{
			for (unsigned int column = 0; column < 3; ++column) 
			{
				Matrix[row][column] -= matrix(row, column);
			}
		}
		return *this;
	}

	Matrix3x3 Matrix3x3::operator*(const Matrix3x3& matrix) 
	{
		Matrix3x3 matrixResult = Matrix3x3();
		for (unsigned int row = 0; row < 3; ++row) 
		{
			for (unsigned int column = 0; column < 3; ++column) 
			{
				matrixResult(row, column) = Matrix[row][0] * matrix(0, column);
				matrixResult(row, column) += Matrix[row][1] * matrix(1, column);
				matrixResult(row, column) += Matrix[row][2] * matrix(2, column);
			}
		}
		return matrixResult;
	}

	Matrix3x3& Matrix3x3::operator*=(const Matrix3x3& matrix) 
	{
		Matrix3x3 matrixResult = Matrix3x3();
		for (unsigned int row = 0; row < 3; ++row) 
		{
			for (unsigned int column = 0; column < 3; ++column) 
			{
				matrixResult(row, column) = Matrix[row][0] * matrix(0, column);
				matrixResult(row, column) += Matrix[row][1] * matrix(1, column);
				matrixResult(row, column) += Matrix[row][2] * matrix(2, column);
			}
		}
		*this = matrixResult;
		return *this;
	}

	SVector operator*(Matrix3x3 matrix, SVector vector) 
	{
		SVector vectorResult;
		SVector temp1 = SVector(matrix(0, 0), matrix(1, 0), matrix(0, 0));
		SVector temp2 = SVector(matrix(0, 1), matrix(1, 1), matrix(0, 1));
		SVector temp3 = SVector(matrix(0, 2), matrix(1, 2), matrix(0, 2));
		vectorResult.X = vector.Dot(temp1);
		vectorResult.Y = vector.Dot(temp2);
		vectorResult.Z = vector.Dot(temp3);
		return vectorResult;
	}

	SVector operator*(SVector vector, Matrix3x3 matrix) 
	{
		return matrix*vector;
	}

	Matrix3x3& Matrix3x3::operator=(const Matrix3x3& matrix) 
	{
		for (unsigned int row = 0; row < 3; ++row) 
		{
			for (unsigned int column = 0; column < 3; ++column) 
			{
				Matrix[row][column] = matrix(row, column);
			}
		}
		return *this;
	}

	bool &Matrix3x3::operator==(const Matrix3x3& matrix) const
	{
		bool equal = true;
		for (unsigned int row = 0; row < 3; ++row) 
		{
			for (unsigned int column = 0; column < 3; ++column) 
			{
				if (Matrix[row][column] != matrix(row, column)) 
				{
					equal = false;
				}
			}
		}
		return equal;
	}
}