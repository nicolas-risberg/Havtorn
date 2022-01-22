// Copyright 2022 Team Havtorn. All Rights Reserved.

#include "hvpch.h"

#include "WindowHandler.h"
//#include "Input.h"
//#include "JsonReader.h"
#ifdef _DEBUG
    #include "ImGui/Core/imgui_impl_win32.h"
#endif
//#include "PostMaster.h"

#ifdef _DEBUG
IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
#endif // DEBUG

namespace Havtorn
{
    LRESULT CWindowHandler::WinProc(_In_ HWND hwnd, _In_ UINT uMsg, _In_ WPARAM wParam, _In_ LPARAM lParam)
    {
        static CWindowHandler* windowHandler = nullptr;
        CREATESTRUCT* createStruct;

#ifdef _DEBUG
        if (ImGui_ImplWin32_WndProcHandler(hwnd, uMsg, wParam, lParam))
        {
            return true;
        }
#endif

        switch (uMsg)
        {
        case WM_CLOSE:

        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;

        case WM_CREATE:
            createStruct = reinterpret_cast<CREATESTRUCT*>(lParam);
            windowHandler = reinterpret_cast<CWindowHandler*>(createStruct->lpCreateParams);
            break;

        case WM_KILLFOCUS:
            windowHandler->LockCursor(false); // If we use this here the WindowIsInEditingMode bool will be preserved
            break;

        case WM_SETFOCUS:
#ifdef _DEBUG
            //if (false/*windowHandler->GameIsInMenu*/)
                windowHandler->ShowAndUnlockCursor();
            //else
                //windowHandler->HideAndLockCursor();
            //windowHandler->WindowIsInEditingMode ? windowHandler->LockCursor(false) : windowHandler->LockCursor(true);
#else
            //if (windowHandler->myGameIsInMenu)
                windowHandler->ShowAndUnlockCursor();
            //else
                //windowHandler->HideAndLockCursor();
#endif
            break;

        default:
            //Input::GetInstance()->UpdateEvents(uMsg, wParam, lParam);
            break;
        }

        return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }

    CWindowHandler::CWindowHandler()
        : WindowData()
        , WindowHandle(0)
        , Resolution(nullptr)
        , ResolutionScale(1.0f)
        , CursorIsLocked(false)
        , WindowIsInEditingMode(false)
    {
    }

    CWindowHandler::~CWindowHandler()
    {
//#ifdef _DEBUG
//        ImGui_ImplWin32_Shutdown();
//#endif // _DEBUG

        LockCursor(false);
        CursorIsLocked = false;
        WindowIsInEditingMode = false;
        WindowHandle = 0;
        delete Resolution;
        Resolution = nullptr;
        UnregisterClass(L"HavtornWindowClass", GetModuleHandle(nullptr));
    }

    bool CWindowHandler::Init(CWindowHandler::SWindowData someWindowData)
    {
        WindowData = someWindowData;

        //rapidjson::Document document = CJsonReader::Get()->LoadDocument("Json/Settings/WindowSettings.json");

        //if (document.HasMember("Window Width"))
        //    WindowData.myWidth = document["Window Width"].GetInt();

        //if (document.HasMember("Window Height"))
        //    WindowData.myHeight = document["Window Height"].GetInt();

        //if (document.HasMember("Window Starting Pos X"))
        //    WindowData.myX = document["Window Starting Pos X"].GetInt();

        //if (document.HasMember("Window Starting Pos Y"))
        //    WindowData.myY = document["Window Starting Pos Y"].GetInt();

        //HCURSOR customCursor = NULL;
        //if (document.HasMember("Cursor Path"))
        //    customCursor = LoadCursorFromFileA(document["Cursor Path"].GetString());

        //if (customCursor == NULL)
        //    customCursor = LoadCursor(nullptr, IDC_ARROW);

        //HICON customIcon = NULL;
        //if (document.HasMember("Icon Path"))
        //    customIcon = (HICON)LoadImageA(NULL, document["Icon Path"].GetString(), IMAGE_ICON, 0, 0, LR_LOADFROMFILE | LR_DEFAULTSIZE);

        //if (customIcon == NULL)
        //    customIcon = (HICON)LoadImageA(NULL, "ironwrought.ico", IMAGE_ICON, 0, 0, LR_LOADFROMFILE | LR_DEFAULTSIZE);

        WNDCLASSEX windowclass = {};
        windowclass.cbSize = sizeof(WNDCLASSEX);
        windowclass.style = CS_VREDRAW | CS_HREDRAW | CS_OWNDC;
        windowclass.lpfnWndProc = CWindowHandler::WinProc;
        windowclass.cbClsExtra = 0;
        windowclass.cbWndExtra = 0;
        windowclass.hInstance = GetModuleHandle(nullptr);
        //windowclass.hIcon = customIcon;
        //windowclass.hCursor = customCursor;
        windowclass.lpszClassName = L"HavtornWindowClass";
        RegisterClassEx(&windowclass);

        std::string gameName = "Havtorn Editor";
        //if (document.HasMember("Game Name"))
        //{
        //    gameName = document["Game Name"].GetString();
        //}

        //bool borderless = false;
        //if (document.HasMember("Borderless Window"))
        //{
        //    borderless = document["Borderless Window"].GetBool();
        //}

        //MaxResX = GetSystemMetrics(SM_CXSCREEN);
        //MaxResY = GetSystemMetrics(SM_CYSCREEN);

        if (false/*borderless*/)
        {
#ifdef _DEBUG
            // Start in borderless
            WindowHandle = CreateWindowA("HavtornWindowClass", gameName.c_str(),
                WS_POPUP | WS_VISIBLE,
                0, 0, WindowData.myWidth, WindowData.myHeight,
                NULL, NULL, GetModuleHandle(nullptr), this);
#else
            // Start in borderless
            WindowHandle = CreateWindowA("HavtornWindowClass", gameName.c_str(),
                WS_POPUP | WS_VISIBLE,
                0, 0, MaxResX, MaxResY,
                NULL, NULL, GetModuleHandle(nullptr), this);
#endif
        }
        else
        {
            // Start in bordered window
            WindowHandle = CreateWindowA("HavtornWindowClass", gameName.c_str(),
                WS_OVERLAPPEDWINDOW | WS_POPUP | WS_VISIBLE,
                WindowData.myX, WindowData.myY, WindowData.myWidth, WindowData.myHeight,
                nullptr, nullptr, nullptr, this);
        }

        //::SetCursor(customCursor);

//#ifdef _DEBUG
        //ImGui_ImplWin32_Init(WindowHandle);
//#endif // _DEBUG

        //LockCursor(true);

        //HBRUSH hBrush = CreateSolidBrush(RGB(255, 153, 0));

        //MENUINFO mi = { 0 };
        //mi.cbSize = sizeof(mi);
        //mi.fMask = MIM_BACKGROUND | MIM_APPLYTOSUBMENUS;
        //mi.hbrBack = hBrush;

        //HMENU hMenu = ::GetMenu(WindowHandle);
        //SetMenuInfo(hMenu, &mi);

        Resolution = new SVector2<F32>();
        return true;
    }

