#pragma once

#ifdef NE_PLATFORM_WINDOWS
	#ifdef NE_BUILD_DLL
		#define NEW_ENGINE_API __declspec(dllexport)
	#else
		#define NEW_ENGINE_API __declspec(dllimport)
	#endif
#else
	#error NewEngine currently only supports 64 bit Windows
#endif