#define _CRT_SECURE_NO_WARNINGS

#include "Precompiled.h"
#include "Main.h"

#if _DEBUG
#include <iostream>
#endif

Rogue::REEngine g_engine;
float g_deltaTime;
float g_fixedDeltaTime;

//Use for console
int APIENTRY

WinMain(HINSTANCE hCurrentInst, HINSTANCE hPreviousInst,
	LPSTR lpszCmdLine, int nCmdShow)
{
	// Enable run-time memory check for debug builds.
#if defined(DEBUG) | defined(_DEBUG)
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

	UNREFERENCED_PARAMETER(hPreviousInst);
	UNREFERENCED_PARAMETER(lpszCmdLine);
	UNREFERENCED_PARAMETER(hCurrentInst);

	g_engine.Init();
	
	g_engine.Update();

	g_engine.Shutdown();

	return 0;

}