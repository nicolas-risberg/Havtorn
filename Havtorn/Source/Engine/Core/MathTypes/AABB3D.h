#pragma once
#include "Vector.h"

namespace Havtorn 
{
	template <class T>
	class AABB3D {
	public:
		// Default constructor: there is no AABB, both min and max points are the zero vector.
		AABB3D();
		// Copy constructor.
		AABB3D(const AABB3D<T> &AABB3D);
		// Constructor taking the positions of the minimum and maximum corners.
		AABB3D(const Vector3<T> &min, const Vector3<T> &max);
		// Init the AABB with the positions of the minimum and maximum corners, same as
		// the constructor above.
		void InitWithMinAndMax(const Vector3<T> &min, const Vector3<T> &max);
		// Returns whether a point is inside the AABB: it is inside when the point is on any
		// of the AABB's sides or inside of the AABB.
		bool IsInside(const Vector3<T> &position) const;

		Vector3<T> _min;
		Vector3<T> _max;
	};

	template <class T>
	AABB3D<T>::AABB3D() : _min(Vector3<T>()), _max(Vector3<T>()) {}

	template <class T>
	AABB3D<T>::AABB3D(const AABB3D<T> &AABB3D) {
		_min = AABB3D._min;
		_max = AABB3D._max;
	}

	template <class T>
	AABB3D<T>::AABB3D(const Vector3<T> &min, const Vector3<T> &max) {
		_min = min;
		_max = max;
	}

	template <class T>
	void AABB3D<T>::InitWithMinAndMax(const Vector3<T> &min, const Vector3<T> &max) {
		_min = min;
		_max = max;
	}

	template <class T>
	bool AABB3D<T>::IsInside(const Vector3<T> &position) const {
		return (((position._x >= _min._x) && (position._x <= _max._x)) && ((position._y >= _min._y) && (position._y <= _max._y)) && ((position._z >= _min._z) && (position._z <= _max._z)));
	}
}
