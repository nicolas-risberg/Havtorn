#pragma once

#include "Core/Core.h"

#include <spdlog/spdlog.h>
#include <spdlog/fmt/ostr.h>

namespace Havtorn
{
	class HAVTORN_API GLog
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

#define HV_LOG_TRACE(...)		::Havtorn::GLog::GetLogger()->trace(__VA_ARGS__)
#define HV_LOG_DEBUG(...)		::Havtorn::GLog::GetLogger()->debug(__VA_ARGS__)
#define HV_LOG_INFO(...)		::Havtorn::GLog::GetLogger()->info(__VA_ARGS__)
#define HV_LOG_WARNING(...)		::Havtorn::GLog::GetLogger()->warn(__VA_ARGS__)
#define HV_LOG_ERROR(...)		::Havtorn::GLog::GetLogger()->error(__VA_ARGS__)
#define HV_LOG_CRITICAL(...)	::Havtorn::GLog::GetLogger()->critical(__VA_ARGS__)