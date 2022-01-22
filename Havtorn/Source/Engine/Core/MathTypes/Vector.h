// Copyright 2022 Team Havtorn. All Rights Reserved.

#pragma once

#include "Core/CoreTypes.h"
#include "EngineMath.h"

namespace Havtorn
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
		inline SVector(F32 x, F32 y);
		inline SVector(F32 x, F32 y, F32 z);
		SVector(const SVector& other) = default;
		~SVector() = default;

		inline SVector operator+(F32 a) const;
		inline SVector operator-(F32 a) const;
		inline SVector operator*(F32 a) const;
		inline SVector operator/(F32 a) const;
		
		inline SVector operator+(const SVector& other) const;
		inline SVector operator-(const SVector& other) const;
		inline SVector operator*(const SVector& other) const;
		inline SVector operator/(const SVector& other) const;

		inline SVector operator+=(F32 a);
		inline SVector operator-=(F32 a);
		inline SVector operator*=(F32 a);
		inline SVector operator/=(F32 a);

		inline SVector operator+=(const SVector& other);
		inline SVector operator-=(const SVector& other);
		inline SVector operator*=(const SVector& other);
		inline SVector operator/=(const SVector& other);

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

		inline void Normalize();
		inline SVector GetNormalized() const;
		inline bool IsNormalized() const;

		inline bool IsEqual(const SVector& other, F32 tolerance = VECTOR_COMPARISON_EPSILON) const;

		inline void ToDirectionAndLength(SVector& OutDirection, F32& outLength) const;

		inline F32 Distance(const SVector& other) const;
		inline F32 DistanceSquared(const SVector& other) const;
		inline F32 Distance2D(const SVector& other) const;
		inline F32 DistanceSquared2D(const SVector& other) const;

		inline SVector Projection(const SVector& other) const;
		inline SVector Mirrored(const SVector& mirrorNormal) const;

		inline static SVector Random(const SVector& lowerBound, const SVector& upperBound);

		inline std::string ToString();
	};

	SVector::SVector() : X(0), Y(0), Z(0) {}
	SVector::SVector(F32 a) : X(a), Y(a), Z(a) {}
	SVector::SVector(F32 x, F32 y) : X(x), Y(y), Z(0) {}
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

	inline SVector SVector::operator/(F32 a) const
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

	inline SVector SVector::operator/(const SVector& other) const
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

	inline SVector SVector::operator/=(F32 a)
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

	inline SVector SVector::operator/=(const SVector& other)
	{
		// TODO.NR: Print out error message about div with zero 
		if (other.X == 0 || other.Y == 0 || other.Z == 0)
			return SVector();

		X /= other.X; Y /= other.Y; Z /= other.Z;
		return *this;
	}

	inline bool SVector::operator==(const SVector& other) const
	{
		return X == other.X && Y == other.Y && Z == other.Z;
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

	inline void SVector::Normalize()
	{
		F32 length = this->Length();
		*this /= length;
	}

	inline SVector SVector::GetNormalized() const
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
		const SVector& direction = other.GetNormalized();

		F32 length = this->Dot(direction);
		return direction * length;
	}

	inline SVector SVector::Mirrored(const SVector& mirrorNormal) const
	{
		return *this - mirrorNormal * (2.0f * (this->Dot(mirrorNormal)));
	}

	inline SVector operator*(F32 a, const SVector& vector)
	{
		SVector newVector = vector;
		newVector *= a;
		return newVector;
	}

	// Returns a vector with components randomized in the range given by lower and upper bounds
	inline SVector SVector::Random(const SVector& lowerBound, const SVector& upperBound)
	{
		F32 x = UMath::Random(lowerBound.X, upperBound.X);
		F32 y = UMath::Random(lowerBound.Y, upperBound.Y);
		F32 z = UMath::Random(lowerBound.Z, upperBound.Z);

		return SVector(x, y, z);
	}

	inline std::string SVector::ToString()
	{
		char buffer[32];
		sprintf_s(buffer, "{X: %.1f, Y: %.1f, Z: %.1f}", X, Y, Z);
		return buffer;
	}
}

