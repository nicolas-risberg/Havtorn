#pragma once

#include <cstdint>

namespace Havtorn
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

#define BIT(x) (1 << x)
#define CACHE_LINE 32
#define CACHE_ALIGN __declspec(align(CACHE_LINE))
}