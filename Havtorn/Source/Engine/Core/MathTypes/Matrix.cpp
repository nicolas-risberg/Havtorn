#include "Matrix.h"
#include "Vector.h"
#include "Quaternion.h"

namespace Havtorn
{
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


	SMatrix SMatrix::CreateRotationAroundX(F32 angleInRadians)
	{
		SMatrix matrix = SMatrix();
		matrix(1, 1) = UMath::Cos(angleInRadians);
		matrix(1, 2) = UMath::Sin(angleInRadians);
		matrix(2, 1) = -UMath::Sin(angleInRadians);
		matrix(2, 2) = UMath::Cos(angleInRadians);
		return matrix;
	}


	SMatrix SMatrix::CreateRotationAroundY(F32 angleInRadians)
	{
		SMatrix matrix = SMatrix();
		matrix(0, 0) = UMath::Cos(angleInRadians);
		matrix(0, 2) = -UMath::Sin(angleInRadians);
		matrix(2, 0) = UMath::Sin(angleInRadians);
		matrix(2, 2) = UMath::Cos(angleInRadians);
		return matrix;
	}


	SMatrix SMatrix::CreateRotationAroundZ(F32 angleInRadians)
	{
		SMatrix matrix = SMatrix();
		matrix(0, 0) = UMath::Cos(angleInRadians);
		matrix(0, 1) = UMath::Sin(angleInRadians);
		matrix(1, 0) = -UMath::Sin(angleInRadians);
		matrix(1, 1) = UMath::Cos(angleInRadians);
		return matrix;
	}

	SMatrix SMatrix::CreateRotationFromEuler(F32 pitch, F32 yaw, F32 roll)
	{
		SQuaternion quaternion = SQuaternion(pitch, yaw, roll);
		return CreateRotationFromQuaternion(quaternion);
	}

	SMatrix SMatrix::CreateRotationFromQuaternion(SQuaternion quaternion)
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

	// Static function for creating a transpose of a matrix.
	SMatrix SMatrix::Transpose(const SMatrix& matrixToTranspose)
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

	SMatrix SMatrix::FastInverse() const
	{
		SMatrix rotation = this->GetRotationMatrix() * -1.0f;
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

	SMatrix SMatrix::LookAtLH(SVector eyePosition, SVector focusPosition, SVector upDirection)
	{
		SVector eyeDirection = focusPosition - eyePosition;
		return LookToLH(eyePosition, eyeDirection, upDirection);
	}

	SMatrix SMatrix::LookToLH(SVector eyePosition, SVector eyeDirection, SVector upDirection)
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

	SMatrix SMatrix::PerspectiveFovLH(F32 fovAngleY, F32 aspectRatio, F32 nearZ, F32 farZ)
	{
		assert(nearZ > 0.f && farZ > 0.f);
		assert(!UMath::NearlyEqual(fovAngleY, 0.0f));
		assert(!UMath::NearlyEqual(aspectRatio, 0.0f));
		assert(!UMath::NearlyEqual(farZ, nearZ));

		F32 sinFov;
		F32 cosFov;
		UMath::MapFov(sinFov, cosFov, 0.5f * fovAngleY);

		float height = cosFov / sinFov;
		float width = height / aspectRatio;
		float fRange = farZ / (farZ - nearZ);

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
		matrix(3, 2) = -fRange * nearZ;
		matrix(3, 3) = 0.0f;
		return matrix;
	}

	SVector4 SMatrix::Row(U8 index) const
	{
		return SVector4(M[index][0], M[index][1], M[index][2], M[index][3]);
	}

	SVector4 SMatrix::Column(U8 index) const
	{
		return SVector4(M[0][index], M[1][index], M[2][index], M[3][index]);
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

}