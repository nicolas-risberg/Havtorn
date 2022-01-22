// Copyright 2022 Team Havtorn. All Rights Reserved.

#include "hvpch.h"
#include <wincodec.h>
#include <document.h>

//#include <ScreenGrab.h>
//#include <DialogueSystem.h>
//#include <PopupTextService.h>

#include "Engine.h"
#include "Application/WindowHandler.h"
#include "Threading/ThreadManager.h"
#include "Graphics/GraphicsFramework.h"
#ifdef _DEBUG
#include "ImGui/ImguiManager.h"
#endif
//#include "WindowHandler.h"
//#include "DirectXFramework.h"
//#include "ForwardRenderer.h"
#include "Scene/Scene.h"
#include "ECS/ECSInclude.h"
//#include "Camera.h"
//#include "EnvironmentLight.h"
//#include "LightFactory.h"
//#include "CameraComponent.h"

//#include "ModelFactory.h"
//#include "CameraFactory.h"
//#include "ParticleEmitterFactory.h"
//#include "TextFactory.h"
//#include "VFXMeshFactory.h"
//#include "LineFactory.h"
//#include "SpriteFactory.h"
//#include "DecalFactory.h"
//
#include "Graphics/RenderManager.h"
//
//#include "ImguiManager.h"
//
//#include "AudioManager.h"
//#include "InputMapper.h"
//
//#include "Debug.h"
//#include "DL_Debug.h"
//
#include "Timer.h"
//#include "MainSingleton.h"
//#include "MaterialHandler.h"
//#include "StateStack.h"
//#include "PhysXWrapper.h"
//#include "SceneManager.h"
//
//
//#include "GameObject.h"

namespace Havtorn
{
	CEngine* CEngine::Instance = nullptr;

	CEngine::CEngine() : RenderSceneActive(true)
	{
		Instance = this;

		Timer = new CTimer();
		WindowHandler = new CWindowHandler();
		ThreadManager = new CThreadManager();
		Framework = new CDirectXFramework();
		RenderManager = new CRenderManager();
#ifdef _DEBUG
		ImguiManager = new CImguiManager();
#endif
		//ForwardRenderer = new CForwardRenderer();
		//ModelFactory = new CModelFactory();
		//CameraFactory = new CCameraFactory();
		//LightFactory = new CLightFactory();
		//ParticleFactory = new CParticleEmitterFactory();
		//VFXFactory = new CVFXMeshFactory();
		//LineFactory = new CLineFactory();
		//SpriteFactory = new CSpriteFactory();
		//TextFactory = new CTextFactory();
		//DecalFactory = new CDecalFactory();
		//InputMapper = new CInputMapper();
		//Debug = new CDebug();
		Scene = new CScene();
		//MainSingleton = new CMainSingleton();
		//// Audio Manager must be constructed after main singleton, since it subscribes to postmaster messages
		//AudioManager = new CAudioManager();
		////myActiveScene = 0; //muc bad
		//myActiveState = CStateStack::EState::BootUp;
		//PhysxWrapper = new CPhysXWrapper();
		//SceneFactory = new CSceneFactory();
		//myDialogueSystem = new CDialogueSystem();
	}

	CEngine::~CEngine()
	{
		//auto it = mySceneMap.begin();
		//while (it != mySceneMap.end())
		//{
		//	delete it->second;
		//	it->second = nullptr;
		//	++it;
		//}

		//delete ModelFactory;
		//ModelFactory = nullptr;
		//delete CameraFactory;
		//CameraFactory = nullptr;
		//delete LightFactory;
		//LightFactory = nullptr;

		//delete ParticleFactory;
		//ParticleFactory = nullptr;
		//delete VFXFactory;
		//VFXFactory = nullptr;
		//delete LineFactory;
		//LineFactory = nullptr;
		//delete SpriteFactory;
		//SpriteFactory = nullptr;
		//delete TextFactory;
		//TextFactory = nullptr;
		//delete DecalFactory;
		//DecalFactory = nullptr;
		//delete InputMapper;
		//InputMapper = nullptr;

		//delete Debug;
		//Debug = nullptr;

		//// Audio Manager must be destroyed before main singleton, since it unsubscribes from postmaster messages
		//delete AudioManager;
		//AudioManager = nullptr;

		////delete myDialogueSystem;
		////myDialogueSystem = nullptr;

		//delete MainSingleton;
		//MainSingleton = nullptr;

		//delete PhysxWrapper;
		//PhysxWrapper = nullptr;

		//delete SceneFactory;
		//SceneFactory = nullptr;

		SAFE_DELETE(Scene);

#ifdef _DEBUG
		SAFE_DELETE(ImguiManager);
#endif
		SAFE_DELETE(ThreadManager);
		SAFE_DELETE(RenderManager);
		SAFE_DELETE(Framework);
		SAFE_DELETE(WindowHandler);
		SAFE_DELETE(Timer);

		Instance = nullptr;
	}

