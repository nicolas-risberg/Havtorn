#pragma once
#include <vector>
#include "Line.h"

namespace NewEngine 
{
	template <class T>
	class LineVolume {
	public:
		// Default constructor: empty LineVolume.
		LineVolume();
		// Constructor taking a list of Line that makes up the LineVolume.
		LineVolume(const std::vector<Line<T>> &line_list);
		// Add a Line to the LineVolume.
		void add_line(const Line<T> &line);
		// Returns whether a point is inside the LineVolume: it is inside when the point is
		// on the line or on the side the normal is pointing away from for all the lines in
		// the LineVolume.
		bool is_inside(const Vector2<T> &position);
	private:
		std::vector<Line<T>> _line_list;
	};

	template <class T>
	LineVolume<T>::LineVolume() {
		_line_list = {};
	}

	template <class T>
	LineVolume<T>::LineVolume(const std::vector<Line<T>> &line_list) {
		_line_list = line_list;
	}

	template <class T>
	void LineVolume<T>::add_line(const Line<T> &line) {
		_line_list.emplace_back(line);
	}

	template <class T>
	bool LineVolume<T>::is_inside(const Vector2<T> &position) {
		for (auto line : _line_list) {
			if (!line.is_inside(position)) {
				return false;
			}
		}
		return true;
	}

}