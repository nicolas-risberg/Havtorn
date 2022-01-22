// Copyright 2022 Team Havtorn. All Rights Reserved.

#pragma once

#include <complex>
#include <random>
#include "Core/CoreTypes.h"
#include "EngineMathSSE.h"
//#include <FastNoise/FastNoise.h>

namespace
{
	std::random_device myRandomDevice{};
	std::mt19937 myRandomEngine{ myRandomDevice() };
}

namespace Havtorn
{

#define SMALL_NUMBER 1.e-8f
#define KINDA_SMALL_NUMBER 1.e-4f
#define DEGREES_TO_RADIANS 0.01745329251f // Pi * (1 / 180)
#define RADIANS_TO_DEGREES 57.2957795131f // 180 * (1 / Pi)

	struct UMath
	{
		static const F32 Pi;
		static const F32 Tau;

		static const F32 PiReciprocal;
		static const F32 TauReciprocal;

		template<typename T>
		static inline T Sqrt(T x);
		template<typename T>
		static inline T Pow(T x, T exp);
		template<typename T>
		static inline T Square(T x);

		/* Checks if x is within the range, exclusive on max. */
		template<typename T>
		static inline bool IsWithin(T x, T minValue, T maxValue);
		
		/* Clamps x to be between minValue and maxValue, inclusive on max. */
		template<typename T>
		static inline T Clamp(T x, T minValue = 0, T maxValue = 1);

		static inline bool NearlyEqual(F32 a, F32 b, F32 tolerance = KINDA_SMALL_NUMBER);

		template<typename T>
		static inline T Abs(T x);

		template<typename T>
		static inline T Lerp(T a, T b, T t);
		template<typename T>
		static inline T InverseLerp(T a, T b, T t);

		/* Remaps value from inRange to outRange. */
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

		/* From XM Math Misc - XMScalarSinCos */
		static inline void MapFov(F32& outSinValue, F32& outCosValue, F32 halfYFovAngle);

		/* Easing Functions Start */
		static inline F32 EaseInSine(F32 x);
		static inline F32 EaseInCubic(F32 x);
		static inline F32 EaseInQuint(F32 x);
		static inline F32 EaseInCirc(F32 x);
		static inline F32 EaseInElastic(F32 x);

		static inline F32 EaseOutSine(F32 x);
		static inline F32 EaseOutCubic(F32 x);
		static inline F32 EaseOutQuint(F32 x);
		static inline F32 EaseOutCirc(F32 x);
		static inline F32 EaseOutElastic(F32 x);

		static inline F32 EaseInOutSine(F32 x);
		static inline F32 EaseInOutCubic(F32 x);
		static inline F32 EaseInOutQuint(F32 x);
		static inline F32 EaseInOutCirc(F32 x);
		static inline F32 EaseInOutElastic(F32 x);

		static inline F32 EaseInQuad(F32 x);
		static inline F32 EaseInQuart(F32 x);
		static inline F32 EaseInExpo(F32 x);
		static inline F32 EaseInBack(F32 x);
		static inline F32 EaseInBounce(F32 x);

		static inline F32 EaseOutQuad(F32 x);
		static inline F32 EaseOutQuart(F32 x);
		static inline F32 EaseOutExpo(F32 x);
		static inline F32 EaseOutBack(F32 x);
		static inline F32 EaseOutBounce(F32 x);

		static inline F32 EaseInOutQuad(F32 x);
		static inline F32 EaseInOutQuart(F32 x);
		static inline F32 EaseInOutExpo(F32 x);
		static inline F32 EaseInOutBack(F32 x);
		static inline F32 EaseInOutBounce(F32 x);
		/* Easing Functions End */

		static inline F32 UpFacingQuadCurve(F32 x);
		static inline F32 DownFacingQuadCurve(F32 x);

		static inline F32 RandomGaussian(F32 mean, F32 standardDeviation);
		static inline F32 Random(F32 lowerBound, F32 upperBound);
		static inline I32 Random(I32 lowerBound, I32 upperBound);

		static inline I32 Random(I32 lowerBound, I32 upperBound, I32 excludedNumber);
		static inline I32 Random(I32 lowerBound, I32 upperBound, std::vector<I32>& excludedNumbers);
	};