	bool CEngine::Init(CWindowHandler::SWindowData& windowData)
	{
		ENGINE_ERROR_BOOL_MESSAGE(WindowHandler->Init(windowData), "Window Handler could not be initialized.");
		WindowHandler->SetInternalResolution();
		ENGINE_ERROR_BOOL_MESSAGE(Framework->Init(WindowHandler), "Framework could not be initialized.");
		ENGINE_ERROR_BOOL_MESSAGE(RenderManager->Init(Framework, WindowHandler), "RenderManager could not be initialized.");
		ENGINE_ERROR_BOOL_MESSAGE(ThreadManager->Init(RenderManager), "Thread Manager could not be initialized.");
		ENGINE_ERROR_BOOL_MESSAGE(Scene->Init(RenderManager), "Scene could not be initialized.");

#ifdef _DEBUG
		ENGINE_ERROR_BOOL_MESSAGE(ImguiManager->Init(Framework, WindowHandler, RenderManager, Scene), "ImguiManager could not be initialized.");
#endif
		//ENGINE_ERROR_BOOL_MESSAGE(ModelFactory->Init(Framework), "Model Factory could not be initiliazed.");
		//ENGINE_ERROR_BOOL_MESSAGE(CameraFactory->Init(WindowHandler), "Camera Factory could not be initialized.");
		//ENGINE_ERROR_BOOL_MESSAGE(CMainSingleton::MaterialHandler().Init(Framework), "Material Handler could not be initialized.");
		//ENGINE_ERROR_BOOL_MESSAGE(LightFactory->Init(*this), "Light Factory could not be initialized.");
		//ENGINE_ERROR_BOOL_MESSAGE(ParticleFactory->Init(Framework), "Particle Factory could not be initialized.");
		//ENGINE_ERROR_BOOL_MESSAGE(VFXFactory->Init(Framework), "VFX Factory could not be initialized.");
		//ENGINE_ERROR_BOOL_MESSAGE(LineFactory->Init(Framework), "Line Factory could not be initialized.");
		//ENGINE_ERROR_BOOL_MESSAGE(SpriteFactory->Init(Framework), "Sprite Factory could not be initialized.");
		//ENGINE_ERROR_BOOL_MESSAGE(TextFactory->Init(Framework), "Text Factory could not be initialized.");
		//ENGINE_ERROR_BOOL_MESSAGE(DecalFactory->Init(Framework), "Decal Factory could not be initialized.");
		//ENGINE_ERROR_BOOL_MESSAGE(InputMapper->Init(), "InputMapper could not be initialized.");

		//ENGINE_ERROR_BOOL_MESSAGE(CMainSingleton::PopupTextService().Init(), "Popup Text Service could not be initialized.");
		//ENGINE_ERROR_BOOL_MESSAGE(CMainSingleton::DialogueSystem().Init(), "Dialogue System could not be initialized.");
		//ENGINE_ERROR_BOOL_MESSAGE(PhysxWrapper->Init(), "PhysX could not be initialized.");
		InitWindowsImaging();

		return true;
	}

	float CEngine::BeginFrame()
	{
#ifdef _DEBUG

		// Very Expensive, maybe not do this
		//std::string fpsString = "Havtorn Editor | FPS: ";
		//fpsString.append(std::to_string(static_cast<I16>(CTimer::AverageFrameRate())));
		//WindowHandler->SetWindowTitle(fpsString);

		ImguiManager->BeginFrame();
#endif

		return CTimer::Mark();
	}

