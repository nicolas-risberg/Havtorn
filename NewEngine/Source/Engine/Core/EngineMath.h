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
		static inline T Sqrt(T x);
		template<typename T>
		static inline T Square(T x);

		/** Checks if x is within the range, exclusive on max. */
		template<typename T>
		static inline bool IsWithin(T x, T minValue, T maxValue);
		
		/** Clamps x to be between minValue and maxValue, inclusive on max. */
		template<typename T>
		static inline T Clamp(T x, T minValue = 0, T maxValue = 1);

		template<typename T>
		static inline T Abs(T x);

		template<typename T>
		static inline T Lerp(T a, T b, T t);
		template<typename T>
		static inline T InverseLerp(T a, T b, T t);

		/** Remaps value from inRange to outRange. */
		template<typename T>
		static inline T Remap(T inRangeMin, T inRangeMax, T outRangeMin, T outRangeMax, T value);

		template<typename T>
		static inline T Sin(T angle);
		template<typename T>
		static inline T Cos(T angle);

		template<typename T>
		static inline T DegToRad(T angleInDegrees);
		template<typename T>
		static inline T RadToDeg(T angleInRadians);
	};

	const F32 UMath::Pi = 3.14159265359f;
	const F32 UMath::Tau = 6.28318530718f;

	const F32 UMath::PiReciprocal = 0.31830988618f;
	const F32 UMath::TauReciprocal = 0.15915494309f;

	template<typename T>
	inline T UMath::Sqrt(T x)
	{
		return std::sqrt(x);
	}

	template<typename T>
	inline T UMath::Square(T x)
	{
		return x * x;
	}

	template<typename T>
	inline bool UMath::IsWithin(T x, T minValue, T maxValue)
	{
		return ((x >= minValue) && (x < maxValue));
	}

	template<typename T>
	inline T UMath::Clamp(T x, T minValue, T maxValue)
	{
		return x < minValue ? minValue : x < maxValue ? x : maxValue;
	}

	template<typename T>
	inline T UMath::Abs(T x)
	{
		return std::abs(x);
	}

	template<typename T>
	inline T UMath::Lerp(T a, T b, T t)
	{
		return a * (1 - t) + (b * t);
	}
	
	template<typename T>
	inline T UMath::InverseLerp(T a, T b, T t)
	{
		return (t - a) / (b - a);
	}

	template<typename T>
	inline T UMath::Remap(T inRangeMin, T inRangeMax, T outRangeMin, T outRangeMax, T value)
	{
		T t = InverseLerp(inRangeMin, inRangeMax, value);
		return Lerp(outRangeMin, outRangeMax, t);
	}

	template<typename T>
	inline T UMath::Sin(T angle)
	{
		return std::sin(angle);
	}

	template<typename T>
	inline T UMath::Cos(T angle)
	{
		return std::cos(angle);
	}

	template<typename T>
	inline T UMath::DegToRad(T angleInDegrees)
	{
		return angleInDegrees * (UMath::Pi * DEGREES_TO_RADIANS_RECIPROCAL);
	}

	template<typename T>
	inline T UMath::RadToDeg(T angleInRadians)
	{
		return angleInRadians * (180 * UMath::PiReciprocal);
	}
}
