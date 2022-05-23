// Copyright 2022 Team Havtorn. All Rights Reserved.

#pragma once
#include "GraphicsStructs.h"

namespace Havtorn
{
	static std::vector<SPositionVertex> PointLightCube = 
	{
		// X      Y      Z      W 
		{ -1.0f, -1.0f, -1.0f,  1.0f },
		{  1.0f, -1.0f, -1.0f,  1.0f },
		{ -1.0f,  1.0f, -1.0f,  1.0f },
		{  1.0f,  1.0f, -1.0f,  1.0f },
		{ -1.0f, -1.0f,  1.0f,  1.0f },
		{  1.0f, -1.0f,  1.0f,  1.0f },
		{ -1.0f,  1.0f,  1.0f,  1.0f },
		{  1.0f,  1.0f,  1.0f,  1.0f },
		// X      Y      Z      W 
		{ -1.0f, -1.0f, -1.0f,  1.0f },
		{ -1.0f,  1.0f, -1.0f,  1.0f },
		{ -1.0f, -1.0f,  1.0f,  1.0f },
		{ -1.0f,  1.0f,  1.0f,  1.0f },
		{  1.0f, -1.0f, -1.0f,  1.0f },
		{  1.0f,  1.0f, -1.0f,  1.0f },
		{  1.0f, -1.0f,  1.0f,  1.0f },
		{  1.0f,  1.0f,  1.0f,  1.0f },
		// X      Y      Z      W 
		{ -1.0f, -1.0f, -1.0f,  1.0f },
		{  1.0f, -1.0f, -1.0f,  1.0f },
		{ -1.0f, -1.0f,  1.0f,  1.0f },
		{  1.0f, -1.0f,  1.0f,  1.0f },
		{ -1.0f,  1.0f, -1.0f,  1.0f },
		{  1.0f,  1.0f, -1.0f,  1.0f },
		{ -1.0f,  1.0f,  1.0f,  1.0f },
		{  1.0f,  1.0f,  1.0f,  1.0f }
	};

	static std::vector<U32> PointLightCubeIndices = 
	{
		0,2,1,
		2,3,1,
		4,5,7,
		4,7,6,
		8,10,9,
		10,11,9,
		12,13,15,
		12,15,14,
		16,17,18,
		18,17,19,
		20,23,21,
		20,22,23
	};
}