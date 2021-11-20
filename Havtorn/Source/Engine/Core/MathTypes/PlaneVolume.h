#pragma once
#include <vector>
#include "Plane.h"

namespace Havtorn 
{
	template <class T>
	class PlaneVolume {
	public:
		// Default constructor: empty PlaneVolume.
		PlaneVolume();
		// Constructor taking a list of Plane that makes up the PlaneVolume.
		PlaneVolume(const std::vector<Plane<T>> &plane_list);
		// Add a Plane to the PlaneVolume.
		void add_plane(const Plane<T> &plane);
		// Returns whether a point is inside the PlaneVolume: it is inside when the point is on the
		// plane or on the side the normal is pointing away from for all the planes in the PlaneVolume.
		bool is_inside(const Vector3<T> &position);
	private:
		std::vector<Plane<T>> _plane_list;
	};

	template <class T>
	PlaneVolume<T>::PlaneVolume() {
		_plane_list = {};
	}

	template <class T>
	PlaneVolume<T>::PlaneVolume(const std::vector<Plane<T>> &plane_list) {
		_plane_list = plane_list;
	}

	template <class T>
	void PlaneVolume<T>::add_plane(const Plane<T> &plane) {
		_plane_list.emplace_back(plane);
	}

	template <class T>
	bool PlaneVolume<T>::is_inside(const Vector3<T> &position) {
		for (auto plane : _plane_list) {
			if (!plane.is_inside(position)) {
				return false;
			}
		}
		return true;
	}

}