    const HWND CWindowHandler::GetWindowHandle() const
    {
        return WindowHandle;
    }

    SVector2<F32> CWindowHandler::GetCenterPosition()
    {
        SVector2<F32> center = {};
        RECT rect = { 0 };
        if (GetWindowRect(WindowHandle, &rect))
        {
            center.X = (rect.right - rect.left) * 0.5f;
            center.Y = (rect.bottom - rect.top) * 0.5f;
        }
        return center;
    }

    SVector2<F32> CWindowHandler::GetResolution()
    {
        return *Resolution;
    }

    void CWindowHandler::SetResolution(SVector2<F32> aResolution)
    {
        if ((I16)aResolution.X <= MaxResX && (I16)aResolution.Y <= MaxResY)
        {
            ::SetWindowPos(WindowHandle, 0, 0, 0, (U16)aResolution.X, (U16)aResolution.Y, SWP_NOMOVE | SWP_NOOWNERZORDER | SWP_NOZORDER);

            SetInternalResolution();
        }
    }

    const bool CWindowHandler::CursorLocked() const
    {
        return CursorIsLocked;
    }

    void CWindowHandler::LockCursor(bool aShouldLock)
    {
        CursorIsLocked = aShouldLock;
        if (aShouldLock)
        {
            while (::ShowCursor(FALSE) >= 0);

            SVector2<F32> center = GetCenterPosition();
            SetCursorPos(static_cast<I16>(center.X), static_cast<I16>(center.Y));
        }
        else {
            while (::ShowCursor(TRUE) < 0);
        }
        aShouldLock ? static_cast<bool>(SetCapture(WindowHandle)) : static_cast<bool>(ReleaseCapture());
    }

    void CWindowHandler::HidLockCursor(bool aShouldLock)
    {
        CursorIsLocked = aShouldLock;
        if (aShouldLock)
        {
            SVector2<F32> center = GetCenterPosition();
            SetCursorPos(static_cast<I16>(center.X), static_cast<I16>(center.Y));
        }
        aShouldLock ? static_cast<bool>(SetCapture(WindowHandle)) : static_cast<bool>(ReleaseCapture());
    }

    void CWindowHandler::HideAndLockCursor(const bool& anIsInEditorMode)
    {
        while (::ShowCursor(FALSE) >= 0);
        SetCapture(WindowHandle);

        CursorIsLocked = true;
        WindowIsInEditingMode = anIsInEditorMode;

        SVector2<F32> center = GetCenterPosition();
        SetCursorPos(static_cast<I16>(center.X), static_cast<I16>(center.Y));

        //CMainSingleton::PostMaster().Send({ EMessageType::CursorHideAndLock, nullptr });
    }

    void CWindowHandler::ShowAndUnlockCursor(const bool& anIsInEditorMode)
    {
        while (::ShowCursor(TRUE) < 0);
        ReleaseCapture();

        CursorIsLocked = false;
        WindowIsInEditingMode = anIsInEditorMode;

        //CMainSingleton::PostMaster().Send({ EMessageType::CursorShowAndUnlock, nullptr });
    }

    void CWindowHandler::SetInternalResolution()
    {
        LPRECT rect = new RECT{ 0, 0, 0, 0 };
        if (GetClientRect(WindowHandle, rect) != 0) 
        {
            Resolution->X = static_cast<F32>(rect->right);
            Resolution->Y = static_cast<F32>(rect->bottom);
        }
        //ClipCursor(rect);
        delete rect;

        ResolutionScale = Resolution->Y / 1080.0f;
    }

    void CWindowHandler::SetWindowTitle(std::string aString)
    {
        SetWindowTextA(WindowHandle, aString.c_str());
    }

    const float CWindowHandler::GetResolutionScale() const
    {
        return ResolutionScale;
    }
}