namespace Havtorn
{
	template<typename T>
	struct SVector2
	{
		T X, Y;

		static const SVector2 Zero;

		static const SVector2 Right;
		static const SVector2 Up;

		static const SVector2 Left;
		static const SVector2 Down;

		inline SVector2();
		inline SVector2(T a);
		inline SVector2(T x, T y);
		inline SVector2(const SVector2& other) = default;
		inline ~SVector2() = default;

		inline SVector2 operator+(T a) const;
		inline SVector2 operator-(T a) const;
		inline SVector2 operator*(T a) const;
		inline SVector2 operator/(T a) const;

		inline SVector2 operator+(const SVector2& other) const;
		inline SVector2 operator-(const SVector2& other) const;
		inline SVector2 operator*(const SVector2& other) const;
		inline SVector2 operator/(const SVector2& other) const;

		inline SVector2 operator+=(T a);
		inline SVector2 operator-=(T a);
		inline SVector2 operator*=(T a);
		inline SVector2 operator/=(T a);

		inline SVector2 operator+=(const SVector2& other);
		inline SVector2 operator-=(const SVector2& other);
		inline SVector2 operator*=(const SVector2& other);
		inline SVector2 operator/=(const SVector2& other);

		inline bool operator==(const SVector2& other) const;
		inline bool operator!=(const SVector2& other) const;

		inline SVector2 operator-() const;

		inline T Dot(const SVector2& other) const;

		inline T Length() const;
		inline T LengthSquared() const;
		inline T Size() const;
		inline T SizeSquared() const;

		inline void Normalize();
		inline SVector2 GetNormalized() const;
		inline bool IsNormalized() const;

		inline bool IsEqual(const SVector2& other, T tolerance = VECTOR_COMPARISON_EPSILON) const;

		//inline void ToDirectionAndLength(SVector2& OutDirection, F32& outLength) const;

		inline T Distance(const SVector2& other) const;
		inline T DistanceSquared(const SVector2& other) const;

		inline SVector2 Projection(const SVector2& other) const;
		inline SVector2 Mirrored(const SVector2& mirrorNormal) const;

		inline std::string ToString();
	};

	template<typename T>
	const SVector2<T> SVector2<T>::Zero = SVector2<T>(0, 0);
	
	template<typename T>
	const SVector2<T> SVector2<T>::Right = SVector2<T>(1, 0);
	template<typename T>
	const SVector2<T> SVector2<T>::Up = SVector2<T>(0, 1);

	template<typename T>
	const SVector2<T> SVector2<T>::Left = SVector2<T>(-1, 0);;
	template<typename T>
	const SVector2<T> SVector2<T>::Down = SVector2<T>(0, -1);

	template<typename T>
	SVector2<T>::SVector2() : X(0), Y(0) {}
	template<typename T>
	SVector2<T>::SVector2(T a) : X(a), Y(a) {}
	template<typename T>
	SVector2<T>::SVector2(T x, T y) : X(x), Y(y) {}

	template<typename T>
	inline SVector2<T> SVector2<T>::operator+(T a) const
	{
		return SVector2(X + a, Y + a);
	}

	template<typename T>
	inline SVector2<T> SVector2<T>::operator-(T a) const
	{
		return SVector2(X - a, Y - a);
	}

	template<typename T>
	inline SVector2<T> SVector2<T>::operator*(T a) const
	{
		return SVector2(X * a, Y * a);
	}

	template<typename T>
	inline SVector2<T> SVector2<T>::operator/(T a) const
	{
		// TODO.NR: Print out error message about div with zero
		if (a == 0)
			return SVector2();

		const T scale = 1 / a;
		return SVector2(X * scale, Y * scale);
	}

	template<typename T>
	inline SVector2<T> SVector2<T>::operator+(const SVector2& other) const
	{
		return SVector2(X + other.X, Y + other.Y);
	}

	template<typename T>
	inline SVector2<T> SVector2<T>::operator-(const SVector2& other) const
	{
		return SVector2(X - other.X, Y - other.Y);
	}

	// Hadamard multiplication
	template<typename T>
	inline SVector2<T> SVector2<T>::operator*(const SVector2& other) const
	{
		return SVector2(X * other.X, Y * other.Y);
	}

