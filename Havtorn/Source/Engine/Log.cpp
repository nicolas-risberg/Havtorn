// Copyright 2022 Team Havtorn. All Rights Reserved.

#include "hvpch.h"

#include "Log.h"

namespace Havtorn
{
	Ref<GLog> GLog::Logger;

	void GLog::Init()
	{
		Logger = std::make_shared<GLog>();

		HV_LOG_TRACE("Logger initialized.");
	}

	void GLog::Print(const EConsoleColor& color, const char* category, const char* message, ...)
	{
		const HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
		const bool isDefault = color == EConsoleColor::White;

		// Set Console Color
		if (!isDefault)
			SetConsoleTextAttribute(hConsole, static_cast<WORD>(color));

		// VA Args, combined message
		va_list argptr;
		va_start(argptr, message);
		const std::string combinedMessage{ StringVsprintf(message, argptr) };

		// Timestamp
		tm newtime{};
		const time_t now = time(nullptr);
		localtime_s(&newtime, &now);
		char timeStamp[16];
		strftime(timeStamp, sizeof timeStamp, "%H:%M:%S", &newtime);

		// Printout
		std::cout << "[" << timeStamp << "]" << " " << category << ": " << combinedMessage << std::endl;

		// Reset Console Color
		if (!isDefault)
			SetConsoleTextAttribute(hConsole, static_cast<WORD>(EConsoleColor::White));
	}
}
