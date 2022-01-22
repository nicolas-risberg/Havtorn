// Copyright 2022 Team Havtorn. All Rights Reserved.

#pragma once
#include "hvpch.h"

#include "Core/Core.h"
#include "Application/WindowHandler.h"
#include "Graphics/GraphicsFramework.h"
#include "Application/LayerStack.h"
//#include "FullscreenRenderer.h"

namespace Havtorn
{
	class CWindowHandler;
	class CThreadManager;
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

	namespace PostMaster 
	{
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
		bool Init(CWindowHandler::SWindowData& windowData);
		float BeginFrame();
		void Update();
		void RenderFrame();
		void EndFrame();
		CWindowHandler* GetWindowHandler();
		void InitWindowsImaging();
		void CrashWithScreenShot(std::wstring& subPath);

		void SetResolution(SVector2<F32> resolution);

		static CEngine* GetInstance();

		//const CStateStack::EState AddScene(const CStateStack::EState aState, CScene* aScene);
		//void SetActiveScene(const CStateStack::EState aState);
		CScene& GetActiveScene();
		//inline const bool IsActiveScene(const CStateStack::EState& aState);
		//void UpdateScene(const CStateStack::EState& aState);
		//CPhysXWrapper& GetPhysx() { return *PhysxWrapper; }
		//CAudioChannel* RequestAudioSource(const PostMaster::SAudioSourceInitData& aData);

		const bool IsInGameScene() const;

		//void ModelViewerSetScene(CScene* aScene);
		//void PopBackScene();
		//void SetActiveScene(CScene* aScene);

		//unsigned int ScenesSize();

		void SetRenderScene(const bool renderSceneActive) { RenderSceneActive = renderSceneActive; }
		//void RemoveScene(CStateStack::EState aState);
		void ClearModelFactory();

		void ShowCursor(const bool& isInEditorMode = true);
		void HideCursor(const bool& isInEditorMode = false);

		//void SetBrokenScreen(bool aShouldSetBrokenScreen);
		//const CFullscreenRenderer::SPostProcessingBufferData& GetPostProcessingBufferData() const;
		//void SetPostProcessingBufferData(const CFullscreenRenderer::SPostProcessingBufferData& someBufferData);

		void SetAudioListener(CGameObject* gameObject);

		void SetIsMenu(bool menuIsOpen);

	private:
		static CEngine* Instance;
		CWindowHandler* WindowHandler;
		CThreadManager* ThreadManager;
		CDirectXFramework* Framework;
		//CForwardRenderer* ForwardRenderer;
#ifdef _DEBUG
		CImguiManager* ImguiManager;
#endif
		CRenderManager* RenderManager;
		CTimer* Timer;
		//CDebug* Debug;
		//CPhysXWrapper* PhysxWrapper;

		//unsigned int myActiveScene;
		//CStateStack::EState myActiveState;
		CScene* Scene;
		//std::vector<CScene*> myScenes;
		//std::unordered_map<CStateStack::EState, CScene*> mySceneMap;

		//CModelFactory* ModelFactory;
		//CCameraFactory* CameraFactory;
		//CLightFactory* LightFactory;
		//CParticleEmitterFactory* ParticleFactory;
		//CVFXMeshFactory* VFXFactory;
		//CLineFactory* LineFactory;
		//CSpriteFactory* SpriteFactory;
		//CTextFactory* TextFactory;
		//CDecalFactory* DecalFactory;
		//CInputMapper* InputMapper;
		//CMainSingleton* MainSingleton;
		//CAudioManager* AudioManager;
		//CSceneFactory* SceneFactory;

		bool RenderSceneActive = false;
	};
}