	template<typename T>
	inline SVector2<T> SVector2<T>::operator/(const SVector2& other) const
	{
		// TODO.NR: Print out error message about div with zero
		if (other.X == 0 || other.Y == 0)
			return SVector2();

		return SVector2(X / other.X, Y / other.Y);
	}

	template<typename T>
	inline SVector2<T> SVector2<T>::operator+=(T a)
	{
		X += a; Y += a;;
		return *this;
	}

	template<typename T>
	inline SVector2<T> SVector2<T>::operator-=(T a)
	{
		X -= a; Y -= a;
		return *this;
	}

	template<typename T>
	inline SVector2<T> SVector2<T>::operator*=(T a)
	{
		X *= a; Y *= a;
		return *this;
	}

	template<typename T>
	inline SVector2<T> SVector2<T>::operator/=(T a)
	{
		// TODO.NR: Print out error message about div with zero
		if (a == 0)
			return SVector2();

		F32 scale = 1 / a;
		X *= scale; Y *= scale;
		return *this;
	}

	template<typename T>
	inline SVector2<T> SVector2<T>::operator+=(const SVector2& other)
	{
		X += other.X; Y += other.Y;
		return *this;
	}

	template<typename T>
	inline SVector2<T> SVector2<T>::operator-=(const SVector2& other)
	{
		X -= other.X; Y -= other.Y;
		return *this;
	}

	// Hadamard multiplication
	template<typename T>
	inline SVector2<T> SVector2<T>::operator*=(const SVector2& other)
	{
		X *= other.X; Y *= other.Y;
		return *this;
	}

	template<typename T>
	inline SVector2<T> SVector2<T>::operator/=(const SVector2& other)
	{
		// TODO.NR: Print out error message about div with zero 
		if (other.X == 0 || other.Y == 0)
			return SVector2();

		X /= other.X; Y /= other.Y;
		return *this;
	}

	template<typename T>
	inline bool SVector2<T>::operator==(const SVector2& other) const
	{
		return X == other.X && Y == other.Y;
	}

	template<typename T>
	inline bool SVector2<T>::operator!=(const SVector2& other) const
	{
		return X != other.X || Y != other.Y;
	}

	template<typename T>
	inline SVector2<T> SVector2<T>::operator-() const
	{
		return SVector2(-X, -Y);
	}

	template<typename T>
	inline T SVector2<T>::Dot(const SVector2& other) const
	{
		return X * other.X + Y * other.Y;
	}

	template<typename T>
	inline T SVector2<T>::Length() const
	{
		return UMath::Sqrt(X * X + Y * Y);
	}

	template<typename T>
	inline T SVector2<T>::LengthSquared() const
	{
		return (X * X + Y * Y);
	}

	template<typename T>
	inline T SVector2<T>::Size() const
	{
		return this->Length();
	}

	template<typename T>
	inline T SVector2<T>::SizeSquared() const
	{
		return this->SizeSquared();
	}

	template<typename T>
	inline void SVector2<T>::Normalize()
	{
		T length = this->Length();
		*this /= length;
	}

	template<typename T>
	inline SVector2<T> SVector2<T>::GetNormalized() const
	{
		T length = this->Length();
		return SVector2(*this) / length;
	}

	template<typename T>
	inline bool SVector2<T>::IsNormalized() const
	{
		if (typeid(T) == typeid(float))
		{
			return UMath::Abs(1 - SizeSquared()) < VECTOR_NORMALIZED_EPSILON;
		}
		else
		{
			return UMath::Abs(1 - SizeSquared()) < 1;
		}
	}

	template<typename T>
	inline bool SVector2<T>::IsEqual(const SVector2& other, T tolerance) const
	{
		return UMath::Abs(X - other.X) <= tolerance && UMath::Abs(Y - other.Y) <= tolerance;
	}

	template<typename T>
	inline T SVector2<T>::Distance(const SVector2& other) const
	{
		return UMath::Sqrt(this->DistanceSquared(other));
	}

	template<typename T>
	inline T SVector2<T>::DistanceSquared(const SVector2& other) const
	{
		return UMath::Square(X - other.X) + UMath::Square(Y - other.Y);
	}