	const F32 UMath::Pi = 3.14159265359f;
	const F32 UMath::Tau = 6.28318530718f;

	const F32 UMath::PiReciprocal = 0.31830988618f;
	const F32 UMath::TauReciprocal = 0.15915494309f;

	template<typename T>
	inline T UMath::Sqrt(T x)
	{
		return static_cast<T>(std::sqrt(x));
	}

	template<typename T>
	inline T UMath::Pow(T x, T exp)
	{
		return std::pow(x, exp);
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
		T t = UMath::InverseLerp(inRangeMin, inRangeMax, value);
		return UMath::Lerp(outRangeMin, outRangeMax, t);
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
		return angleInDegrees * DEGREES_TO_RADIANS;
	}

	template<typename T>
	inline T UMath::RadToDeg(T angleInRadians)
	{
		return angleInRadians * RADIANS_TO_DEGREES;
	}

	inline bool UMath::NearlyEqual(F32 a, F32 b, F32 tolerance)
	{
		return UMath::Abs(a - b) < tolerance;
	}

	inline void UMath::MapFov(F32& outSinValue, F32& outCosValue, F32 halfYFovAngle)
	{
		// Map Value to y in [-pi,pi], x = 2*pi*quotient + remainder.
		F32 quotient = 0.5f * UMath::PiReciprocal * halfYFovAngle;
		if (halfYFovAngle >= 0.0f)
		{
			quotient = static_cast<F32>(static_cast<I32>(quotient + 0.5f));
		}
		else
		{
			quotient = static_cast<F32>(static_cast<I32>(quotient - 0.5f));
		}
		F32 y = halfYFovAngle - UMath::Tau * quotient;

		// Map y to [-pi/2,pi/2] with sin(y) = sin(Value).
		F32 sign;
		if (y > 0.5f * UMath::Pi)
		{
			y = UMath::Pi - y;
			sign = -1.0f;
		}
		else if (y < -(0.5f * UMath::Pi))
		{
			y = -UMath::Pi - y;
			sign = -1.0f;
		}
		else
		{
			sign = +1.0f;
		}

		F32 y2 = y * y;

		// 11-degree minimax approximation
		outSinValue = (((((-2.3889859e-08f * y2 + 2.7525562e-06f) * y2 - 0.00019840874f) * y2 + 0.0083333310f) * y2 - 0.16666667f) * y2 + 1.0f) * y;

		// 10-degree minimax approximation
		F32 p = ((((-2.6051615e-07f * y2 + 2.4760495e-05f) * y2 - 0.0013888378f) * y2 + 0.041666638f) * y2 - 0.5f) * y2 + 1.0f;
		outCosValue = sign * p;
	}

	/* Easing Functions Start */
	inline F32 UMath::EaseInSine(F32 x)
	{
		x = UMath::Clamp(x);
		return 1.0f - UMath::Cos(x * 0.5f * UMath::Pi);
	}
	inline F32 UMath::EaseInCubic(F32 x)
	{
		x = UMath::Clamp(x);
		return x * x * x;
	}
	inline F32 UMath::EaseInQuint(F32 x)
	{
		x = UMath::Clamp(x);
		return x * x * x * x * x;
	}
	inline F32 UMath::EaseInCirc(F32 x)
	{
		x = UMath::Clamp(x);
		return 1.0f - UMath::Sqrt(1.0f - (x * x));
	}
	inline F32 UMath::EaseInElastic(F32 x)
	{
		x = UMath::Clamp(x);
		return x * x * x * x * UMath::Sin(x * UMath::Pi * 4.5f);
	}

	inline F32 UMath::EaseOutSine(F32 x)
	{
		x = UMath::Clamp(x);
		return UMath::Sin(x * 0.5f * UMath::Pi);
	}
	inline F32 UMath::EaseOutCubic(F32 x)
	{
		x = UMath::Clamp(x);
		return 1.0f - UMath::Pow(1.0f - x, 3.0f);
	}
	inline F32 UMath::EaseOutQuint(F32 x)
	{
		x = UMath::Clamp(x);
		return 1.0f - UMath::Pow(1.0f - x, 5.0f);
	}
	inline F32 UMath::EaseOutCirc(F32 x)
	{
		x = UMath::Clamp(x);
		return UMath::Sqrt(1.0f - ((x - 1.0f) * (x - 1.0f)));
	}
	inline F32 UMath::EaseOutElastic(F32 x)
	{
		x = UMath::Clamp(x);
		return 1.0f - UMath::Pow(x - 1.0f, 4.0f) * UMath::Sin(x * UMath::Pi * 4.5f);
	}

