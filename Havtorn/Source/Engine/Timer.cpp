#include "hvpch.h"
#include "Timer.h"

namespace Havtorn
{
	CTimer* CTimer::Instance = nullptr;
	
	CTimer::CTimer()
	{
		Instance = this;
		Last = std::chrono::steady_clock::now();
		First = Last;

		FixedTime = 0.0f;
		FixedTimeInterval = 1.0f / 60.0f;
	}

	CTimer::~CTimer()
	{
		Instance = nullptr;
	}

	//Total duration in seconds since start
	float CTimer::Time()
	{
		return Instance->TotalTime();
	}

	//Delta time in seconds between the last two frames
	float CTimer::Dt()
	{
		return Instance->DeltaTime;
	}

	float CTimer::FixedDt()
	{
		return Instance->FixedTimeInterval;
	}

	float CTimer::Mark()
	{
		return Instance->NewFrame();
	}

	bool CTimer::FixedTimeStep()
	{
		return Instance->FixedTime >= Instance->FixedTimeInterval;
	}

	float CTimer::NewFrame()
	{
		const auto old = Last;
		Last = std::chrono::steady_clock::now();
		const std::chrono::duration<float> dt = Last - old;
		DeltaTime = dt.count();

		if (FixedTime > FixedTimeInterval)
		{
			FixedTime -= FixedTimeInterval;
		}

		FixedTime += DeltaTime;

		return DeltaTime;
	}

	float CTimer::TotalTime() const
	{
		return std::chrono::duration<float>(std::chrono::steady_clock::now() - First).count();
	}
}