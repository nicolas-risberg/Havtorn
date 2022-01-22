// Copyright 2022 Team Havtorn. All Rights Reserved.

#pragma once

#include "Core/Core.h"
#include <cstdarg>

namespace Havtorn
{
	enum class EConsoleColor
	{
		DarkGreen = 2,
		DarkAqua = 3,
		White = 7,
		Red = 12,
		Yellow = 14,
		WhiteRedBackground = 79
	};

	class HAVTORN_API GLog
	{
	public:
		static void Init();
		static Ref<GLog>& GetLogger() { return Logger; }

		static void Print(const EConsoleColor& color, const char* category, const char* message, ...);

		// TODO.NR: Figure out another place for this. This is the exact same as the one used in EngineException.h
		static std::string StringVsprintf(const char* format, const std::va_list args)
		{
			va_list tmpArgs; //unfortunately you cannot consume a va_list twice
			va_copy(tmpArgs, args); //so we have to copy it
			const int requiredLen = _vscprintf(format, tmpArgs) + 1;
			va_end(tmpArgs);

			char buff[4096];
			memset(buff, 0, requiredLen);
			if (vsnprintf_s(buff, requiredLen, format, args) < 0)
			{
				return "StringVsprintf encoding error";
			}
			return { buff };
		}

	private:
#pragma warning(disable:4251)
		static Ref<GLog> Logger;
#pragma warning(default:4251)
	};
}

#define HV_LOG_TRACE(...)		::Havtorn::GLog::GetLogger()->Print(EConsoleColor::White, "TRACE", __VA_ARGS__)
#define HV_LOG_DEBUG(...)		::Havtorn::GLog::GetLogger()->Print(EConsoleColor::DarkGreen, "DEBUG", __VA_ARGS__)
#define HV_LOG_INFO(...)		::Havtorn::GLog::GetLogger()->Print(EConsoleColor::DarkAqua, "INFO", __VA_ARGS__)
#define HV_LOG_WARNING(...)		::Havtorn::GLog::GetLogger()->Print(EConsoleColor::Yellow, "WARNING", __VA_ARGS__)
#define HV_LOG_ERROR(...)		::Havtorn::GLog::GetLogger()->Print(EConsoleColor::Red, "ERROR", __VA_ARGS__)
#define HV_LOG_CRITICAL(...)	::Havtorn::GLog::GetLogger()->Print(EConsoleColor::WhiteRedBackground, "CRITICAL", __VA_ARGS__)
