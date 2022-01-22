// Copyright 2022 Team Havtorn. All Rights Reserved.

#pragma once

#include "Vector.h"

namespace Havtorn 
{
	class Line 
	{
	public:
		// Default constructor: there is no line, the normal is the zero vector.
		Line();

		// Copy constructor.
		Line(const Line& line);

		// Constructor that takes two points that define the line, the direction is point1 - point0.
		Line(const SVector& point0, const SVector& point1);

		// Init the line with two points, the same as the constructor above.
		void Init(const SVector& point0, const SVector& point1);

		// Init the line with a point and a direction.
		void Init(const SVector& point, const SVector& direction);

		// Uses a simplified calculcation assuming the second point defining the line lies a distance of 1 from Point, so the denominator
		// is simply 1. Reference: https://mathworld.wolfram.com/Point-LineDistance3-Dimensional.html
		F32 PointToLineDistance(const SVector& otherPoint) const;

		// Returns whether a point is inside the line: it is inside when the point is on the line or on the side the normal is pointing away from.
		// N.B. This assumes points lie in the XY-Plane, i.e. use this for 2D only. 
		bool IsInside(const SVector& position) const;

		// Returns the direction of the line.
		const SVector& GetDirection() const;

		// Returns the normal of the line, which is (-direction.y, direction.x, 0).
		const SVector& GetNormal2D() const;

	private:
		SVector Point;
		SVector Direction;
	};

	Line::Line() 
	{
		Point = SVector();
		Direction = SVector();
	}

	Line::Line(const Line& line) 
	{
		Point = line.Point;
		Direction = line.Direction.GetNormalized();
	}
	
	Line::Line(const SVector& point0, const SVector& point1) 
	{
		Point = point0;
		Direction = SVector(point1 - point0).GetNormalized();
	}

	void Line::Init(const SVector& point0, const SVector& point1) 
	{
		Point = point0;
		Direction = SVector(point1 - point0).GetNormalized();
	}

	void Line::Init(const SVector& point, const SVector& direction) 
	{
		Point = point;
		Direction = direction.GetNormalized();
	}

	inline F32 Line::PointToLineDistance(const SVector& otherPoint) const
	{
		SVector point2 = Point + Direction;
		return SVector(SVector(otherPoint - Point).Cross(SVector(otherPoint - point2))).Length();
	}

	bool Line::IsInside(const SVector& position) const
	{
		// 2D version
		SVector vector2D = SVector(position - Point);
		vector2D.Z = 0.0f;
		return vector2D.Dot(SVector(-Direction.Y, Direction.X, 0.0f)) <= 0;
	}

	const SVector& Line::GetDirection() const 
	{
		return Direction;
	}
	
	const SVector& Line::GetNormal2D() const 
	{
		return SVector(-Direction.Y, Direction.X);
	}
}