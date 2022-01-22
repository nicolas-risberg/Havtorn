// Copyright 2022 Team Havtorn. All Rights Reserved.

#pragma once

#include "EngineMath.h"
#include "Vector.h"

namespace Havtorn
{
	struct ICurve
	{
		virtual SVector GetPoint(F32 t) = 0;
		virtual SVector GetTangent(F32 t) = 0;
		virtual SVector GetNormal(F32 t) = 0;
	};

	struct SQuadraticBezier : public ICurve
	{
		SVector GetPoint(F32 t) override
		{
			F32 tt = t * t;
			
			return
				P2 * (tt)
		      + P1 * (2.0f*t - tt)
			  + P0 * (1.0f - 2.0f*t + tt)
			;
		}

		SVector GetTangent(F32 t) override
		{
			return
				P2 * (2.0f*t)
			  + P1 * (2.0f - 2.0f*t)
			  + P0 * (-2.0f + 2.0f*t)
			;
		}

		SVector GetNormal(F32 t) override
		{
			SVector tangent = GetTangent(t);
			// TODO.NR: Rotate tangent 90 degrees for normal
			return tangent;
		}

		explicit SQuadraticBezier(const SVector& p0, const SVector& p1, const SVector& p2)
			: P0(p0), P1(p1), P2(p2) {}

		SVector P0, P1, P2;
	};

	struct SCubicBezier : public ICurve
	{
		SVector GetPoint(F32 t) override
		{
			F32 tt = t * t;
			F32 ttt = t * t * t;

			return
				P3 * (ttt)
			  + P2 * (3.0f*tt - 3.0f*ttt)
			  + P1 * (3.0f*t - 6.0f*tt + 3.0f*ttt)
			  + P0 * (1.0f - 3.0f*t + 3.0f*tt - ttt)
			;
		}

		SVector GetTangent(F32 t)
		{
			F32 tt = t * t;

			return
				P3 * (3.0f*tt)
			  + P2 * (6.0f*t - 9.0f*tt)
			  + P1 * (3.0f - 12.0f*t + 9.0f*tt)
			  + P0 * (-3.0f + 6.0f*t - 3.0f*tt)
				;
		}

		SVector GetNormal(F32 t) override
		{
			SVector tangent = GetTangent(t);
			// TODO.NR: Rotate tangent 90 degrees for normal
			return tangent;
		}

		explicit SCubicBezier(const SVector& p0, const SVector& p1, const SVector& p2, const SVector& p3)
			: P0(p0), P1(p1), P2(p2), P3(p3) {}

		SVector P0, P1, P2, P3;
	};

	struct SCatmullRom : public ICurve
	{
		SVector GetPoint(F32 t) override;
		SVector GetTangent(F32 t);
		SVector GetNormal(F32 t) override;

		explicit SCatmullRom(const SVector& p0, const SVector& p1, const SVector& p2, const SVector& p3);

		SVector P0, P1, P2, P3;
	};
}