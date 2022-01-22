// Copyright 2022 Team Havtorn. All Rights Reserved.

#pragma once

#ifdef HV_PLATFORM_WINDOWS
	#ifdef HV_BUILD_DLL
		#define HAVTORN_API __declspec(dllexport)
	#else
		#define HAVTORN_API __declspec(dllimport)
	#endif

#ifndef HV_DIRECTX_11
	#define HV_DIRECTX_11
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

#define HV_ASSERT_BUFFER(x) static_assert((sizeof(x) % 16) == 0, "CB size not padded correctly");

#define BIT(x) (1 << x)
#define CACHE_LINE 32
#define CACHE_ALIGN __declspec(align(CACHE_LINE))