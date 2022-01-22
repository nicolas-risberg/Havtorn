// Copyright 2022 Team Havtorn. All Rights Reserved.

#pragma once

#include "Vector.h"

namespace Havtorn 
{
	class Sphere 
	{
	public:
		// Default constructor: there is no sphere, the radius is zero and the position is
		// the zero vector.
		Sphere();
		// Copy constructor.
		Sphere(const Sphere& sphere);
		// Constructor that takes the center position and radius of the sphere.
		Sphere(const SVector& center, F32 radius);
		// Init the sphere with a center and a radius, the same as the constructor above.
		void Init(const SVector& center, F32 radius);
		// Returns whether a point is inside the sphere: it is inside when the point is on the
		// sphere surface or inside of the sphere.
		bool IsInside(const SVector& position) const;
		
		SVector Center;
		F32 Radius;
	};

	Sphere::Sphere() : Center(SVector()), Radius(0) {}

	Sphere::Sphere(const Sphere& sphere) 
	{
		Center = sphere.Center;
		Radius = sphere.Radius;
	}

	Sphere::Sphere(const SVector& center, F32 radius) 
	{
		Center = center;
		Radius = radius;
	}

	void Sphere::Init(const SVector& center, F32 radius) 
	{
		Center = center;
		Radius = radius;
	}

	bool Sphere::IsInside(const SVector& position) const 
	{
		F32 distanceSquared = (position.X - Center.X) * (position.X - Center.X) + (position.Y - Center.Y) * (position.Y - Center.Y) + (position.Z - Center.Z) * (position.Z - Center.Z);
		return distanceSquared <= Radius * Radius;
	}
}
