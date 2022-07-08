// Copyright 2022 Team Havtorn. All Rights Reserved.

#pragma once

namespace Havtorn
{
#define COMPONENT_ADDER_DECLARATION(x) Ref<S##x> Add##x##ToEntity(Ref<SEntity> entity);
#define COMPONENT_ADDER_DEFINITION(x) Ref<S##x> CScene::Add##x##ToEntity(Ref<SEntity> entity){ entity->AddComponent(EComponentType::##x, x##s.size()); return x##s.emplace_back(std::make_shared<S##x>(entity, EComponentType::##x));}

#define COMPONENT_VECTOR_DECLARATION(x) std::vector<Ref<S##x>> x##s;
#define COMPONENT_VECTOR_GETTER(x) std::vector<Ref<S##x>>& Get##x##s() { return x##s; }

	struct SEntity;
	struct STransformComponent;
	struct SStaticMeshComponent;
	struct SCameraComponent;
	struct SMaterialComponent;
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
		COMPONENT_VECTOR_GETTER(MaterialComponent)
		std::vector<Ref<SDirectionalLightComponent>>& GetDirectionalLightComponents() { return DirectionalLightComponents; }
		COMPONENT_VECTOR_GETTER(PointLightComponent)
		std::vector<Ref<SSpotLightComponent>>& GetSpotLightComponents() { return SpotLightComponents; }

		std::vector<Ref<SEntity>>& GetEntities() { return Entities; }

		Ref<SEntity> CreateEntity(const std::string& name);
		COMPONENT_ADDER_DECLARATION(TransformComponent)
		COMPONENT_ADDER_DECLARATION(StaticMeshComponent)
		COMPONENT_ADDER_DECLARATION(CameraComponent)
		COMPONENT_ADDER_DECLARATION(MaterialComponent)
		COMPONENT_ADDER_DECLARATION(DirectionalLightComponent)
		COMPONENT_ADDER_DECLARATION(PointLightComponent)
		COMPONENT_ADDER_DECLARATION(SpotLightComponent)

	private:
		std::vector<Ref<SEntity>> Entities;
		std::vector<Ref<STransformComponent>> TransformComponents;
		std::vector<Ref<SStaticMeshComponent>> StaticMeshComponents;
		std::vector<Ref<SCameraComponent>> CameraComponents;
		COMPONENT_VECTOR_DECLARATION(MaterialComponent)
		std::vector<Ref<SDirectionalLightComponent>> DirectionalLightComponents;
		COMPONENT_VECTOR_DECLARATION(PointLightComponent)
		std::vector<Ref<SSpotLightComponent>> SpotLightComponents;
		std::vector<Ptr<CSystem>> Systems;
	};
}
