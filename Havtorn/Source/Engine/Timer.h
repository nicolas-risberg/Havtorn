#pragma once
#include <chrono>
#include "hvpch.h"

namespace Havtorn
{
	class CTimer 
	{
	public:
		static F32 Time();
		static F32 Dt();
		static F32 FixedDt();

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
		F32 DeltaTime;
		F32 FixedTime;
		F32 FixedTimeInterval;
	};
}