	inline F32 UMath::EaseInOutSine(F32 x)
	{
		x = UMath::Clamp(x);
		return -(UMath::Cos(x * UMath::Pi) - 1.0f) * 0.5f;
	}
	inline F32 UMath::EaseInOutCubic(F32 x)
	{
		x = UMath::Clamp(x);
		return x < 0.5f ? 4.0f * x * x * x : 1.0f - UMath::Pow((-2.0f * x) + 2.0f, 3.0f) * 0.5f;
	}
	inline F32 UMath::EaseInOutQuint(F32 x)
	{
		x = UMath::Clamp(x);
		return x < 0.5f ? 16.0f * x * x * x * x * x : 1.0f - UMath::Pow((-2.0f * x) + 2.0f, 5.0f) * 0.5f;
	}
	inline F32 UMath::EaseInOutCirc(F32 x)
	{
		x = UMath::Clamp(x);
		return x < 0.5f
			? (1.0f - UMath::Sqrt(1.0f - UMath::Pow(2.0f * x, 2.0f))) * 0.5f
			: (UMath::Sqrt(1.0f - UMath::Pow((-2.0f * x) + 2.0f, 2.0f)) + 1.0f) * 0.5f;
	}
	inline F32 UMath::EaseInOutElastic(F32 x)
	{
		x = UMath::Clamp(x);
		F32 x2;
		if (x < 0.45f) {
			x2 = x * x;
			return 8.0f * x2 * x2 * UMath::Sin(x * UMath::Pi * 9.0f);
		}
		else if (x < 0.55f) {
			return 0.5f + 0.75f * UMath::Sin(x * UMath::Pi * 4.0f);
		}
		else {
			x2 = (x - 1.0f) * (x - 1.0f);
			return 1.0f - 8.0f * x2 * x2 * UMath::Sin(x * UMath::Pi * 9.0f);
		}
	}

	inline F32 UMath::EaseInQuad(F32 x)
	{
		x = UMath::Clamp(x);
		return x * x;
	}
	inline F32 UMath::EaseInQuart(F32 x)
	{
		x = UMath::Clamp(x);
		return x * x * x * x;
	}
	inline F32 UMath::EaseInExpo(F32 x)
	{
		x = UMath::Clamp(x);
		return x == 0.0f ? 0.0f : UMath::Pow(2.0f, 10.0f * x - 10.0f);
	}
	inline F32 UMath::EaseInBack(F32 x)
	{
		x = UMath::Clamp(x);
		const F32 c1 = 1.70158f;
		const F32 c3 = c1 + 1.0f;

		return c3 * x * x * x - c1 * x * x;
	}
	inline F32 UMath::EaseInBounce(F32 x)
	{
		x = UMath::Clamp(x);
		return UMath::Pow(2.0f, 6.0f * (x - 1.0f)) * UMath::Abs(UMath::Sin(x * UMath::Pi * 3.5f));
	}

	inline F32 UMath::EaseOutQuad(F32 x)
	{
		x = UMath::Clamp(x);
		return 1.0f - (1.0f - x) * (1.0f - x);
	}
	inline F32 UMath::EaseOutQuart(F32 x)
	{
		x = UMath::Clamp(x);
		return 1.0f - UMath::Pow(1.0f - x, 4.0f);
	}
	inline F32 UMath::EaseOutExpo(F32 x)
	{
		x = UMath::Clamp(x);
		return x == 1.0f ? 1.0f : 1.0f - UMath::Pow(2.0f, -10.0f * x);
	}
	inline F32 UMath::EaseOutBack(F32 x)
	{
		x = UMath::Clamp(x);
		const F32 c1 = 1.70158f;
		const F32 c3 = c1 + 1.0f;

		return 1.0f + c3 * UMath::Pow(x - 1.0f, 3.0f) + c1 * UMath::Pow(x - 1.0f, 2.0f);
	}
	inline F32 UMath::EaseOutBounce(F32 x)
	{
		x = UMath::Clamp(x);
		return 1.0f - UMath::Pow(2.0f, -6.0f * x) * UMath::Abs(UMath::Cos(x * UMath::Pi * 3.5f));
	}

