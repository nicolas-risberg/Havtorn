// Copyright 2022 Team Havtorn. All Rights Reserved.

#include "TransformComponent.h"

#include "imgui.h"

namespace Havtorn
{
	void STransformComponent::InspectInEditor()
	{
		const SVector vector = Transform.GetMatrix().Translation();
		F32 data[3] = { vector.X, vector.Y, vector.Z };
		ImGui::InputFloat3("Position", data);
		Transform.GetMatrix().Translation({data[0], data[1], data[2]});
	}
}