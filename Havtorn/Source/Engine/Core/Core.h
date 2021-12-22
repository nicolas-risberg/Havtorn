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

#ifdef HV_ENABLE_ASSERTS
	#define HV_ASSERT(x, ...) { if(!(x)) { HV_LOG_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
#else
	#define HV_ASSERT(x, ...)
#endif

#define SAFE_DELETE(x) delete x; x = nullptr;