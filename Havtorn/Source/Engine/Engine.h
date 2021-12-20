#pragma once
#include "hvpch.h"

#include "Core/Core.h"
#include "Application/WindowHandler.h"
#include "Graphics/GraphicsFramework.h"
#include "Application/LayerStack.h"
//#include "FullscreenRenderer.h"

#define IRONWROUGHT CEngine::GetInstance()
#define IRONWROUGHT_ACTIVE_SCENE CEngine::GetInstance()->GetActiveScene()

namespace Havtorn
{
	class CWindowHandler;
	class CDirextXFramework;
	class CTimer;
	class CImguiManager;
	class CModelFactory;
	class CCameraFactory;
	class CLightFactory;
	class CScene;
	class CRenderManager;
	class CParticleEmitterFactory;
	class CSpriteFactory;
	class CTextFactory;
	class CDecalFactory;
	class CInputMapper;
	class CDebug;
	class CMainSingleton;
	class CForwardRenderer;
	class CVFXMeshFactory;
	class CLineFactory;
	class CAudioManager;
	class CAudioChannel;
	class CPhysXWrapper;
	class CSceneFactory;
	class CGameObject;

	namespace PostMaster {
		struct SAudioSourceInitData;
	}

	class CEngine
	{
		friend class CForwardRenderer;
		friend class CModelFactory;
		friend class CVFXMeshFactory;
		friend class CLightFactory;
		friend class CRenderManager;
		friend class CLineFactory;
		friend class CDecalFactory;
		friend class CMaterialHandler;
		friend class CSceneFactory;

	public:
		CEngine();
		~CEngine();
		bool Init(CWindowHandler::SWindowData& someWindowData);
		float BeginFrame();
		void Update();
		void RenderFrame();
		void EndFrame();
		CWindowHandler* GetWindowHandler();
		void InitWindowsImaging();
		void CrashWithScreenShot(std::wstring& aSubPath);

		void SetResolution(SVector2<F32> aResolution);

		static CEngine* GetInstance();

		//const CStateStack::EState AddScene(const CStateStack::EState aState, CScene* aScene);
		//void SetActiveScene(const CStateStack::EState aState);
		//CScene& GetActiveScene();
		//inline const bool IsActiveScene(const CStateStack::EState& aState);
		//void UpdateScene(const CStateStack::EState& aState);
		//CPhysXWrapper& GetPhysx() { return *myPhysxWrapper; }
		//CAudioChannel* RequestAudioSource(const PostMaster::SAudioSourceInitData& aData);

		const bool IsInGameScene() const;

		//void ModelViewerSetScene(CScene* aScene);
		//void PopBackScene();
		//void SetActiveScene(CScene* aScene);

		//unsigned int ScenesSize();

		void SetRenderScene(const bool aRenderSceneActive) { myRenderSceneActive = aRenderSceneActive; }
		//void RemoveScene(CStateStack::EState aState);
		void ClearModelFactory();

		void ShowCursor(const bool& anIsInEditorMode = true);
		void HideCursor(const bool& anIsInEditorMode = false);

		//void SetBrokenScreen(bool aShouldSetBrokenScreen);
		//const CFullscreenRenderer::SPostProcessingBufferData& GetPostProcessingBufferData() const;
		//void SetPostProcessingBufferData(const CFullscreenRenderer::SPostProcessingBufferData& someBufferData);

		void SetAudioListener(CGameObject* aGameObject);

		void SetIsMenu(bool aMenuIsOpen);

	private:
		//void AllScenesToInactive();

		// More or less a temp fix for SP7. Aki 2021 04 15
		//void CheckIfMenuState(const CStateStack::EState& aState);

	private:
		static CEngine* ourInstance;
		CWindowHandler* myWindowHandler;
		CDirectXFramework* myFramework;
		CForwardRenderer* myForwardRenderer;
		CImguiManager* ImguiManager;
		CRenderManager* myRenderManager;
		CTimer* myTimer;
		CDebug* myDebug;
		CPhysXWrapper* myPhysxWrapper;

		//unsigned int myActiveScene;
		//CStateStack::EState myActiveState;
		//std::vector<CScene*> myScenes;
		//std::unordered_map<CStateStack::EState, CScene*> mySceneMap;

		CModelFactory* myModelFactory;
		CCameraFactory* myCameraFactory;
		CLightFactory* myLightFactory;
		CParticleEmitterFactory* myParticleFactory;
		CVFXMeshFactory* myVFXFactory;
		CLineFactory* myLineFactory;
		CSpriteFactory* mySpriteFactory;
		CTextFactory* myTextFactory;
		CDecalFactory* myDecalFactory;
		CInputMapper* myInputMapper;
		CMainSingleton* myMainSingleton;
		CAudioManager* myAudioManager;
		CSceneFactory* mySceneFactory;

		bool myRenderSceneActive = false;
	};
}