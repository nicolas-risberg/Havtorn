#pragma once
#include <complex>
#include "CoreTypes.h"
#include "EngineMathSSE.h"

namespace NewEngine
{

#define SMALL_NUMBER 1.e-8f
#define DEGREES_TO_RADIANS_RECIPROCAL 0.00555555555f // 1 / 180

	struct UMath
	{
		static const F32 Pi;
		static const F32 Tau;

		static const F32 PiReciprocal;
		static const F32 TauReciprocal;

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
		static inline T Abs(const T& x);

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

	const F32 UMath::Pi = 3.14159265359f;
	const F32 UMath::Tau = 6.28318530718f;

	const F32 UMath::PiReciprocal = 0.31830988618f;
	const F32 UMath::TauReciprocal = 0.15915494309f;

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
	inline T UMath::Abs(const T& x)
	{
		return std::abs(x);
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
	inline T UMath::Sin(const T& angle)
	{
		return std::sin(angle);
	}

	template<typename T>
	inline T UMath::Cos(const T& angle)
	{
		return std::cos(angle);
	}

	template<typename T>
	inline T UMath::DegToRad(const T& angleInDegrees)
	{
		return angleInDegrees * (UMath::Pi * DEGREES_TO_RADIANS_RECIPROCAL);
	}

	template<typename T>
	inline T UMath::RadToDeg(const T& angleInRadians)
	{
		return angleInRadians * (180 * UMath::PiReciprocal);
	}
}
