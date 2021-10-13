#pragma once
#include <complex>

namespace NewEngine
{
	struct UMath
	{
		static const float Pi;
		static const float Tau;

		template<typename T>
		static inline T Sqrt(const T& x);
		template<typename T>
		static inline T Square(const T& x);

		/** Checks if x is within the range, exclusive on max. */
		template<typename T>
		static inline bool IsWithin(const T& x, const T& minValue, const T& maxValue);
		
		/** Clamps x to be between minValue and maxValue, inclusive on max. */
		template<typename T>
		static inline T Clamp(const T& x, const T& minValue = 0, const T& maxValue = 1);

		template<typename T>
		static inline T Lerp(const T& a, const T& b, const T& t);
		template<typename T>
		static inline T InverseLerp(const T& a, const T& b, const T& t);

		/** Remaps value from inRange to outRange. */
		template<typename T>
		static inline T Remap(const T& inRangeMin, const T& inRangeMax, const T& outRangeMin, const T& outRangeMax, const T& value);

		template<typename T>
		static inline T Sin(const T& angle);
		template<typename T>
		static inline T Cos(const T& angle);

		template<typename T>
		static inline T DegToRad(const T& angleInDegrees);
		template<typename T>
		static inline T RadToDeg(const T& angleInRadians);
	};

	const float UMath::Pi = 3.14159265359f;
	const float UMath::Tau = 6.28318530718f;

	template<typename T>
	inline T UMath::Sqrt(const T& x)
	{
		return std::sqrt(x);
	}

	template<typename T>
	inline T UMath::Square(const T& x)
	{
		return x * x;
	}

	template<typename T>
	inline bool UMath::IsWithin(const T& x, const T& minValue, const T& maxValue)
	{
		return ((x >= minValue) && (x < maxValue));
	}

	template<typename T>
	inline T UMath::Clamp(const T& x, const T& minValue, const T& maxValue)
	{
		return x < minValue ? minValue : x < maxValue ? x : maxValue;
	}

	template<typename T>
	inline T UMath::Lerp(const T& a, const T& b, const T& t)
	{
		return a * (1 - t) + (b * t);
	}
	
	template<typename T>
	inline T UMath::InverseLerp(const T& a, const T& b, const T& t)
	{
		return (t - a) / (b - a);
	}

	template<typename T>
	inline T UMath::Remap(const T& inRangeMin, const T& inRangeMax, const T& outRangeMin, const T& outRangeMax, const T& value)
	{
		T t = InverseLerp(inRangeMin, inRangeMax, value);
		return Lerp(outRangeMin, outRangeMax, t);
	}

	template<typename T>
	inline T Sin(const T& angle)
	{
		return std::sin(angle);
	}

	template<typename T>
	inline T Cos(const T& angle)
	{
		return std::cos(angle);
	}

	template<typename T>
	static inline T DegToRad(const T& angleInDegrees)
	{
		return angleInDegrees * (UMath::Pi / 180);
	}

	template<typename T>
	static inline T RadToDeg(const T& angleInRadians)
	{
		return angleInRadians * (180 / UMath::Pi);
	}
}
