#pragma once
#include <complex>

namespace NewEngine
{
#define VECTOR_COMPARISON_EPSILON 1.e-4;

	template<typename T>
	struct SVector3
	{
		T X, Y, Z;

		static const SVector3 Right;
		static const SVector3 Up;
		static const SVector3 Forward;

		static const SVector3 Left;
		static const SVector3 Down;
		static const SVector3 Backward;

		inline SVector3();
		inline SVector3(T a);
		inline SVector3(T x, T y, T z);
		SVector3(const SVector3& other) = default;
		~SVector3() = default;

		inline SVector3 operator+(T a) const;
		inline SVector3 operator-(T a) const;
		inline SVector3 operator*(T a) const;
			   SVector3 operator/(T a) const;
		
		inline SVector3 operator+(const SVector3& other) const;
		inline SVector3 operator-(const SVector3& other) const;
		inline SVector3 operator*(const SVector3& other) const;
			   SVector3 operator/(const SVector3& other) const;

		inline SVector3 operator+=(T a) const;
		inline SVector3 operator-=(T a) const;
		inline SVector3 operator*=(T a) const;
			   SVector3 operator/=(T a) const;

		inline SVector3 operator+=(const SVector3& other) const;
		inline SVector3 operator-=(const SVector3& other) const;
		inline SVector3 operator*=(const SVector3& other) const;
			   SVector3 operator/=(const SVector3& other) const;

		inline bool operator==(const SVector3& other) const;
		inline bool operator!=(const SVector3& other) const;

		inline T Dot(const SVector3& other) const;
		inline SVector3 Cross(const SVector3& other) const;

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
	};

	template<typename T>
	const SVector3<T> SVector3<T>::Right	= SVector3<T>(1, 0, 0);
	template<typename T>
	const SVector3<T> SVector3<T>::Up		= SVector3<T>(0, 1, 0);
	template<typename T>
	const SVector3<T> SVector3<T>::Forward	= SVector3<T>(0, 0, 1);

	template<typename T>
	const SVector3<T> SVector3<T>::Left		= SVector3<T>(-1, 0, 0);;
	template<typename T>
	const SVector3<T> SVector3<T>::Down		= SVector3<T>(0, -1, 0);
	template<typename T>
	const SVector3<T> SVector3<T>::Backward	= SVector3<T>(0, 0, -1);

	template<typename T>
	SVector3<T>::SVector3() : X(0), Y(0), Z(0) {}
	template<typename T>
	SVector3<T>::SVector3(T a) : X(a), Y(a), Z(a) {}
	template<typename T>
	SVector3<T>::SVector3(T x, T y, T z) : X(x), Y(y), Z(z) {}
	
	template<typename T>
	inline SVector3<T> SVector3<T>::operator+(T a) const
	{
		return SVector3(X + a, Y + a, Z + a);
	}

	template<typename T>
	inline SVector3<T> SVector3<T>::operator-(T a) const
	{
		return SVector3(X - a, Y - a, Z - a);
	}

	template<typename T>
	inline SVector3<T> SVector3<T>::operator*(T a) const
	{
		return SVector3(X * a, Y * a, Z * a);
	}

	template<typename T>
	SVector3<T> SVector3<T>::operator/(T a) const
	{
		// TODO.NR: Print out error message about div with zero
		if (a == 0)
			return SVector3();

		const T scale = 1 / a;
		return SVector3(X * scale, Y * scale, Z * scale);
	}

	template<typename T>
	inline SVector3<T> SVector3<T>::operator+(const SVector3& other) const
	{
		return SVector3(X + other.X, Y + other.Y, Z + other.Z);
	}

	template<typename T>
	inline SVector3<T> SVector3<T>::operator-(const SVector3& other) const
	{
		return SVector3(X - other.X, Y - other.Y, Z - other.Z);
	}

	// Hadamard multiplication
	template<typename T>
	inline SVector3<T> SVector3<T>::operator*(const SVector3& other) const
	{
		return SVector3(X * other.X, Y *= other.Y, Z *= other.Z);
	}

	template<typename T>
	SVector3<T> SVector3<T>::operator/(const SVector3& other) const
	{
		// TODO.NR: Print out error message about div with zero
		if (other.X == 0 || other.Y == 0 || other.Z == 0)
			return SVector3();

		return SVector3(X / other.X, Y / other.Y, Z / other.Z);
	}

	template<typename T>
	inline SVector3<T> SVector3<T>::operator+=(T a) const
	{
		X += a; Y += a; Z += a;
		return *this;
	}

	template<typename T>
	inline SVector3<T> SVector3<T>::operator-=(T a) const
	{
		X -= a; Y -= a; Z -= a;
		return *this;
	}

	template<typename T>
	inline SVector3<T> SVector3<T>::operator*=(T a) const
	{
		X *= a; Y *= a; Z *= a;
		return *this;
	}

	template<typename T>
	SVector3<T> SVector3<T>::operator/=(T a) const
	{
		// TODO.NR: Print out error message about div with zero
		if (a == 0)
			return SVector3();

		T scale = 1 / a;
		X *= scale; Y *= scale; Z *= scale;
		return *this;
	}

	template<typename T>
	inline SVector3<T> SVector3<T>::operator+=(const SVector3& other) const
	{
		X += other.X; Y += other.Y; Z += other.Z;
		return *this;
	}

	template<typename T>
	inline SVector3<T> SVector3<T>::operator-=(const SVector3& other) const
	{
		X -= other.X; Y -= other.Y; Z -= other.Z;
		return *this;
	}

	// Hadamard multiplication
	template<typename T>
	inline SVector3<T> SVector3<T>::operator*=(const SVector3& other) const
	{
		X *= other.X; Y *= other.Y; Z *= other.Z;
		return *this;
	}

	template<typename T>
	SVector3<T> SVector3<T>::operator/=(const SVector3& other) const
	{
		// TODO.NR: Print out error message about div with zero 
		if (other.X == 0 || other.Y == 0 || other.Z == 0)
			return SVector3();

		X /= other.X; Y /= other.Y; Z /= other.Z;
		return *this;
	}

	template<typename T>
	inline bool SVector3<T>::operator==(const SVector3& other) const
	{
		return X == other.X && Y == other.Y && Z == other.Y;
	}

	template<typename T>
	inline bool SVector3<T>::operator!=(const SVector3& other) const
	{
		return X != other.X || Y != other.Y || Z != other.Z;
	}

	template<typename T>
	inline T SVector3<T>::Dot(const SVector3& other) const
	{
		return X * other.X + Y * other.Y + Z * other.Z;
	}

	template<typename T>
	inline SVector3<T> SVector3<T>::Cross(const SVector3& other) const
	{
		return SVector3
		(
			Y * other.Z - Z * other.Y,
			Z * other.X - X * other.Z,
			X * other.Y - Y * other.X
		);
	}

	template<typename T> 
	SVector3<T> operator*(T a, const SVector3<T>& vector)
	{
		SVector3 newVector = vector;
		newVector *= a;
		return newVector;
	}
}