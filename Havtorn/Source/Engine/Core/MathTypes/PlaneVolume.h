// Copyright 2022 Team Havtorn. All Rights Reserved.

#pragma once

#include "Plane.h"

namespace Havtorn 
{
	class PlaneVolume 
	{
	public:
		// Default constructor: empty PlaneVolume.
		PlaneVolume();
		// Constructor taking a list of Plane that makes up the PlaneVolume.
		PlaneVolume(const std::vector<Plane> &planeList);
		// Add a Plane to the PlaneVolume.
		void AddPlane(const Plane& plane);
		// Returns whether a point is inside the PlaneVolume: it is inside when the point is on the
		// plane or on the side the normal is pointing away from for all the planes in the PlaneVolume.
		bool IsInside(const SVector& position);

	private:
		std::vector<Plane> PlaneList;
	};

	PlaneVolume::PlaneVolume() 
	{
		PlaneList = {};
	}

	PlaneVolume::PlaneVolume(const std::vector<Plane> &planeList) 
	{
		PlaneList = planeList;
	}

	void PlaneVolume::AddPlane(const Plane& plane) 
	{
		PlaneList.emplace_back(plane);
	}

	bool PlaneVolume::IsInside(const SVector& position) 
	{
		for (auto& plane : PlaneList) 
		{
			if (!plane.IsInside(position)) 
			{
				return false;
			}
		}
		return true;
	}

}
