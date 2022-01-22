// Copyright 2022 Team Havtorn. All Rights Reserved.

#pragma once

namespace Havtorn
{
	struct SEntity
	{
		explicit SEntity(U64 id, std::string name);

		const std::string Name = "";
		const U64 ID = 0;

		void AddComponent(EComponentType type, U64 arrayIndex) const;
		void RemoveComponent(EComponentType type) const;
		bool HasComponent(EComponentType type) const;
		const I64 GetComponentIndex(EComponentType type) const;

	private:
		 mutable std::array<I64, static_cast<size_t>(EComponentType::Count)> ComponentIndices = {};
	};
}
