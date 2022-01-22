// Copyright 2022 Team Havtorn. All Rights Reserved.

#pragma once

#include "Vector.h"

namespace Havtorn 
{
	class Ray 
	{
	public:
		// Default constructor: there is no ray, the origin and direction are the
        // zero vector.
		Ray();
		// Copy constructor.
		Ray(const Ray& ray);
		// Constructor that takes two points that define the ray, the direction is
		// aPoint - aOrigin and the origin is aOrigin.
		Ray(const SVector& origin, const SVector& point);
		// Init the ray with two points, the same as the constructor above.
		void Init(const SVector& origin, const SVector& point);
		// Init the ray with an origin and a direction.
		void Init(const SVector& origin, const SVector& direction);
		// For a given distance value from the origin, returns the corresponding point on the ray.
		SVector GetPointOnRay(F32 distanceFromOrigin);

		SVector Origin;
		SVector Direction;
	};

	Ray::Ray() : Origin(SVector()), Direction(SVector()) {}

	Ray::Ray(const Ray& ray) 
	{
		Origin = ray.Origin;
		Direction = ray.Direction;
	}

	Ray::Ray(const SVector& origin, const SVector& point) 
	{
		Origin = origin;
		Direction = point - origin;
	}

	void Ray::Init(const SVector& origin, const SVector& point) 
	{
		Origin = origin;
		Direction = point - origin;
	}

	inline SVector Ray::GetPointOnRay(F32 distanceFromOrigin)
	{
		return Origin + Direction * distanceFromOrigin;
	}

	void Ray::Init(const SVector& origin, const SVector& direction) 
	{
		Origin = origin;
		Direction = direction;
	}
}