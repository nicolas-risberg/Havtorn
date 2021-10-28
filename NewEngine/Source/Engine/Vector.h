#pragma once
#include "CoreTypes.h"
#include "EngineMath.h"

namespace NewEngine
{

#define VECTOR_COMPARISON_EPSILON 1.e-4f
#define VECTOR_NORMALIZED_EPSILON 1.e-1f

	struct SVector
	{
		F32 X, Y, Z;

		static const SVector Zero;

		static const SVector Right;
		static const SVector Up;
		static const SVector Forward;

		static const SVector Left;
		static const SVector Down;
		static const SVector Backward;

		inline SVector();
		inline SVector(F32 a);
		inline SVector(F32 x, F32 y, F32 z);
		SVector(const SVector& other) = default;
		~SVector() = default;

		inline SVector operator+(F32 a) const;
		inline SVector operator-(F32 a) const;
		inline SVector operator*(F32 a) const;
			   SVector operator/(F32 a) const;
		
		inline SVector operator+(const SVector& other) const;
		inline SVector operator-(const SVector& other) const;
		inline SVector operator*(const SVector& other) const;
			   SVector operator/(const SVector& other) const;

		inline SVector operator+=(F32 a);
		inline SVector operator-=(F32 a);
		inline SVector operator*=(F32 a);
			   SVector operator/=(F32 a);

		inline SVector operator+=(const SVector& other);
		inline SVector operator-=(const SVector& other);
		inline SVector operator*=(const SVector& other);
			   SVector operator/=(const SVector& other);

		inline bool operator==(const SVector& other) const;
		inline bool operator!=(const SVector& other) const;

		inline SVector operator-() const;

		inline F32 Dot(const SVector& other) const;
		inline SVector Cross(const SVector& other) const;

		inline F32 Length() const;
		inline F32 LengthSquared() const;
		inline F32 Size() const;
		inline F32 SizeSquared() const;

		inline F32 Length2D() const;
		inline F32 LengthSquared2D() const;
		inline F32 Size2D() const;
		inline F32 SizeSquared2D() const;

		void Normalize();
		SVector Normalized() const;
		bool IsNormalized() const;

		inline bool IsEqual(const SVector& other, F32 tolerance = VECTOR_COMPARISON_EPSILON) const;

		inline void ToDirectionAndLength(SVector& OutDirection, F32& outLength) const;

		inline F32 Distance(const SVector& other) const;
		inline F32 DistanceSquared(const SVector& other) const;
		inline F32 Distance2D(const SVector& other) const;
		inline F32 DistanceSquared2D(const SVector& other) const;

		SVector Projection(const SVector& other) const;
		SVector Mirrored(const SVector& mirrorNormal) const;

		std::string ToString();
	};

	const SVector SVector::Zero		= SVector(0, 0, 0);

	const SVector SVector::Right	= SVector(1, 0, 0);
	const SVector SVector::Up		= SVector(0, 1, 0);
	const SVector SVector::Forward	= SVector(0, 0, 1);

	const SVector SVector::Left		= SVector(-1, 0, 0);;
	const SVector SVector::Down		= SVector(0, -1, 0);
	const SVector SVector::Backward	= SVector(0, 0, -1);

	SVector::SVector() : X(0), Y(0), Z(0) {}
	SVector::SVector(F32 a) : X(a), Y(a), Z(a) {}
	SVector::SVector(F32 x, F32 y, F32 z) : X(x), Y(y), Z(z) {}
	
	inline SVector SVector::operator+(F32 a) const
	{
		return SVector(X + a, Y + a, Z + a);
	}

	inline SVector SVector::operator-(F32 a) const
	{
		return SVector(X - a, Y - a, Z - a);
	}

	inline SVector SVector::operator*(F32 a) const
	{
		return SVector(X * a, Y * a, Z * a);
	}

	SVector SVector::operator/(F32 a) const
	{
		// TODO.NR: Print out error message about div with zero
		if (a == 0)
			return SVector();

		const F32 scale = 1 / a;
		return SVector(X * scale, Y * scale, Z * scale);
	}

	inline SVector SVector::operator+(const SVector& other) const
	{
		return SVector(X + other.X, Y + other.Y, Z + other.Z);
	}

	inline SVector SVector::operator-(const SVector& other) const
	{
		return SVector(X - other.X, Y - other.Y, Z - other.Z);
	}

	// Hadamard multiplication
	inline SVector SVector::operator*(const SVector& other) const
	{
		return SVector(X * other.X, Y * other.Y, Z * other.Z);
	}

	SVector SVector::operator/(const SVector& other) const
	{
		// TODO.NR: Print out error message about div with zero
		if (other.X == 0 || other.Y == 0 || other.Z == 0)
			return SVector();

		return SVector(X / other.X, Y / other.Y, Z / other.Z);
	}

	inline SVector SVector::operator+=(F32 a)
	{
		X += a; Y += a; Z += a;
		return *this;
	}

	inline SVector SVector::operator-=(F32 a)
	{
		X -= a; Y -= a; Z -= a;
		return *this;
	}

