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
		static inline T Pow(T x, T exp);
		template<typename T>
		static inline T Square(T x);

		/* Checks if x is within the range, exclusive on max. */
		template<typename T>
		static inline bool IsWithin(T x, T minValue, T maxValue);
		
		/* Clamps x to be between minValue and maxValue, inclusive on max. */
		template<typename T>
		static inline T Clamp(T x, T minValue = 0, T maxValue = 1);

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
		return angleInDegrees * (UMath::Pi * DEGREES_TO_RADIANS_RECIPROCAL);
	}

	template<typename T>
	inline T UMath::RadToDeg(T angleInRadians)
	{
		return angleInRadians * (180 * UMath::PiReciprocal);
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
}
