// Copyright 2022 Team Havtorn. All Rights Reserved.

#pragma once

#include <xmmintrin.h>
#include "Core/CoreTypes.h"

namespace Havtorn
{
	using VectorRegister = __m128;

#define SHUFFLE_MASK(A0, A1, B2, B3) ((A0) | ((A1)<<2) | ((B2)<<4) | ((B3)<<6))

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

	/**
	* Replicates one element into all four elements.
	* 
	* @param vectorRegister	Source vector.
	* @param elementIndex Index from 0 to 3 of the element to replicate.
	* @return VectorRegister(registerPointer[0], registerPointer[1], registerPointer[2], registerPointer[3])
	*/
	inline void VectorRegisterReplicate(const VectorRegister& /*vectorRegister*/, U8 /*elementIndex*/)
	{
		//_mm_shuffle_ps(vectorRegister, vectorRegister, SHUFFLE_MASK(elementIndex, elementIndex, elementIndex, elementIndex));
	}

	inline void VectorRegisterAbs(const VectorRegister& /*vectorRegister*/)
	{
		//_mm_and_ps(vectorRegister, VectorRegisterConstants::SignMask);
	}

	inline void VectorRegisterNegate(const VectorRegister& vectorRegister)
	{
		_mm_sub_ps(_mm_setzero_ps(), vectorRegister);
	}

	inline VectorRegister VectorRegisterAdd(const VectorRegister& vec1, const VectorRegister& vec2)
	{
		return _mm_add_ps(vec1, vec2);
	}

	inline VectorRegister VectorRegisterSubtract(const VectorRegister& vec1, const VectorRegister& vec2)
	{
		return _mm_sub_ps(vec1, vec2);
	}

	inline VectorRegister VectorRegisterMultiply(const VectorRegister& vec1, const VectorRegister& vec2)
	{
		return _mm_mul_ps(vec1, vec2);
	}

	inline void VectorRegisterMultiplyAdd(const VectorRegister& vec1, const VectorRegister& vec2, VectorRegister& vec3)
	{
		_mm_add_ps(_mm_mul_ps(vec1, vec2), vec3);
	}
}
