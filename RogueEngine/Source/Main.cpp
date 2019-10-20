#define _CRT_SECURE_NO_WARNINGS

#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <sstream>
#include <chrono>
#include <Windows.h>

#include "Main.h"
#include "Logger.h"
#include "VSync.h"	
#include "SOIL.h"
#include "Config.h"
#include "WindowHelper.h"
#include "MemoryManager.h"
#include "GameStateList.h"
#include "Resource.h"
#include "ComponentList.h"
#include "BasicIO.h"
#include "GLHelper.hpp"

#if _DEBUG
#include <iostream>
#endif
#include "REMath.h"
#include "REEditor.h"
#include "Editor.h"

REEngine gEngine;
float gDeltaTime;
float gFixedDeltaTime;
bool EditorMode = false;

//const char* FileName = "/Resources/test.json";
static const int SCREEN_FULLSCREEN = 0;
static const int SCREEN_WIDTH = 960;
static const int SCREEN_HEIGHT = 540;

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

	HDC   hDC;				/* device context */
	HGLRC hRC;				/* opengl context */
	HWND  hWnd;				/* window */
	MSG   msg = { 0 };		/* message */

	REConfig config;
	config.ConfigInit();
	hWnd = CreateOpenGLWindow(const_cast<char*>(config.GetTitle().c_str()), config.GetX(), config.GetY(),
		config.GetWidth(), config.GetHeight(), 0, config.GetFlags());

	if (hWnd == NULL)
		exit(1);

	hDC = GetDC(hWnd);
	hRC = wglCreateContext(hDC);
	wglMakeCurrent(hDC, hRC);

	ShowWindow(hWnd, nCmdShow);
	if (EditorMode)
	{
		Rogue::Editor Editor;
		Editor.init();
		Editor.update();
		Editor.Shutdown();

		return (int)msg.wParam;
	}

	AllocConsole();
	(void)freopen("CONIN$", "r", stdin);
	(void)freopen("CONOUT$", "w", stdout);
	(void)freopen("CONOUT$", "w", stderr);


	//Ensures program closes properly 
	SetConsoleCtrlHandler(CtrlHandler, true);

	//setVSync(1);

	gEngine.init();
	
	//////////////////////////
	//Graphics Debug
	/////////////////////////

	std::cout << glGetString(GL_VERSION) << std::endl;

	RE_INFO("TEST FILEWRITER");
	//BasicIO::WriteLevelJsonFile("Resources/TestJsonFileCreator.json", 1);
	//BasicIO::WriteArchetypeJsonFile("Resources/TestArchetypeJsonFile.json", 3);
	//BasicIO::WriteLevelJsonFile("Resources/Level 1.json", 8);

	//gObjectFactory.LoadLevel("Resources/Level 1.json");
	//gObjectFactory.LoadArchetypes("Resources/Archetypes.json");
	//gObjectFactory.SaveArchetypes("Resources/Archetypes.json");

	//gObjectFactory.SaveLevel("Resources/Level 1.json");

	// Update engine.
	gEngine.update(hDC);

	wglMakeCurrent(NULL, NULL);
	ReleaseDC(hWnd, hDC);
	wglDeleteContext(hRC);
	DestroyWindow(hWnd);

	return (int)msg.wParam;

}

