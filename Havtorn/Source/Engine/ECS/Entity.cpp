// Copyright 2022 Team Havtorn. All Rights Reserved.

#include "hvpch.h"
#include "Entity.h"

namespace Havtorn
{
	SEntity::SEntity(U64 id, std::string name)
		: ID(id)
		, Name(name)
	{
		for (U16 i = 0; i < static_cast<U16>(EComponentType::Count); ++i)
			ComponentIndices[i] = -1;
	}

	void SEntity::AddComponent(EComponentType type, U64 arrayIndex) const
	{
		ComponentIndices[static_cast<size_t>(type)] = arrayIndex;
	}

	void SEntity::RemoveComponent(EComponentType type) const
	{
		ComponentIndices[static_cast<size_t>(type)] = -1;
	}

	bool SEntity::HasComponent(EComponentType type) const
	{
		return ComponentIndices[static_cast<size_t>(type)] != -1;
	}

	const I64 SEntity::GetComponentIndex(EComponentType type) const
	{
		return ComponentIndices[static_cast<size_t>(type)];
	}
}