	template<typename T>
	inline SVector2<T> SVector2<T>::Projection(const SVector2& other) const
	{
		const SVector2& direction = other.GetNormalized();

		F32 length = this->Dot(direction);
		return direction * length;
	}

	template<typename T>
	inline SVector2<T> SVector2<T>::Mirrored(const SVector2& mirrorNormal) const
	{
		return *this - mirrorNormal * (2 * (this->Dot(mirrorNormal)));
	}

	template<typename T>
	inline SVector2<T> operator*(T a, const SVector2<T>& vector)
	{
		SVector2 newVector = vector;
		newVector *= a;
		return newVector;
	}

	template<typename T>
	inline std::string SVector2<T>::ToString()
	{
		char buffer[32];
		sprintf_s(buffer, "{X: %.1f, Y: %.1f}", X, Y);
		return buffer;
	}
}

namespace Havtorn
{

#define VECTOR_COMPARISON_EPSILON 1.e-4f
#define VECTOR_NORMALIZED_EPSILON 1.e-1f

	struct SVector4
	{
		F32 X, Y, Z, W;

		static const SVector4 Zero;

		static const SVector4 Right;
		static const SVector4 Up;
		static const SVector4 Forward;

		static const SVector4 Left;
		static const SVector4 Down;
		static const SVector4 Backward;

		inline SVector4();
		inline SVector4(F32 a);
		inline SVector4(const SVector& vec, F32 w);
		inline SVector4(F32 x, F32 y, F32 z, F32 w);
		SVector4(const SVector4& other) = default;
		~SVector4() = default;

		inline SVector4 operator+(F32 a) const;
		inline SVector4 operator-(F32 a) const;
		inline SVector4 operator*(F32 a) const;
		inline SVector4 operator/(F32 a) const;

		inline SVector4 operator+(const SVector4& other) const;
		inline SVector4 operator-(const SVector4& other) const;
		inline SVector4 operator*(const SVector4& other) const;
		inline SVector4 operator/(const SVector4& other) const;

		inline SVector4 operator+=(F32 a);
		inline SVector4 operator-=(F32 a);
		inline SVector4 operator*=(F32 a);
		inline SVector4 operator/=(F32 a);

		inline SVector4 operator+=(const SVector4& other);
		inline SVector4 operator-=(const SVector4& other);
		inline SVector4 operator*=(const SVector4& other);
		inline SVector4 operator/=(const SVector4& other);

		inline SVector4 operator*=(const struct SMatrix& other);

		inline bool operator==(const SVector4& other) const;
		inline bool operator!=(const SVector4& other) const;

		inline SVector4 operator-() const;

		inline F32 Dot(const SVector4& other) const;
		inline SVector4 Cross(const SVector4& other) const;

		inline F32 Length() const;
		inline F32 LengthSquared() const;
		inline F32 Size() const;
		inline F32 SizeSquared() const;

		inline F32 Length2D() const;
		inline F32 LengthSquared2D() const;
		inline F32 Size2D() const;
		inline F32 SizeSquared2D() const;

		inline void Normalize();
		inline SVector4 GetNormalized() const;
		inline bool IsNormalized() const;

		inline bool IsEqual(const SVector4& other, F32 tolerance = VECTOR_COMPARISON_EPSILON) const;

		inline bool IsPosition() const;
		inline bool IsDirection() const;

		inline void ToDirectionAndLength(SVector4& OutDirection, F32& outLength) const;

		inline F32 Distance(const SVector4& other) const;
		inline F32 DistanceSquared(const SVector4& other) const;
		inline F32 Distance2D(const SVector4& other) const;
		inline F32 DistanceSquared2D(const SVector4& other) const;

		inline SVector4 Projection(const SVector4& other) const;
		inline SVector4 Mirrored(const SVector4& mirrorNormal) const;

		static inline SVector4 Random(SVector& lowerBound, SVector& upperBound, F32 w);

		inline std::string ToString();
	};

