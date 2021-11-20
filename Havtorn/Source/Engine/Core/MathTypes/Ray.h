#pragma once
#include "Vector.h"

namespace Havtorn 
{
	template <class T>
	class Ray {
	public:
		// Default constructor: there is no ray, the origin and direction are the
        // zero vector.
		Ray();
		// Copy constructor.
		Ray(const Ray<T> &ray);
		// Constructor that takes two points that define the ray, the direction is
		// aPoint - aOrigin and the origin is aOrigin.
		Ray(const Vector3<T> &origin, const Vector3<T> &point);
		// Init the ray with two points, the same as the constructor above.
		void InitWith2Points(const Vector3<T> &origin, const Vector3<T> &point);
		// Init the ray with an origin and a direction.
		void InitWithOriginAndDirection(const Vector3<T> &origin, const Vector3<T> &direction);

		Vector3<T> _origin;
		Vector3<T> _direction;
	};

	template <class T>
	Ray<T>::Ray() : _origin(Vector3<T>()), _direction(Vector3<T>()) {}

	template <class T>
	Ray<T>::Ray(const Ray<T> &ray) {
		_origin = ray._origin;
		_direction = ray._direction;
	}

	template <class T>
	Ray<T>::Ray(const Vector3<T> &origin, const Vector3<T> &point) {
		_origin = origin;
		_direction = point - origin;
	}

	template <class T>
	void Ray<T>::InitWith2Points(const Vector3<T> &origin, const Vector3<T> &point) {
		_origin = origin;
		_direction = point - origin;
	}

	template <class T>
	void Ray<T>::InitWithOriginAndDirection(const Vector3<T> &origin, const Vector3<T> &direction) {
		_origin = origin;
		_direction = direction;
	}
}