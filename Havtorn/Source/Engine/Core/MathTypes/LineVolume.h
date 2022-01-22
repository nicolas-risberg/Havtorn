// Copyright 2022 Team Havtorn. All Rights Reserved.

#pragma once

#include "Line.h"

namespace Havtorn 
{
	class LineVolume 
	{
	public:
		// Default constructor: empty LineVolume.
		LineVolume();
		// Constructor taking a list of Line that makes up the LineVolume.
		LineVolume(const std::vector<Line>& lineList);
		// Add a Line to the LineVolume.
		void AddLine(const Line& line);
		// Returns whether a point is inside the LineVolume: it is inside when the point is
		// on the line or on the side the normal is pointing away from for all the lines in
		// the LineVolume.
		bool IsInside(const SVector& position);
	private:
		std::vector<Line> LineList;
	};

	LineVolume::LineVolume() 
	{
		LineList = {};
	}

	LineVolume::LineVolume(const std::vector<Line> &lineList) 
	{
		LineList = lineList;
	}

	void LineVolume::AddLine(const Line& line) 
	{
		LineList.emplace_back(line);
	}

	bool LineVolume::IsInside(const SVector& position) 
	{	
		for (auto& line : LineList) 
		{
			if (!line.IsInside(position)) 
			{
				return false;
			}
		}
		return true;
	}

}