	SVector4::SVector4() : X(0), Y(0), Z(0), W(0) {}
	SVector4::SVector4(F32 a) : X(a), Y(a), Z(a), W(0) {}
	SVector4::SVector4(const SVector& vec, F32 w) : X(vec.X), Y(vec.Y), Z(vec.Z), W(w) {}
	SVector4::SVector4(F32 x, F32 y, F32 z, F32 w) : X(x), Y(y), Z(z), W(w) {}

	inline SVector4 SVector4::operator+(F32 a) const
	{
		return SVector4(X + a, Y + a, Z + a, W);
	}

	inline SVector4 SVector4::operator-(F32 a) const
	{
		return SVector4(X - a, Y - a, Z - a, W);
	}

	inline SVector4 SVector4::operator*(F32 a) const
	{
		return SVector4(X * a, Y * a, Z * a, W);
	}

	inline SVector4 SVector4::operator/(F32 a) const
	{
		// TODO.NR: Print out error message about div with zero
		if (a == 0)
			return SVector4();

		const F32 scale = 1 / a;
		return SVector4(X * scale, Y * scale, Z * scale, W);
	}

	// Never add two points (W=1, positions) together
	inline SVector4 SVector4::operator+(const SVector4& other) const
	{
		return SVector4(X + other.X, Y + other.Y, Z + other.Z, W + other.W);
	}

	// Should become a direction (W=0) when taking one position minus another
	inline SVector4 SVector4::operator-(const SVector4& other) const
	{
		return SVector4(X - other.X, Y - other.Y, Z - other.Z, W - other.W);
	}

	// Hadamard multiplication
	inline SVector4 SVector4::operator*(const SVector4& other) const
	{
		return SVector4(X * other.X, Y * other.Y, Z * other.Z, W);
	}

	inline SVector4 SVector4::operator/(const SVector4& other) const
	{
		// TODO.NR: Print out error message about div with zero
		if (other.X == 0 || other.Y == 0 || other.Z == 0)
			return SVector4();

		return SVector4(X / other.X, Y / other.Y, Z / other.Z, W);
	}

	inline SVector4 SVector4::operator+=(F32 a)
	{
		X += a; Y += a; Z += a;
		return *this;
	}

	inline SVector4 SVector4::operator-=(F32 a)
	{
		X -= a; Y -= a; Z -= a;
		return *this;
	}

	inline SVector4 SVector4::operator*=(F32 a)
	{
		X *= a; Y *= a; Z *= a;
		return *this;
	}

	inline SVector4 SVector4::operator/=(F32 a)
	{
		// TODO.NR: Print out error message about div with zero
		if (a == 0)
			return SVector4();

		F32 scale = 1 / a;
		X *= scale; Y *= scale; Z *= scale;
		return *this;
	}

	// Never add two points (W=1, positions) together
	inline SVector4 SVector4::operator+=(const SVector4& other)
	{
		X += other.X; Y += other.Y; Z += other.Z; W += other.W;
		return *this;
	}

	// Should become a direction (W=0) when taking one position minus another
	inline SVector4 SVector4::operator-=(const SVector4& other)
	{
		X -= other.X; Y -= other.Y; Z -= other.Z; W -= other.W;
		return *this;
	}

	// Hadamard multiplication
	inline SVector4 SVector4::operator*=(const SVector4& other)
	{
		X *= other.X; Y *= other.Y; Z *= other.Z;
		return *this;
	}

	inline SVector4 SVector4::operator/=(const SVector4& other)
	{
		// TODO.NR: Print out error message about div with zero 
		if (other.X == 0 || other.Y == 0 || other.Z == 0)
			return SVector4();

		X /= other.X; Y /= other.Y; Z /= other.Z;
		return *this;
	}

	inline bool SVector4::operator==(const SVector4& other) const
	{
		return X == other.X && Y == other.Y && Z == other.Z;
	}

	inline bool SVector4::operator!=(const SVector4& other) const
	{
		return X != other.X || Y != other.Y || Z != other.Z;
	}

	inline SVector4 SVector4::operator-() const
	{
		return SVector4(-X, -Y, -Z, W);
	}

	inline F32 SVector4::Dot(const SVector4& other) const
	{
		return X * other.X + Y * other.Y + Z * other.Z + W * other.W;
	}