	inline SVector SVector::operator*=(F32 a)
	{
		X *= a; Y *= a; Z *= a;
		return *this;
	}

	SVector SVector::operator/=(F32 a)
	{
		// TODO.NR: Print out error message about div with zero
		if (a == 0)
			return SVector();

		F32 scale = 1 / a;
		X *= scale; Y *= scale; Z *= scale;
		return *this;
	}

	inline SVector SVector::operator+=(const SVector& other)
	{
		X += other.X; Y += other.Y; Z += other.Z;
		return *this;
	}

	inline SVector SVector::operator-=(const SVector& other)
	{
		X -= other.X; Y -= other.Y; Z -= other.Z;
		return *this;
	}

	// Hadamard multiplication
	inline SVector SVector::operator*=(const SVector& other)
	{
		X *= other.X; Y *= other.Y; Z *= other.Z;
		return *this;
	}

	SVector SVector::operator/=(const SVector& other)
	{
		// TODO.NR: Print out error message about div with zero 
		if (other.X == 0 || other.Y == 0 || other.Z == 0)
			return SVector();

		X /= other.X; Y /= other.Y; Z /= other.Z;
		return *this;
	}

	inline bool SVector::operator==(const SVector& other) const
	{
		return X == other.X && Y == other.Y && Z == other.Y;
	}

	inline bool SVector::operator!=(const SVector& other) const
	{
		return X != other.X || Y != other.Y || Z != other.Z;
	}

	inline SVector SVector::operator-() const
	{
		return SVector(-X, -Y, -Z);
	}

	inline F32 SVector::Dot(const SVector& other) const
	{
		return X * other.X + Y * other.Y + Z * other.Z;
	}

	inline SVector SVector::Cross(const SVector& other) const
	{
		return SVector
		(
			Y * other.Z - Z * other.Y,
			Z * other.X - X * other.Z,
			X * other.Y - Y * other.X
		);
	}

	inline F32 SVector::Length() const
	{
		return UMath::Sqrt(X * X + Y * Y + Z * Z);
	}

	inline F32 SVector::LengthSquared() const
	{
		return (X * X + Y * Y + Z * Z);
	}

	inline F32 SVector::Size() const
	{
		return this->Length();
	}

	inline F32 SVector::SizeSquared() const
	{
		return this->SizeSquared();
	}

	inline F32 SVector::Length2D() const
	{
		return UMath::Sqrt(X * X + Y * Y);
	}

	inline F32 SVector::LengthSquared2D() const
	{
		return (X * X + Y * Y);
	}

	inline F32 SVector::Size2D() const
	{
		return this->Length2D();
	}

	inline F32 SVector::SizeSquared2D() const
	{
		return this->LengthSquared2D();
	}

	void SVector::Normalize()
	{
		F32 length = this->Length();
		*this /= length;
	}

	inline SVector SVector::Normalized() const
	{
		F32 length = this->Length();
		return SVector(*this) / length;
	}

	inline bool SVector::IsNormalized() const
	{
		return UMath::Abs(1 - SizeSquared()) < VECTOR_NORMALIZED_EPSILON;
	}

	inline bool SVector::IsEqual(const SVector& other, F32 tolerance) const
	{
		return UMath::Abs(X - other.X) <= tolerance && UMath::Abs(Y - other.Y) <= tolerance && UMath::Abs(Z - other.Z) <= tolerance;
	}

	inline void SVector::ToDirectionAndLength(SVector& outDirection, F32& outLength) const
	{
		outLength = Size();
		if (outLength > SMALL_NUMBER)
		{
			F32 lengthReciprocal = 1.0f / outLength;
			outDirection = SVector(X * lengthReciprocal, Y * lengthReciprocal, Z * lengthReciprocal);
		}
		else
		{
			outDirection = SVector::Zero;
		}
	}

	inline F32 SVector::Distance(const SVector& other) const
	{
		return UMath::Sqrt(this->DistanceSquared(other));
	}

	inline F32 SVector::DistanceSquared(const SVector& other) const
	{
		return UMath::Square(X - other.X) + UMath::Square(Y - other.Y) + UMath::Square(Z - other.Z);
	}

	inline F32 SVector::Distance2D(const SVector& other) const
	{
		return UMath::Sqrt(this->DistanceSquared2D(other));
	}

	inline F32 SVector::DistanceSquared2D(const SVector& other) const
	{
		return UMath::Square(X - other.X) + UMath::Square(Y - other.Y);
	}

	inline SVector SVector::Projection(const SVector& other) const
	{
		const SVector& direction = other.Normalized();

		F32 length = this->Dot(direction);
		return direction * length;
	}

	inline SVector SVector::Mirrored(const SVector& mirrorNormal) const
	{
		return *this - mirrorNormal * (2.0f * (this->Dot(mirrorNormal)));
	}

	SVector operator*(F32 a, const SVector& vector)
	{
		SVector newVector = vector;
		newVector *= a;
		return newVector;
	}

	std::string SVector::ToString()
	{
		char buffer[32];
		sprintf_s(buffer, "{X: %.1f, Y: %.1f, Z: %.1f}", X, Y, Z);
		return buffer;
	}
}