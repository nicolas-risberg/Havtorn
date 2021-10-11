#include <WindowsInclude.h>
#include <EngineTypes.h>
#include <iostream>

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

using namespace NewEngine;

int WINAPI wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nShowCmd)
{
	hInstance;
	hPrevInstance;
	lpCmdLine;
	nShowCmd;

#ifdef USE_CONSOLE
	OpenConsole();
#endif

	SVector3<F32> haqvin;
	haqvin = SVector3(2.0f, 3.0f, 5600.0f);

	SVector3<F32> nico = { 1.0f, 1.0f, 0.0f };

	F32 dot = nico.Dot(haqvin);

	dot = haqvin.X;

	system("pause");

#ifdef USE_CONSOLE
	CloseConsole();
#endif

	return 0;
}