	inline F32 UMath::EaseInOutQuad(F32 x)
	{
		x = UMath::Clamp(x);
		return x < 0.5f ? 2.0f * x * x : 1.0f - UMath::Pow(-2.0f * x + 2.0f, 2.0f) * 0.5f;
	}
	inline F32 UMath::EaseInOutQuart(F32 x)
	{
		x = UMath::Clamp(x);
		return x < 0.5f ? 8.0f * x * x * x * x : 1.0f - UMath::Pow(-2.0f * x + 2.0f, 4.0f) * 0.5f;
	}
	inline F32 UMath::EaseInOutExpo(F32 x)
	{
		x = UMath::Clamp(x);
		if (x < 0.5f) 
		{
			return (UMath::Pow(2.0f, 16.0f * x) - 1.0f) / 510.0f;
		}
		else 
		{
			return 1.0f - 0.5f * UMath::Pow(2.0f, -16.0f * (x - 0.5f));
		}
	}
	inline F32 UMath::EaseInOutBack(F32 x)
	{
		x = UMath::Clamp(x);
		const F32 c1 = 1.70158f;
		const F32 c2 = c1 * 1.525f;

		return x < 0.5f
			? (UMath::Pow(2.0f * x, 2.0f) * ((c2 + 1.0f) * 2.0f * x - c2)) * 0.5f
			: (UMath::Pow(2.0f * x - 2.0f, 2.0f) * ((c2 + 1.0f) * (x * 2.0f - 2.0f) + c2) + 2.0f) * 0.5f;
	}
	inline F32 UMath::EaseInOutBounce(F32 x)
	{
		x = UMath::Clamp(x);
		return x < 0.5f
			? (1.0f - UMath::EaseOutBounce(1.0f - 2.0f * x)) * 0.5f
			: (1.0f + UMath::EaseOutBounce(2.0f * x - 1.0f)) * 0.5f;
	}
	/* Easing Functions End */

	inline F32 UMath::UpFacingQuadCurve(F32 x)
	{
		x = UMath::Clamp(x);
		return (4.0f * x) - (4.0f * x * x);
	}
	inline F32 UMath::DownFacingQuadCurve(F32 x)
	{
		x = UMath::Clamp(x);
		return 1.0f - (4.0f * x) - (4.0f * x * x);
	}

	inline F32 UMath::RandomGaussian(F32 mean, F32 standardDeviation)
	{
		return std::normal_distribution<F32>{ mean, standardDeviation }(myRandomEngine);
	}

	inline F32 UMath::Random(F32 lowerBound, F32 upperBound)
	{
		return std::uniform_real_distribution<F32>{ lowerBound, upperBound }(myRandomEngine);
	}

	inline I32 UMath::Random(I32 lowerBound, I32 upperBound)
	{
		return std::uniform_int_distribution<>{ lowerBound, upperBound }(myRandomEngine);
	}

	inline I32 UMath::Random(I32 lowerBound, I32 upperBound, I32 excludedNumber)
	{
		I32 num = excludedNumber;

		if (upperBound - lowerBound > 0)
		{
			while (num == excludedNumber)
			{
				num = Random(lowerBound, upperBound);
			}
		}
		else
		{
			return Random(lowerBound, upperBound);
		}

		return num;
	}

	inline I32 UMath::Random(I32 lowerBound, I32 upperBound, std::vector<I32>& excludedNumbers)
	{
		if (excludedNumbers.empty())
		{
			I32 result = Random(lowerBound, upperBound);
			excludedNumbers.push_back(result);
			return result;
		}

		I32 num = excludedNumbers[0];

		if (upperBound - lowerBound > 0)
		{
			while (std::find(excludedNumbers.begin(), excludedNumbers.end(), num) != excludedNumbers.end())
			{
				num = Random(lowerBound, upperBound);
			}
			excludedNumbers.push_back(num);

			if (excludedNumbers.size() > upperBound)
			{
				excludedNumbers.clear();
			}
		}
		else
		{
			return Random(lowerBound, upperBound);
		}

		return num;
	}
}
