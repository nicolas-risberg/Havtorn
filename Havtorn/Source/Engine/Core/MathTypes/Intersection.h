// Copyright 2022 Team Havtorn. All Rights Reserved.

#pragma once

#include "Plane.h"
#include "Sphere.h"
#include "AABB3D.h"
#include "Ray.h"
#include <math.h>

namespace Havtorn 
{
	// If the ray is parallel to the plane, aOutIntersectionPoint remains unchanged. If
	// the ray is in the plane, true is returned, if not, false is returned. If the ray
	// isn't parallel to the plane, the intersection point is stored in
	// aOutIntersectionPoint and true returned.
	template <class T>
	bool IntersectionPlaneLine(const Plane<T> &plane, const Ray<T> &ray, Vector3<T> &out_intersection_point) {
		T d = plane.GetPoint().Dot(plane.GetNormal());
		T dn = ray.Direction.Dot(plane.GetNormal());
		if (dn == 0) {
			return false;
		}

		T t = (d - (ray.Origin.Dot(plane.GetNormal())) / dn);

		if (t > 0) {
			out_intersection_point = ray.Origin + t * ray.Direction;
		}

		return t > 0;
	}

	// If the ray intersects the AABB, true is returned, if not, false is returned.
	// A ray in one of the AABB's sides is counted as intersecting it.
	template <class T>
	bool IntersectionAABBLine(const AABB3D<T> &AABB, const Ray<T> &ray) {
		Plane<T> plane_x_max = Plane<T>(AABB.Max, Vector3<T>(1, 0, 0));
		Plane<T> plane_y_max = Plane<T>(AABB.Max, Vector3<T>(0, 1, 0));
		Plane<T> plane_z_max = Plane<T>(AABB.Max, Vector3<T>(0, 0, 1));
		Plane<T> plane_x_min = Plane<T>(AABB.Min, Vector3<T>(1, 0, 0));
		Plane<T> plane_y_min = Plane<T>(AABB.Min, Vector3<T>(0, 1, 0));
		Plane<T> plane_z_min = Plane<T>(AABB.Min, Vector3<T>(0, 0, 1));

		Vector3<T> intersection_plane_x_max; 
		Vector3<T> intersection_plane_y_max;
		Vector3<T> intersection_plane_z_max;
		Vector3<T> intersection_plane_x_min;
		Vector3<T> intersection_plane_y_min;
		Vector3<T> intersection_plane_z_min;

		if (IntersectionPlaneLine(plane_x_max, ray, intersection_plane_x_max)) {
			if (AABB.IsInside(intersection_plane_x_max)) {
				return true;
			}
		}
		if (IntersectionPlaneLine(plane_y_max, ray, intersection_plane_y_max)) {
			if (AABB.IsInside(intersection_plane_y_max)) {
				return true;
			}
		}
		if (IntersectionPlaneLine(plane_z_max, ray, intersection_plane_z_max)) {
			if (AABB.IsInside(intersection_plane_z_max)) {
				return true;
			}
		}
		if (IntersectionPlaneLine(plane_x_min, ray, intersection_plane_x_min)) {
			if (AABB.IsInside(intersection_plane_x_min)) {
				return true;
			}
		}
		if (IntersectionPlaneLine(plane_y_min, ray, intersection_plane_y_min)) {
			if (AABB.IsInside(intersection_plane_y_min)) {
				return true;
			}
		}
		if (IntersectionPlaneLine(plane_z_min, ray, intersection_plane_z_min)) {
			if (AABB.IsInside(intersection_plane_z_min)) {
				return true;
			}
		}
		return false;
	}
	
	// If the ray intersects the sphere, true is returned, if not, false is returned.
	// A ray intersecting the surface of the sphere is considered as intersecting it.
	template <class T>
	bool IntersectionSphereLine(const Sphere<T> &sphere, const Ray<T> &ray) {
		Vector3<T> e = sphere.Center - ray.Origin;
		T a = e.Dot(ray.Direction);
		T t = a - sqrt((sphere.Radius * sphere.Radius) - e.LengthSqr() + (a * a));
		return t > 0;
		{
		//Vector3<T> ray_to_sphere = sphere.Center - ray.Origin;
		//T projection = ray_to_sphere.Dot(ray.Direction);
		//
		////if (projection < 0) {
		////	return false;
		////}

		//T perpendicular_component_squared = ray_to_sphere.LengthSqr() - (projection * projection);
		////
		////if (perpendicular_component_squared > sphere.Radius *sphere.Radius) {
		////	return false;
		////}

		////if (perpendicular_component_squared < 0) {
		////	return false;
		////}
		////
		//T intersection_point_squared = sphere.Radius * sphere.Radius - perpendicular_component_squared;
		//T t0 = sqrt(projection) - sqrt(intersection_point_squared);
		//T t1 = sqrt(projection) + sqrt(intersection_point_squared);
		//
		//if (t0 > t1) { std::swap(t0, t1); }

		//return t0 > 0;
		}
	}
}