	void CEngine::Update()
	{
		//if (mySceneMap.find(myActiveState) != mySceneMap.end())
		//{
		//	if (CTimer::FixedTimeStep() == true)
		//	{
		//		PhysxWrapper->Simulate(); //<-- Anropas i samma intervall som Fixed "är"
		//		mySceneMap[myActiveState]->FixedUpdate();
		//	}
		//	mySceneMap[myActiveState]->Update();
		//}

		Scene->Update();
		//AudioManager->Update();
		//CMainSingleton::DialogueSystem().Update();
		//Debug->Update();
		//CSceneFactory::Get()->Update(); //Used for loading Scenes on a seperate Thread!
	}

	void CEngine::RenderFrame()
	{
		if (!RenderSceneActive)
			return;

		//ThreadManager->PushJob(std::bind(&CRenderManager::Render, RenderManager));
		RenderManager->Render();
#ifdef _DEBUG
		ImguiManager->Update();
#endif
	}

	void CEngine::EndFrame()
	{
#ifdef _DEBUG
		ImguiManager->EndFrame();
#endif
		Framework->EndFrame();
	}

	CWindowHandler* CEngine::GetWindowHandler()
	{
		return WindowHandler;
	}

	void CEngine::InitWindowsImaging()
	{
#if (_WIN32_WINNT >= 0x0A00 /*_WIN32_WINNT_WIN10*/)
		Microsoft::WRL::Wrappers::RoInitializeWrapper initialize(RO_INIT_MULTITHREADED);
		if (FAILED(initialize))
			// error
#else
		HRESULT hr = CoInitializeEx(nullptr, COINIT_MULTITHREADED);
		if (FAILED(hr))
			// error
#endif
			return;
	}

#include <DbgHelp.h>
#include <strsafe.h>

	void CEngine::CrashWithScreenShot(std::wstring& /*subPath*/)
	{
		//DL_Debug::CDebug::GetInstance()->CopyToCrashFolder(aSubPath);

		//aSubPath += L"\\screenshot.bmp";
		//HRESULT hr = CoInitialize(nullptr);
		//hr = SaveWICTextureToFile(Framework->GetContext(), Framework->GetBackbufferTexture(),
		//	GUID_ContainerFormatBmp, aSubPath.c_str(),
		//	&GUID_WICPixelFormat16bppBGR565);

		//if (FAILED(hr)) {
		//	return;
		//}
		//CoUninitialize();
	}

	void CEngine::SetResolution(SVector2<F32> resolution)
	{
		WindowHandler->SetResolution(resolution);
		RenderManager->Release();
		RenderManager->ReInit(Framework, WindowHandler);
		//mySceneMap[CStateStack::EState::InGame]->ReInitCanvas(ASSETPATH("Assets/Graphics/UI/JSON/UI_MainMenu.json"));
		//mySceneMap[CStateStack::EState::PauseMenu]->ReInitCanvas(ASSETPATH("Assets/Graphics/UI/JSON/UI_PauseMenu.json"));
	}

	CEngine* CEngine::GetInstance()
	{
		return Instance;
	}

	//const CStateStack::EState CEngine::AddScene(const CStateStack::EState aState, CScene* aScene)
	//{

	//	auto it = mySceneMap.find(aState);
	//	if (it != mySceneMap.end())
	//	{
	//		delete it->second;
	//		it->second = nullptr;
	//		mySceneMap.erase(it);
	//	}
	//	mySceneMap[aState] = aScene;

	//	return aState;
	//}

	//void CEngine::SetActiveScene(const CStateStack::EState aState)
	//{
	//	//ENGINE_BOOL_POPUP(mySceneMap[aState], "The Scene you tried to Get was nullptr!");
	//	myActiveState = aState;
	//	if (mySceneMap.find(myActiveState) == mySceneMap.end())
	//	{
	//		AddScene(myActiveState, CSceneManager::CreateEmpty());
	//	}

