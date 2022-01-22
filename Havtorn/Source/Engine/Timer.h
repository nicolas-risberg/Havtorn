// Copyright 2022 Team Havtorn. All Rights Reserved.

#pragma once
#include <chrono>
#include "hvpch.h"

namespace Havtorn
{

#define NO_FRAMES_TO_AVERAGE 1500

	class CTimer 
	{
	public:
		static F32 Time();
		static F32 Dt();
		static F32 FixedDt();
		static F32 AverageFrameRate();

		CTimer();
		~CTimer();

		static F32 Mark();

		static bool FixedTimeStep();

	private:
		float NewFrame();
		float TotalTime() const;

	private:
		static CTimer* Instance;
		std::chrono::steady_clock::time_point First;
		std::chrono::steady_clock::time_point Last;
		
		std::array<F32, NO_FRAMES_TO_AVERAGE> FrameTimes;
		
		F32 DeltaTime;
		F32 FixedTime;
		F32 FixedTimeInterval;

		F32 AverageFrameTime;
		U16 CurrentFrameTimeIndex;
		U16 FrameTimesLoaded;
	};
}
