#pragma once
#include <cstdint>
//#include <math.h>
#include <complex>

namespace NewEngine
{
	using U8 = std::uint8_t;
	using U16 = std::uint16_t;
	using U32 = std::uint32_t;
	using U64 = std::uint64_t;

	using I8 = std::int8_t;
	using I16 = std::int16_t;
	using I32 = std::int32_t;
	using I64 = std::int64_t;

	using F32 = float;

	// Used to declare types cache aligned in CACHE_LINE bytes
#define CACHE_LINE  32
#define CACHE_ALIGN __declspec(align(CACHE_LINE))

	template<typename T>
	struct SVector3
	{
		SVector3() : X(0), Y(0), Z(0) {};
		SVector3(T a) : X(a), Y(a), Z(a) {};
		SVector3(T x, T y, T z) : X(x), Y(y), Z(z) {};
		SVector3(const SVector3& other) = default;
		~SVector3() = default;
		
		static const SVector3 Right;
		static const SVector3 Up;
		static const SVector3 Forward;

		static const SVector3 Left;
		static const SVector3 Down;
		static const SVector3 Backward;

		SVector3 operator*(T a) const
		{
			SVector3 newVector = *this;
			newVector *= a;
			return newVector;
		}

		void operator*=(T a)
		{
			X *= a;
			Y *= a;
			Z *= a;
		}

		SVector3 operator/(T a) const
		{
			SVector3 newVector = *this;
			newVector /= a;
			return newVector;
		}

		void operator/=(T a)
		{
			if (a != 0)
			{
				X /= a;
				Y /= a;
				Z /= a;
			}
		}

		// Hadamard multiplication
		const SVector3& operator*(const SVector3& other) const
		{
			return SVector3(X * other.X, Y *= other.Y, Z *= other.Z);
		}

		// Hadamard multiplication
		void operator*=(const SVector3& other)
		{
			X *= other.X;
			Y *= other.Y;
			Z *= other.Z;
		}

		void operator/=(const SVector3& other)
		{
			if (other.X != 0.0f)
				X /= other.X;
			
			if (other.Y != 0.0f)
				Y /= other.Y;
	
			if (other.Z != 0.0f)
				Z /= other.Z;
		}

		T Dot(const SVector3& other) const
		{
			return X * other.X + Y * other.Y + Z * other.Z;
		}

		const SVector3& Cross(const SVector3& other) const
		{
			SVector3 returnVector = SVector3();
			returnVector.X = Y * other.Z - Z * other.Y;
			returnVector.Y = Z * other.X - X * other.Z;
			returnVector.Z = X * other.Y - Y * other.X;

			return returnVector;
		}

		T Length() const
		{
			return std::sqrt(X * X + Y * Y + Z * Z);
		}

		T LengthSquared() const
		{
			return (X * X + Y * Y + Z * Z);
		}

		void Normalize()
		{
			T length = this->Length();
			
			*this /= length;
		}

		SVector3 Normalized() const
		{
			T length = this->Length();

			return SVector3(*this) / length;
		}

		/*
		* Gets the projection of this vector on other.
		* 
		* @param other Vector to project onto.
		*/
		SVector3 Project(const SVector3& other) const
		{
			const SVector3& direction = other.Normalized();

			T length = this->Dot(direction);
			return direction * length;
		}

		SVector3 Mirror() const
		{
			return SVector3(*this) * -1;
		}

		SVector3 Reflect() { return this; }

		T X, Y, Z;
	};

	template<typename T>
	const SVector3<T> SVector3<T>::Right		= SVector3<T>(1, 0, 0);
	template<typename T>
	const SVector3<T> SVector3<T>::Up			= SVector3<T>(0, 1, 0);
	template<typename T>
	const SVector3<T> SVector3<T>::Forward		= SVector3<T>(0, 0, 1);
						
	template<typename T>
	const SVector3<T> SVector3<T>::Left			= SVector3<T>(-1, 0, 0);;
	template<typename T>
	const SVector3<T> SVector3<T>::Down			= SVector3<T>(0, -1, 0);
	template<typename T>
	const SVector3<T> SVector3<T>::Backward		= SVector3<T>(0, 0, -1);

	template<typename T> SVector3<T> operator*(T a, const SVector3<T>& vector)
	{
		SVector3 newVector = vector;
		newVector *= a;
		return newVector;
	}
}