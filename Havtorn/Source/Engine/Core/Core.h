#pragma once

#ifdef HV_PLATFORM_WINDOWS
	#ifdef HV_BUILD_DLL
		#define HAVTORN_API __declspec(dllexport)
	#else
		#define HAVTORN_API __declspec(dllimport)
	#endif
#else
	#error Havtorn currently only supports 64 bit Windows
#endif