	inline SVector4 SVector4::Cross(const SVector4& other) const
	{
		return SVector4
		(
			Y * other.Z - Z * other.Y,
			Z * other.X - X * other.Z,
			X * other.Y - Y * other.X,
			W
		);
	}

	inline F32 SVector4::Length() const
	{
		return UMath::Sqrt(X * X + Y * Y + Z * Z);
	}

	inline F32 SVector4::LengthSquared() const
	{
		return (X * X + Y * Y + Z * Z);
	}

	inline F32 SVector4::Size() const
	{
		return this->Length();
	}

	inline F32 SVector4::SizeSquared() const
	{
		return this->SizeSquared();
	}

	inline F32 SVector4::Length2D() const
	{
		return UMath::Sqrt(X * X + Y * Y);
	}

	inline F32 SVector4::LengthSquared2D() const
	{
		return (X * X + Y * Y);
	}

	inline F32 SVector4::Size2D() const
	{
		return this->Length2D();
	}

	inline F32 SVector4::SizeSquared2D() const
	{
		return this->LengthSquared2D();
	}

	inline void SVector4::Normalize()
	{
		F32 length = this->Length();
		*this /= length;
	}

	inline SVector4 SVector4::GetNormalized() const
	{
		F32 length = this->Length();
		return SVector4(*this) / length;
	}

	inline bool SVector4::IsNormalized() const
	{
		return UMath::Abs(1 - SizeSquared()) < VECTOR_NORMALIZED_EPSILON;
	}

	inline bool SVector4::IsEqual(const SVector4& other, F32 tolerance) const
	{
		return UMath::Abs(X - other.X) <= tolerance && UMath::Abs(Y - other.Y) <= tolerance && UMath::Abs(Z - other.Z) <= tolerance;
	}

	inline bool SVector4::IsPosition() const
	{
		return W == 1.0f;
	}

	inline bool SVector4::IsDirection() const
	{
		return W == 0.0f;
	}

	inline void SVector4::ToDirectionAndLength(SVector4& outDirection, F32& outLength) const
	{
		outLength = Size();
		if (outLength > SMALL_NUMBER)
		{
			F32 lengthReciprocal = 1.0f / outLength;
			outDirection = SVector4(X * lengthReciprocal, Y * lengthReciprocal, Z * lengthReciprocal, W);
		}
		else
		{
			outDirection = SVector4::Zero;
		}
	}

	inline F32 SVector4::Distance(const SVector4& other) const
	{
		return UMath::Sqrt(this->DistanceSquared(other));
	}

	inline F32 SVector4::DistanceSquared(const SVector4& other) const
	{
		return UMath::Square(X - other.X) + UMath::Square(Y - other.Y) + UMath::Square(Z - other.Z);
	}

	inline F32 SVector4::Distance2D(const SVector4& other) const
	{
		return UMath::Sqrt(this->DistanceSquared2D(other));
	}

	inline F32 SVector4::DistanceSquared2D(const SVector4& other) const
	{
		return UMath::Square(X - other.X) + UMath::Square(Y - other.Y);
	}

	inline SVector4 SVector4::Projection(const SVector4& other) const
	{
		const SVector4& direction = other.GetNormalized();

		F32 length = this->Dot(direction);
		return direction * length;
	}

	inline SVector4 SVector4::Mirrored(const SVector4& mirrorNormal) const
	{
		return *this - mirrorNormal * (2.0f * (this->Dot(mirrorNormal)));
	}

	inline SVector4 operator*(F32 a, const SVector4& vector)
	{
		SVector4 newVector = vector;
		newVector *= a;
		return newVector;
	}

	inline SVector4 SVector4::Random(SVector& lowerBound, SVector& upperBound, F32 w)
	{
		SVector vector3 = SVector::Random(lowerBound, upperBound);
		return SVector4(vector3.X, vector3.Y, vector3.Z, w);
	}

	inline std::string SVector4::ToString()
	{
		char buffer[64];
		sprintf_s(buffer, "{X: %.1f, Y: %.1f, Z: %.1f, W: %.1f}", X, Y, Z, W);
		return buffer;
	}
}
