#pragma once
#include "EngineMath.h"
#include "Vector.h"

namespace Havtorn
{
	struct ICurve
	{
		virtual SVector GetPoint(float t) = 0;
	};

	struct SQuadraticBezier : public ICurve
	{
		SVector GetPoint(float t) override
		{
			F32 tt = t * t;
			
			return
				P2 * (tt)
		      + P1 * (2*t - tt)
			  + P0 * (1 - 2*t + tt)
			;
		}

		SQuadraticBezier(const SVector& p0, const SVector& p1, const SVector& p2)
			: P0(p0), P1(p1), P2(p2)
		{}

		SVector P0, P1, P2;
	};

	struct SCubicBezier : public ICurve
	{
		SVector GetPoint(float t) override
		{
			F32 tt = t * t;
			F32 ttt = t * t * t;

			return
				P3 * (ttt)
			  + P2 * (3*tt - 3*ttt)
			  + P1 * (3*t - 6*tt + 3*ttt)
			  + P0 * (1 - 3*t + 3*tt - ttt)
			;
		}

		SCubicBezier(const SVector& p0, const SVector& p1, const SVector& p2, const SVector& p3)
			: P0(p0), P1(p1), P2(p2), P3(p3)
		{}

		SVector P0, P1, P2, P3;
	};
}