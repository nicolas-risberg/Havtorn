// Copyright 2022 Team Havtorn. All Rights Reserved.

#pragma once
#include <thread>
#include <mutex>
#include <queue>

namespace Havtorn
{
	enum class ERenderThreadStatus
	{
		ReadyToRender,
		PostRender,
		Count
	};

	class CRenderManager;

	class CThreadManager
	{
	public:
		CThreadManager();
		~CThreadManager();
		bool Init(CRenderManager* renderManager);
		void WaitAndPerformJobs();
		void PushJob(std::function<void()> job);
		void Shutdown();

		static std::mutex RenderMutex;
		static std::condition_variable RenderCondition;
		static ERenderThreadStatus RenderThreadStatus;

	private:
		std::vector<std::thread> JobThreads;
		std::queue<std::function<void()>> JobQueue;
		std::thread RenderThread;
		std::mutex QueueMutex;
		std::mutex ThreadPoolMutex;
		std::condition_variable Condition;
		std::function<void()> Job;

		U8 NumberOfThreads;
		bool Terminate;
		bool IsTerminated;
	};
}
