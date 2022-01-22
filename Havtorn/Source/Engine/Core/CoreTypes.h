// Copyright 2022 Team Havtorn. All Rights Reserved.

#pragma once

#include <cstdint>
#include <wrl.h>

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

	using String = std::string;

	template<typename T>
	using Ptr = std::unique_ptr<T>;
	template<typename T>
	using Ref = std::shared_ptr<T>;

	template<typename T>
	using WinComPtr = Microsoft::WRL::ComPtr<T>;
}