	//	CTimer::Mark();
	//	//mySceneMap[myActiveState]->Awake();// Unused
	//	mySceneMap[myActiveState]->Start();
	//}

	//CScene& CEngine::GetActiveScene()
	//{
	//	//ENGINE_BOOL_POPUP(mySceneMap[myActiveState], "The Scene you tried to Get was nullptr!");
	//	return *mySceneMap[myActiveState];
	//}

	//const bool CEngine::IsActiveScene(const CStateStack::EState& aState)
	//{
	//	return myActiveState == aState;
	//}

	//void CEngine::UpdateScene(const CStateStack::EState& aState)
	//{
	//	// Added by Aki as a test :P - Works, may be undesirable. // 2021 04 14

	//	assert(mySceneMap.find(aState) != mySceneMap.end() && "No CScene exists!");
	//	assert(mySceneMap[aState] != nullptr && "No CScene exists!");

	//	if (mySceneMap.find(aState) == mySceneMap.end())
	//		return;
	//	if (!mySceneMap[aState])
	//		return;

	//	CScene& scene = *mySceneMap[aState];
	//	scene.Update();
	//}

	//CAudioChannel* CEngine::RequestAudioSource(const PostMaster::SAudioSourceInitData& aData)
	//{
	//	return AudioManager->AddSource(aData);
	//}

	//const bool CEngine::IsInGameScene() const
	//{
	//	return myActiveState == CStateStack::EState::InGame;
	//}

	//void CEngine::ModelViewerSetScene(CScene* aScene)
	//{
	//	myActiveState = CStateStack::EState::InGame;
	//	mySceneMap[myActiveState] = aScene;
	//}

	//void CEngine::RemoveScene(CStateStack::EState aState)
	//{
	//	if (mySceneMap.find(aState) == mySceneMap.end())
	//		return;

	//	CMainSingleton::PostMaster().Unsubscribe(EMessageType::ComponentAdded, mySceneMap.at(aState));
	//	delete mySceneMap.at(aState);
	//	mySceneMap.at(aState) = nullptr;
	//	mySceneMap.erase(aState);
	//}

	void CEngine::ClearModelFactory()
	{
		//ModelFactory->ClearFactory();
	}

	void CEngine::ShowCursor(const bool& isInEditorMode)
	{
		WindowHandler->ShowAndUnlockCursor(isInEditorMode);
	}
	void CEngine::HideCursor(const bool& isInEditorMode)
	{
		WindowHandler->HideAndLockCursor(isInEditorMode);
	}

	//void CEngine::CheckIfMenuState(const CStateStack::EState& aState)
	//{
	//	bool isInMenu = true;
	//	switch (aState)
	//	{
	//	case CStateStack::EState::BootUp:
	//		isInMenu = false;
	//		break;

	//	case CStateStack::EState::InGame:
	//		isInMenu = false;
	//		break;

	//	case CStateStack::EState::MainMenu:
	//		isInMenu = true;
	//		break;

	//	case CStateStack::EState::PauseMenu:
	//		isInMenu = true;
	//		break;

	//	default:break;
	//	}
	//	WindowHandler->GameIsInMenu(isInMenu);
	//}

	//void CEngine::SetBrokenScreen(bool aShouldSetBrokenScreen)
	//{
	//	RenderManager->SetBrokenScreen(aShouldSetBrokenScreen);
	//}

	//const CFullscreenRenderer::SPostProcessingBufferData& CEngine::GetPostProcessingBufferData() const
	//{
	//	return RenderManager->GetPostProcessingBufferData();
	//}

	//void CEngine::SetPostProcessingBufferData(const CFullscreenRenderer::SPostProcessingBufferData& someBufferData)
	//{
	//	RenderManager->SetPostProcessingBufferData(someBufferData);
	//}

	//void CEngine::SetAudioListener(CGameObject* aGameObject)
	//{
	//	AudioManager->SetListener(aGameObject);
	//}

	//void CEngine::SetIsMenu(bool aMenuIsOpen)
	//{
	//	WindowHandler->GameIsInMenu(aMenuIsOpen);
	//}

}