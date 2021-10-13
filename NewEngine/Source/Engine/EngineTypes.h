#pragma once
#include <cstdint>
#include "Vector.h"

namespace NewEngine
{
	using U8 = std::uint8_t;
	using U16 = std::uint16_t;
	using U32 = std::uint32_t;
	using U64 = std::uint64_t;

	using I8 = std::int8_t;
	using I16 = std::int16_t;
	using I32 = std::int32_t;
	using I64 = std::int64_t;

	using F32 = float;

	// Used to declare types cache aligned in CACHE_LINE bytes
#define CACHE_LINE  32
#define CACHE_ALIGN __declspec(align(CACHE_LINE))

	using SVector3f = SVector3<float>;
}