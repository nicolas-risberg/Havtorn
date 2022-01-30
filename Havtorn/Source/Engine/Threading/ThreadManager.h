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
		CThreadManager(const CThreadManager&) = delete;
		CThreadManager(const CThreadManager&&) = delete;
		CThreadManager operator=(const CThreadManager&) = delete;
		CThreadManager operator=(const CThreadManager&&) = delete;

		bool Init(CRenderManager* renderManager);
		[[noreturn]] void WaitAndPerformJobs();
		void PushJob(const std::function<void()>& job);
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
