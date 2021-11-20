#include "Log.h"
#include <spdlog/sinks/stdout_color_sinks.h>

namespace Havtorn
{
	std::shared_ptr<spdlog::logger> GLog::Logger;

	void GLog::Init()
	{
		spdlog::set_level(spdlog::level::level_enum::trace);
		spdlog::set_pattern("%^[%T] %n: %v%$");
		Logger = spdlog::stdout_color_mt("CORE");
	}
}