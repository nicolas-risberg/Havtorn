// Copyright 2022 Team Havtorn. All Rights Reserved.

#pragma once

#include "Vector.h"

namespace Havtorn 
{
	class AABB3D 
	{
	public:
		// Default constructor: there is no AABB, both min and max points are the zero vector.
		AABB3D();
		// Copy constructor.
		AABB3D(const AABB3D& AABB3D);
		// Constructor taking the positions of the minimum and maximum corners.
		AABB3D(const SVector& min, const SVector& max);
		// Init the AABB with the positions of the minimum and maximum corners, same as
		// the constructor above.
		void Init(const SVector& min, const SVector& max);
		// Returns whether a point is inside the AABB: it is inside when the point is on any
		// of the AABB's sides or inside of the AABB.
		bool IsInside(const SVector& position) const;

		SVector Min;
		SVector Max;
	};

	AABB3D::AABB3D() : Min(SVector()), Max(SVector()) {}

	AABB3D::AABB3D(const AABB3D& AABB3D) 
	{
		Min = AABB3D.Min;
		Max = AABB3D.Max;
	}

	AABB3D::AABB3D(const SVector& min, const SVector& max) 
	{
		Min = min;
		Max = max;
	}

	void AABB3D::Init(const SVector& min, const SVector& max) 
	{
		Min = min;
		Max = max;
	}

	bool AABB3D::IsInside(const SVector& position) const 
	{
		return (((position.X >= Min.X) && (position.X <= Max.X)) && ((position.Y >= Min.Y) && (position.Y <= Max.Y)) && ((position.Z >= Min.Z) && (position.Z <= Max.Z)));
	}
}
