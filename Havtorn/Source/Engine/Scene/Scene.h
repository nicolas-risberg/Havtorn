// Copyright 2022 Team Havtorn. All Rights Reserved.

#pragma once

namespace Havtorn
{
	struct SEntity;
	struct STransformComponent;
	struct SRenderComponent;
	struct SCameraComponent;
	class CSystem;
	class CRenderManager;

	class CScene final
	{
	public:
		CScene() = default;
		~CScene();

		bool Init(CRenderManager* renderManager);
		void Update();

		void InitDemoScene();

		std::vector<Ref<STransformComponent>>& GetTransformComponents() { return TransformComponents; }
		std::vector<Ref<SRenderComponent>>& GetRenderComponents() { return RenderComponents; }
		std::vector<Ref<SCameraComponent>>& GetCameraComponents() { return CameraComponents; }

		std::vector<Ref<SEntity>>& GetEntities() { return Entities; }

	private:
		std::vector<Ref<SEntity>> Entities;
		std::vector<Ref<STransformComponent>> TransformComponents;
		std::vector<Ref<SRenderComponent>> RenderComponents;
		std::vector<Ref<SCameraComponent>> CameraComponents;
		std::vector<Ptr<CSystem>> Systems;
	};
}
