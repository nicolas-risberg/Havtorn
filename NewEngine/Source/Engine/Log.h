#pragma once

#include "Core.h"
#include <memory.h>
#include <spdlog/spdlog.h>

namespace NewEngine
{
	class NEW_ENGINE_API GLog
	{
	public:
		static void Init();

		static std::shared_ptr<spdlog::logger>& GetLogger() { return Logger; }

	private:
#pragma warning(disable:4251)
		static std::shared_ptr<spdlog::logger> Logger;
#pragma warning(default:4251)
	};
}

#define NE_LOG_TRACE(...)		::NewEngine::GLog::GetLogger()->trace(__VA_ARGS__)
#define NE_LOG_DEBUG(...)		::NewEngine::GLog::GetLogger()->debug(__VA_ARGS__)
#define NE_LOG_INFO(...)		::NewEngine::GLog::GetLogger()->info(__VA_ARGS__)
#define NE_LOG_WARNING(...)		::NewEngine::GLog::GetLogger()->warn(__VA_ARGS__)
#define NE_LOG_ERROR(...)		::NewEngine::GLog::GetLogger()->error(__VA_ARGS__)
#define NE_LOG_CRITICAL(...)	::NewEngine::GLog::GetLogger()->critical(__VA_ARGS__)