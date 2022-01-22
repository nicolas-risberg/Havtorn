// Copyright 2022 Team Havtorn. All Rights Reserved.

#include "hvpch.h"
#include "Curve.h"

namespace Havtorn
{
	SVector SCatmullRom::GetPoint(F32 t)
	{
		F32 tt = t * t;
		F32 ttt = t * t * t;

		return
			(P3 * (-tt + ttt)
				+ P2 * (t + 4.0f * tt - 3.0f * ttt)
				+ P1 * (2.0f - 5.0f * tt + 3.0f * ttt)
				+ P0 * (-t + 2.0f * tt - ttt)) * 0.5f
			;
	}

	SVector SCatmullRom::GetTangent(F32 t)
	{
		F32 tt = t * t;

		return
			(P3 * (-2.0f * t + 3.0f * tt)
				+ P2 * (1.0f + 8.0f * t - 9.0f * tt)
				+ P1 * (-10.0f * t + 9.0f * tt)
				+ P0 * (-1.0f + 4.0f * t - 3 * tt)) * 0.5f
			;
	}

	SVector SCatmullRom::GetNormal(F32 t)
	{
		SVector tangent = GetTangent(t);
		// TODO.NR: Rotate tangent 90 degrees for normal
		return tangent;
	}

	SCatmullRom::SCatmullRom(const SVector& p0, const SVector& p1, const SVector& p2, const SVector& p3)
		: P0(p0), P1(p1), P2(p2), P3(p3) 
	{}
}
