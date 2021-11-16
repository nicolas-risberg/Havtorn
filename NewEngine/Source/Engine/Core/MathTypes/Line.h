#pragma once
#include "Vector.h"

namespace NewEngine 
{
	template <class T>
	class Line {
	public:
		// Default constructor: there is no line, the normal is the zero vector.
		Line();

		// Copy constructor.
		Line(const Line <T> &line);

		// Constructor that takes two points that define the line, the direction is point1 - point0.
		Line(const Vector2<T> &point0, const Vector2<T> &point1);

		// Init the line with two points, the same as the constructor above.
		void init_with_2_points(const Vector2<T> &point0, const Vector2<T> &point1);

		// Init the line with a point and a direction.
		void init_with_point_and_direction(const Vector2<T> &point, const Vector2<T> &direction);

		// Returns whether a point is inside the line: it is inside when the point is on the line or on the side the normal is pointing away from.
		bool is_inside(const Vector2<T> &position) const;

		// Returns the direction of the line.
		const Vector2<T> &get_direction() const;

		// Returns the normal of the line, which is (-direction.y, direction.x).
		const Vector2<T> &get_normal() const;

	private:
		Vector2<T> _point;
		Vector2<T> _direction;
	};

	template <class T>
	Line<T>::Line() {
		_point = Vector2<T>();
		_direction = Vector2<T>();
	}

	template <class T>
	Line<T>::Line(const Line<T> &line) {
		_point = line._point;
		_direction = line._direction.GetNormalized();
	}
	
	template <class T>
	Line<T>::Line(const Vector2<T> &point0, const Vector2<T> &point1) {
		_point = point0;
		_direction = Vector2<T>(point1 - point0).GetNormalized();
	}

	template <class T>
	void Line<T>::init_with_2_points(const Vector2<T> &point0, const Vector2<T> &point1) {
		_point = point0;
		_direction = Vector2<T>(point1 - point0).GetNormalized();
	}

	template <class T>
	void Line<T>::init_with_point_and_direction(const Vector2<T> &point, const Vector2<T> &direction) {
		_point = point;
		_direction = direction.GetNormalized();
	}

	template <class T>
	bool Line<T>::is_inside(const Vector2<T> &position) const {
		return Vector2<T>(position - _point).Dot(Vector2<T>(-_direction.y, _direction.x)) <= 0;
	}

	template <class T>
	const Vector2<T> &Line<T>::get_direction() const {
		return _direction;
	}
	
	template <class T>
	const Vector2<T> &Line<T>::get_normal() const {
		return Vector2<T>(-_direction.y, _direction.x);
	}
}