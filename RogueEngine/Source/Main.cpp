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
bool gameIsRunning = true;
ObjectFactory gObjectFactory;
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

	UNREFERENCED_PARAMETER(hPreviousInst);
	UNREFERENCED_PARAMETER(lpszCmdLine);
	UNREFERENCED_PARAMETER(hCurrentInst);
	HDC   hDC;				/* device context */
	HGLRC hRC;				/* opengl context */
	HWND  hWnd;				/* window */
	MSG   msg = { 0 };		/* message */
	REConfig config;
	config.ConfigInit();
	hWnd = CreateOpenGLWindow(const_cast<char*>(config.GetTitle().c_str()), config.GetX(), config.GetY(), config.GetWidth(), config.GetHeight(),0, config.GetFlags());
	if (hWnd == NULL)
		exit(1);

	hDC = GetDC(hWnd);
	hRC = wglCreateContext(hDC);
	wglMakeCurrent(hDC, hRC);

	ShowWindow(hWnd, nCmdShow);
	if (EditorMode)
	{
		Rogue::EditorManager Editor;
		Editor.Init();
		Editor.Update();
		Editor.Shutdown();
	}

	AllocConsole();
	(void)freopen("CONIN$", "r", stdin);
	(void)freopen("CONOUT$", "w", stdout);
	(void)freopen("CONOUT$", "w", stderr);


// Enable run-time memory check for debug builds.
#if defined(DEBUG) | defined(_DEBUG)
	_CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
#endif

	//Ensures program closes properly 
	SetConsoleCtrlHandler(CtrlHandler, true);

	//setVSync(1);

	RE_INFO("Logging App info succeeded");

	gEngine.init();
	
	//////////////////////////
	//Graphics Debug
	/////////////////////////

	std::cout << glGetString(GL_VERSION) << std::endl;

	//---------------------------------------------------------------//
	//	Generate entities (Can't use object factory in coordinator)	 //
	//---------------------------------------------------------------//
	RE_CORE_INFO("Generating entities...");

	RE_INFO("TEST FILEWRITER");
	BasicIO::WriteJsonFile("Resources/TestJsonFileCreator.json", 1);

	RE_INFO("TEST OBJECT FACTORY");
	std::stringstream debugStr;
	size_t objInLevel = gObjectFactory.GetActiveEntity().size();
	debugStr << "Number of entities at start: " << objInLevel;

	//BasicIO::WriteJsonFile("Resources/Level 1.json", 8);

	RE_INFO(debugStr.str());
	gObjectFactory.LoadLevel("Resources/Level 1.json");

	debugStr.clear();
	debugStr.str("");

	//gObjectFactory.SaveLevel("Resources/Level 1.json");

	std::chrono::high_resolution_clock timer;
	config.SetFPS(60);

	while (gameIsRunning)
	{
		auto start = timer.now();
		while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		// Update engine.
		gEngine.update();

		SwapBuffers(hDC);
		auto stop = timer.now();
	//	if (gEngine.m_coordinator.FPSChecker())
	//	{
	//		config.SetFPS(30);
	//	}
	//	else
	//	{
	//		config.SetFPS(60);
	//	}
	//	while (gDeltaTime <= config.GetFPS())
	//	{
	//		stop = timer.now();
	//		gDeltaTime = std::chrono::duration_cast<std::chrono::microseconds>(stop - start).count() / 1000000.0f;
	//	}


	gDeltaTime = std::chrono::duration_cast<std::chrono::microseconds>(stop - start).count() / 1000000.0f;

		if (gEngine.m_coordinator.performanceChecker())
		{
			std::cout << "FPS: " << 1 / gDeltaTime << std::endl;
		}
	}

	std::cin.get();


	wglMakeCurrent(NULL, NULL);
	ReleaseDC(hWnd, hDC);
	wglDeleteContext(hRC);
	DestroyWindow(hWnd);

	return (int)msg.wParam;

}

