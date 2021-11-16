#pragma once
#include "Vector.h"

namespace NewEngine 
{
	template <class T>
	class Sphere {
	public:
		// Default constructor: there is no sphere, the radius is zero and the position is
		// the zero vector.
		Sphere();
		// Copy constructor.
		Sphere(const Sphere<T> &sphere);
		// Constructor that takes the center position and radius of the sphere.
		Sphere(const Vector3<T> &center, T radius);
		// Init the sphere with a center and a radius, the same as the constructor above.
		void InitWithCenterAndRadius(const Vector3<T> &center, T radius);
		// Returns whether a point is inside the sphere: it is inside when the point is on the
		// sphere surface or inside of the sphere.
		bool IsInside(const Vector3<T> &position) const;
		
		Vector3<T> _center;
		T _radius;
	};

	template <class T>
	Sphere<T>::Sphere() : _center(Vector3<T>()), _radius(0) {}

	template <class T>
	Sphere<T>::Sphere(const Sphere<T> &sphere) {
		_center = sphere._center;
		_radius = sphere._radius;
	}

	template <class T>
	Sphere<T>::Sphere(const Vector3<T> &center, T radius) {
		_center = center;
		_radius = radius;
	}

	template <class T>
	void Sphere<T>::InitWithCenterAndRadius(const Vector3<T> &center, T radius) {
		_center = center;
		_radius = radius;
	}

	template <class T>
	bool Sphere<T>::IsInside(const Vector3<T> &position) const {
		T distance_squared = (position._x - _center._x) * (position._x - _center._x) + (position._y - _center._y) * (position._y - _center._y) + (position._z - _center._z) * (position._z - _center._z);
		return distance_squared <= _radius * _radius;
	}
}
