#pragma once

namespace Havtorn
{
	struct SEntity;
	struct STransformComponent;
	struct SRenderComponent;
	class CSystem;

	class CScene final
	{
	public:
		CScene() = default;
		~CScene();

		bool Init();
		void Update();

		std::vector<Ptr<STransformComponent>>& GetTransformComponents() { return TransformComponents; }
		std::vector<Ptr<SRenderComponent>>& GetRenderComponents() { return RenderComponents; }
	private:
		std::vector<Ptr<SEntity>> Entities;
		std::vector<Ptr<STransformComponent>> TransformComponents;
		std::vector<Ptr<SRenderComponent>> RenderComponents;
		std::vector<Ptr<CSystem>> Systems;
	};
}
