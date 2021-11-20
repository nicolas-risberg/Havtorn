#pragma once
#include "Vector.h"

namespace Havtorn 
{
	template <class T>
	class Plane {
	public:
		// Default constructor.
		Plane();
		// Constructor taking three points where the normal is (point1 - point0) x (point2 -point0).
		Plane(const Vector3<T> &point0, const Vector3<T> &point1, const Vector3<T> &point2);
		// Constructor taking a point and a normal.
		Plane(const Vector3<T> &point0, const Vector3<T> &normal);
		// Init the plane with three points, the same as the constructor above.
		void init_with_3_points(const Vector3<T> &point0, const Vector3<T> &point1, const Vector3<T> &point2);
		// Init the plane with a point and a normal, the same as the constructor above.
		void init_with_point_and_normal(const Vector3<T> &point, const Vector3<T> &normal);
		// Returns whether a point is inside the plane: it is inside when the point is on the plane or on the side the normal is pointing away from.
		bool is_inside(const Vector3<T> & position) const;
		// Returns the normal of the plane.
		const Vector3<T> &get_normal() const;
		const Vector3<T> &get_point() const;
	private:
		Vector3<T> _point;
		Vector3<T> _normal;
	};

	template <class T>
	Plane<T>::Plane() : _point(Vector3<T>()), _normal(Vector3<T>()) {}

	template <class T>
	Plane<T>::Plane(const Vector3<T> &point0, const Vector3<T> &point1, const Vector3<T> &point2) : _point(point0) {
		_normal = Vector3<T>(point1 - point0).Cross(Vector3<T>(point2 - point0)).GetNormalized();
	}

	template <class T>
	Plane<T>::Plane(const Vector3<T> &point0, const Vector3<T> &normal) : _point(point0), _normal(normal) {}

	template <class T>
	void Plane<T>::init_with_3_points(const Vector3<T> &point0, const Vector3<T> &point1, const Vector3<T> &point2) {
		_point = point0;
		_normal = Vector3<T>(point1 - point0).Cross(Vector3<T>(point2 - point0)).GetNormalized();
	}

	template <class T>
	void Plane<T>::init_with_point_and_normal(const Vector3<T> &point0, const Vector3<T> &normal) {
		_point = point0;
		_normal = normal;
	}

	template <class T>
	bool Plane<T>::is_inside(const Vector3<T> &position) const {
		return Vector3<T>(position - _point).Dot(_normal) <= 0;
	}

	template <class T>
	const Vector3<T>& Plane<T>::get_normal() const {
		return _normal;
	}
	
	template <class T>
	const Vector3<T>& Plane<T>::get_point() const {
		return _point;
	}
}