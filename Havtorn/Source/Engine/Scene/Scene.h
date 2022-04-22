// Copyright 2022 Team Havtorn. All Rights Reserved.

#pragma once

namespace Havtorn
{
#define COMPONENT_VECTOR_DECLARATION(x) std::vector<Ref<S##x>> x##s;
#define COMPONENT_VECTOR_GETTER(x) std::vector<Ref<S##x>>& Get##x##s() { return x##s; }

	struct SEntity;
	struct STransformComponent;
	struct SStaticMeshComponent;
	struct SCameraComponent;
	struct SDirectionalLightComponent;
	struct SPointLightComponent;
	struct SSpotLightComponent;
	class CSystem;
	class CRenderManager;

	class CScene final
	{
	public:
		CScene() = default;
		~CScene() = default;

		bool Init(CRenderManager* renderManager);
		void Update();

		void InitDemoScene(CRenderManager* renderManager);

		std::vector<Ref<STransformComponent>>& GetTransformComponents() { return TransformComponents; }
		std::vector<Ref<SStaticMeshComponent>>& GetStaticMeshComponents() { return StaticMeshComponents; }
		std::vector<Ref<SCameraComponent>>& GetCameraComponents() { return CameraComponents; }
		std::vector<Ref<SDirectionalLightComponent>>& GetDirectionalLightComponents() { return DirectionalLightComponents; }
		COMPONENT_VECTOR_GETTER(PointLightComponent)
		std::vector<Ref<SSpotLightComponent>>& GetSpotLightComponents() { return SpotLightComponents; }

		std::vector<Ref<SEntity>>& GetEntities() { return Entities; }

	private:
		std::vector<Ref<SEntity>> Entities;
		std::vector<Ref<STransformComponent>> TransformComponents;
		std::vector<Ref<SStaticMeshComponent>> StaticMeshComponents;
		std::vector<Ref<SCameraComponent>> CameraComponents;
		std::vector<Ref<SDirectionalLightComponent>> DirectionalLightComponents;
		COMPONENT_VECTOR_DECLARATION(PointLightComponent)
		std::vector<Ref<SSpotLightComponent>> SpotLightComponents;
		std::vector<Ptr<CSystem>> Systems;
	};
}
