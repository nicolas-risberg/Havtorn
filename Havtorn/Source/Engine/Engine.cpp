#include "hvpch.h"
#include <wincodec.h>
#include <document.h>

#ifdef _DEBUG
#include <imgui.h>
#include <imgui_impl_dx11.h>
#include <imgui_impl_win32.h>
#endif

//#include <ScreenGrab.h>
//#include <DialogueSystem.h>
//#include <PopupTextService.h>

#include "Engine.h"
#include "Application/WindowHandler.h"
#include "Graphics/GraphicsFramework.h"
#include "ImGui/ImguiManager.h"
//#include "WindowHandler.h"
//#include "DirectXFramework.h"
//#include "ForwardRenderer.h"
//#include "Scene.h"
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
//#include "RenderManager.h"
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
	CEngine* CEngine::ourInstance = nullptr;

	CEngine::CEngine() : myRenderSceneActive(true)
	{
		ourInstance = this;

		myTimer = new CTimer();
		myWindowHandler = new CWindowHandler();
		myFramework = new CDirectXFramework();
		ImguiManager = new CImguiManager();
		//myForwardRenderer = new CForwardRenderer();
		//myModelFactory = new CModelFactory();
		//myCameraFactory = new CCameraFactory();
		//myLightFactory = new CLightFactory();
		//myParticleFactory = new CParticleEmitterFactory();
		//myVFXFactory = new CVFXMeshFactory();
		//myLineFactory = new CLineFactory();
		//mySpriteFactory = new CSpriteFactory();
		//myTextFactory = new CTextFactory();
		//myDecalFactory = new CDecalFactory();
		//myInputMapper = new CInputMapper();
		//myDebug = new CDebug();
		//myRenderManager = nullptr;
		//myMainSingleton = new CMainSingleton();
		//// Audio Manager must be constructed after main singleton, since it subscribes to postmaster messages
		//myAudioManager = new CAudioManager();
		////myActiveScene = 0; //muc bad
		//myActiveState = CStateStack::EState::BootUp;
		//myPhysxWrapper = new CPhysXWrapper();
		//mySceneFactory = new CSceneFactory();
		//myDialogueSystem = new CDialogueSystem();
	}

	CEngine::~CEngine()
	{
#ifdef _DEBUG
		ImGui_ImplDX11_Shutdown();
#endif



		//auto it = mySceneMap.begin();
		//while (it != mySceneMap.end())
		//{
		//	delete it->second;
		//	it->second = nullptr;
		//	++it;
		//}

		//delete myModelFactory;
		//myModelFactory = nullptr;
		//delete myCameraFactory;
		//myCameraFactory = nullptr;
		//delete myLightFactory;
		//myLightFactory = nullptr;
		//delete myRenderManager;
		//myRenderManager = nullptr;

		//delete myParticleFactory;
		//myParticleFactory = nullptr;
		//delete myVFXFactory;
		//myVFXFactory = nullptr;
		//delete myLineFactory;
		//myLineFactory = nullptr;
		//delete mySpriteFactory;
		//mySpriteFactory = nullptr;
		//delete myTextFactory;
		//myTextFactory = nullptr;
		//delete myDecalFactory;
		//myDecalFactory = nullptr;
		//delete myInputMapper;
		//myInputMapper = nullptr;

		//delete myDebug;
		//myDebug = nullptr;

		//// Audio Manager must be destroyed before main singleton, since it unsubscribes from postmaster messages
		//delete myAudioManager;
		//myAudioManager = nullptr;

		////delete myDialogueSystem;
		////myDialogueSystem = nullptr;

		//delete myMainSingleton;
		//myMainSingleton = nullptr;

		//delete myPhysxWrapper;
		//myPhysxWrapper = nullptr;

		//delete mySceneFactory;
		//mySceneFactory = nullptr;

		delete ImguiManager;
		ImguiManager = nullptr;

		delete myWindowHandler;
		myWindowHandler = nullptr;
		delete myFramework;
		myFramework = nullptr;
		delete myTimer;
		myTimer = nullptr;

		ourInstance = nullptr;
	}

	bool CEngine::Init(CWindowHandler::SWindowData& someWindowData)
	{
		ENGINE_ERROR_BOOL_MESSAGE(myWindowHandler->Init(someWindowData), "Window Handler could not be initialized.");
		ENGINE_ERROR_BOOL_MESSAGE(myFramework->Init(myWindowHandler), "Framework could not be initialized.");
		ImguiManager->Init(myFramework->GetDevice(), myFramework->GetContext(), myWindowHandler->GetWindowHandle());
		myWindowHandler->SetInternalResolution();
		//ENGINE_ERROR_BOOL_MESSAGE(myModelFactory->Init(myFramework), "Model Factory could not be initiliazed.");
		//ENGINE_ERROR_BOOL_MESSAGE(myCameraFactory->Init(myWindowHandler), "Camera Factory could not be initialized.");
		//ENGINE_ERROR_BOOL_MESSAGE(CMainSingleton::MaterialHandler().Init(myFramework), "Material Handler could not be initialized.");
		//myRenderManager = new CRenderManager();
		//ENGINE_ERROR_BOOL_MESSAGE(myRenderManager->Init(myFramework, myWindowHandler), "RenderManager could not be initialized.");
		//ENGINE_ERROR_BOOL_MESSAGE(myLightFactory->Init(*this), "Light Factory could not be initialized.");
		//ENGINE_ERROR_BOOL_MESSAGE(myParticleFactory->Init(myFramework), "Particle Factory could not be initialized.");
		//ENGINE_ERROR_BOOL_MESSAGE(myVFXFactory->Init(myFramework), "VFX Factory could not be initialized.");
		//ENGINE_ERROR_BOOL_MESSAGE(myLineFactory->Init(myFramework), "Line Factory could not be initialized.");
		//ENGINE_ERROR_BOOL_MESSAGE(mySpriteFactory->Init(myFramework), "Sprite Factory could not be initialized.");
		//ENGINE_ERROR_BOOL_MESSAGE(myTextFactory->Init(myFramework), "Text Factory could not be initialized.");
		//ENGINE_ERROR_BOOL_MESSAGE(myDecalFactory->Init(myFramework), "Decal Factory could not be initialized.");
		//ENGINE_ERROR_BOOL_MESSAGE(myInputMapper->Init(), "InputMapper could not be initialized.");

		//ENGINE_ERROR_BOOL_MESSAGE(CMainSingleton::PopupTextService().Init(), "Popup Text Service could not be initialized.");
		//ENGINE_ERROR_BOOL_MESSAGE(CMainSingleton::DialogueSystem().Init(), "Dialogue System could not be initialized.");
		//ENGINE_ERROR_BOOL_MESSAGE(myPhysxWrapper->Init(), "PhysX could not be initialized.");
		InitWindowsImaging();

		return true;
	}

	float CEngine::BeginFrame()
	{
#ifdef _DEBUG
		std::string fpsString = std::to_string((1.0f / CTimer::Dt()));
		size_t decimalIndex = fpsString.find_first_of('.');
		fpsString = fpsString.substr(0, decimalIndex);
		myWindowHandler->SetWindowTitle("Havtorn Editor | FPS: " + fpsString);

		ImGui_ImplDX11_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();
#endif

		return CTimer::Mark();
	}

	void CEngine::Update()
	{
		//if (mySceneMap.find(myActiveState) != mySceneMap.end())
		//{
		//	if (CTimer::FixedTimeStep() == true)
		//	{
		//		myPhysxWrapper->Simulate(); //<-- Anropas i samma intervall som Fixed "är"
		//		mySceneMap[myActiveState]->FixedUpdate();
		//	}
		//	mySceneMap[myActiveState]->Update();
		//}

		//myAudioManager->Update();
		//CMainSingleton::DialogueSystem().Update();
		//myDebug->Update();
		//CSceneFactory::Get()->Update(); //Used for loading Scenes on a seperate Thread!
	}

	void CEngine::RenderFrame()
	{
		if (!myRenderSceneActive)
			return;

		//ENGINE_BOOL_POPUP(mySceneMap[myActiveState], "The Scene you want to render is nullptr");
		//myRenderManager->Render(*mySceneMap[myActiveState]);
		//CMainSingleton::ImguiManager().Update();
		ImguiManager->Update();
	}

	void CEngine::EndFrame()
	{
#ifdef _DEBUG
		ImGui::Render();
		ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
#endif // _DEBUG

		myFramework->EndFrame();
	}

	CWindowHandler* CEngine::GetWindowHandler()
	{
		return myWindowHandler;
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

	void CEngine::CrashWithScreenShot(std::wstring& /*aSubPath*/)
	{
		//DL_Debug::CDebug::GetInstance()->CopyToCrashFolder(aSubPath);

		//aSubPath += L"\\screenshot.bmp";
		//HRESULT hr = CoInitialize(nullptr);
		//hr = SaveWICTextureToFile(myFramework->GetContext(), myFramework->GetBackbufferTexture(),
		//	GUID_ContainerFormatBmp, aSubPath.c_str(),
		//	&GUID_WICPixelFormat16bppBGR565);

		//if (FAILED(hr)) {
		//	return;
		//}
		//CoUninitialize();
	}

	void CEngine::SetResolution(SVector2<F32> aResolution)
	{
		myWindowHandler->SetResolution(aResolution);
		//myRenderManager->Release();
		//myRenderManager->ReInit(myFramework, myWindowHandler);
		//mySceneMap[CStateStack::EState::InGame]->ReInitCanvas(ASSETPATH("Assets/Graphics/UI/JSON/UI_MainMenu.json"));
		//mySceneMap[CStateStack::EState::PauseMenu]->ReInitCanvas(ASSETPATH("Assets/Graphics/UI/JSON/UI_PauseMenu.json"));
	}

	CEngine* CEngine::GetInstance()
	{
		return ourInstance;
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
	//	return myAudioManager->AddSource(aData);
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
		//myModelFactory->ClearFactory();
	}

	void CEngine::ShowCursor(const bool& anIsInEditorMode)
	{
		myWindowHandler->ShowAndUnlockCursor(anIsInEditorMode);
	}
	void CEngine::HideCursor(const bool& anIsInEditorMode)
	{
		myWindowHandler->HideAndLockCursor(anIsInEditorMode);
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
	//	myWindowHandler->GameIsInMenu(isInMenu);
	//}

	//void CEngine::SetBrokenScreen(bool aShouldSetBrokenScreen)
	//{
	//	myRenderManager->SetBrokenScreen(aShouldSetBrokenScreen);
	//}

	//const CFullscreenRenderer::SPostProcessingBufferData& CEngine::GetPostProcessingBufferData() const
	//{
	//	return myRenderManager->GetPostProcessingBufferData();
	//}

	//void CEngine::SetPostProcessingBufferData(const CFullscreenRenderer::SPostProcessingBufferData& someBufferData)
	//{
	//	myRenderManager->SetPostProcessingBufferData(someBufferData);
	//}

	//void CEngine::SetAudioListener(CGameObject* aGameObject)
	//{
	//	myAudioManager->SetListener(aGameObject);
	//}

	//void CEngine::SetIsMenu(bool aMenuIsOpen)
	//{
	//	myWindowHandler->GameIsInMenu(aMenuIsOpen);
	//}

}