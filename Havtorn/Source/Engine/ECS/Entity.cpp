#include "hvpch.h"
#include "Entity.h"

namespace Havtorn
{
	SEntity::SEntity(U64 id, std::string name)
		: ID(id)
		, Name(name)
	{
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
