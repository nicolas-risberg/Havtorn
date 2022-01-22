// Copyright 2022 Team Havtorn. All Rights Reserved.

//#include <NewEngine.h>
//#include <iostream>
//
//#pragma region Console
//
//#ifdef _DEBUG
//#define USE_CONSOLE
//#endif
//void OpenConsole()
//{
//	::AllocConsole();
//	FILE* stream;
//	freopen_s(&stream, "CONIN$", "r", stdin);
//	freopen_s(&stream, "CONOUT$", "w", stdout);
//	freopen_s(&stream, "CONOUT$", "w", stderr);
//	setvbuf(stdin, NULL, _IONBF, NULL);
//	setvbuf(stdout, NULL, _IONBF, NULL);
//	setvbuf(stderr, NULL, _IONBF, NULL);
//}
//
//void CloseConsole()
//{
//	fclose(stdin);
//	fclose(stdout);
//	fclose(stderr);
//}
//
//#pragma endregion
//
//using namespace NewEngine;
//
//int WINAPI wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nShowCmd)
//{
//	hInstance;
//	hPrevInstance;
//	lpCmdLine;
//	nShowCmd;
//
//#ifdef USE_CONSOLE
//	OpenConsole();
//#endif
//
//	GLog::Init();
//
//	NE_LOG_TRACE("Trace");
//	NE_LOG_DEBUG("Debug");
//	NE_LOG_INFO("Info");
//	NE_LOG_WARNING("Warning");
//	NE_LOG_ERROR("Error");
//	NE_LOG_CRITICAL("Critical");
//
//	SVector haqvin;
//	haqvin = SVector(1.0f, 1.0f, 0.0f);
//
//	SVector nico = { -2.0f, 0.0f, 0.0f };
//
//	SVector projection = nico.Projection(haqvin);
//	NE_LOG_WARNING("Projection Vector: {}", projection.ToString());
//
//	system("pause");
//
//#ifdef USE_CONSOLE
//	CloseConsole();
//#endif
//
//	return 0;
//}
