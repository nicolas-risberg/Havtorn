#pragma once
#include <xmmintrin.h>
#include "CoreTypes.h"

namespace NewEngine
{
	using VectorRegister = __m128;

	inline VectorRegister MakeVectorRegister(F32 x, F32 y, F32 z, F32 w)
	{
		return _mm_setr_ps(x, y, z, w);
	}

	inline VectorRegister VectorRegisterZero()
	{
		return _mm_setzero_ps();
	}

	/**
	 * Loads 4 floats from aligned memory.
	 * 
	 * @param registerPointer Aligned memory pointer to the 4 floats.
	 * @return VectorRegister(registerPointer[0], registerPointer[1], registerPointer[2], registerPointer[3])
	 */
	inline void VectorRegisterLoadAligned(void* registerPointer)
	{
		_mm_load_ps((const F32*)(registerPointer));
	}

	/**
	 * Stores 4 floats to aligned memory.
	 * 
	 * @param vectorRegister Floats to store.
	 * @return VectorRegister(registerPointer[0], registerPointer[1], registerPointer[2], registerPointer[3])
	 */
	inline void VectorRegisterStoreAligned(const VectorRegister& vectorRegister, void* registerPointer)
	{
		_mm_store_ps((F32*)(registerPointer), vectorRegister);
	}
}
