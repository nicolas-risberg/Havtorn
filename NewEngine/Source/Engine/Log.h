#pragma once

#include <memory.h>
#include <spdlog/spdlog.h>

namespace NewEngine
{
	class GLog
	{
	public:
		static void Init();

		inline static std::shared_ptr<spdlog::logger>& GetLogger() { return Logger; }

	private:
		static std::shared_ptr<spdlog::logger> Logger;
	};
}

#define NE_LOG_TRACE(...)		::NewEngine::GLog::GetLogger()->trace(__VA_ARGS__)
#define NE_LOG_DEBUG(...)		::NewEngine::GLog::GetLogger()->debug(__VA_ARGS__)
#define NE_LOG_INFO(...)		::NewEngine::GLog::GetLogger()->info(__VA_ARGS__)
#define NE_LOG_WARNING(...)		::NewEngine::GLog::GetLogger()->warn(__VA_ARGS__)
#define NE_LOG_ERROR(...)		::NewEngine::GLog::GetLogger()->error(__VA_ARGS__)
#define NE_LOG_CRITICAL(...)	::NewEngine::GLog::GetLogger()->critical(__VA_ARGS__)