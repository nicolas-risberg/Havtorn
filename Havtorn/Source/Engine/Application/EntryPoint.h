#pragma once
#include "Havtorn.h"
#include <iostream>

#ifdef HV_PLATFORM_WINDOWS

extern Havtorn::CApplication* Havtorn::CreateApplication();

#pragma region Console

#ifdef _DEBUG
#define USE_CONSOLE
#endif
void OpenConsole()
{
	::AllocConsole();
	FILE* stream;
	freopen_s(&stream, "CONIN$", "r", stdin);
	freopen_s(&stream, "CONOUT$", "w", stdout);
	freopen_s(&stream, "CONOUT$", "w", stderr);
	setvbuf(stdin, NULL, _IONBF, NULL);
	setvbuf(stdout, NULL, _IONBF, NULL);
	setvbuf(stderr, NULL, _IONBF, NULL);
}

void CloseConsole()
{
	fclose(stdin);
	fclose(stdout);
	fclose(stderr);
}

#pragma endregion

using namespace Havtorn;

int WINAPI wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nShowCmd)
{
	hInstance;
	hPrevInstance;
	lpCmdLine;
	nShowCmd;

#ifdef USE_CONSOLE
	OpenConsole();
#endif

	GLog::Init();

	auto application = CreateApplication();
	application->Run();
	delete application;

	HV_LOG_TRACE("Trace");
	HV_LOG_DEBUG("Debug");
	HV_LOG_INFO("Info");
	HV_LOG_WARNING("Warning");
	HV_LOG_ERROR("Error");
	HV_LOG_CRITICAL("Critical");

	SVector haqvin;
	haqvin = SVector(1.0f, 1.0f, 0.0f);

	SVector nico = { -2.0f, 0.0f, 0.0f };

	SVector projection = nico.Projection(haqvin);
	HV_LOG_WARNING("Projection Vector: {}", projection.ToString());

	system("pause");

#ifdef USE_CONSOLE
	CloseConsole();
#endif

	return 0;
}

#endif