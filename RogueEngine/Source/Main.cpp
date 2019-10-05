#define _CRT_SECURE_NO_WARNINGS

#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include "Main.h"
#include "TestSystem.h"
#include <chrono>
#include "VSync.h"
#include "SOIL.h"
#include "Config.h"
#include "WindowHelper.h"
#include "MemoryManager.h"
#include "REMath.h"


REEngine gEngine;
float gDeltaTime;
bool gameIsRunning = true;
ObjectFactory gObjectFactory;

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
	MSG   msg = { 0 };				/* message */
	REConfig config;
	config.ConfigInit();
	hWnd = CreateOpenGLWindow(const_cast<char*>(config.GetTitle().c_str()), config.GetX(), config.GetY(), config.GetWidth(), config.GetHeight(),0, config.GetFlags());
	if (hWnd == NULL)
		exit(1);

	hDC = GetDC(hWnd);
	hRC = wglCreateContext(hDC);
	wglMakeCurrent(hDC, hRC);

	ShowWindow(hWnd, nCmdShow);

	AllocConsole();
	(void)freopen("CONIN$", "r", stdin);
	(void)freopen("CONOUT$", "w", stdout);
	(void)freopen("CONOUT$", "w", stderr);

	// Enable run-time memory check for debug builds.
	#if defined(DEBUG) | defined(_DEBUG)
	    _CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
	#endif

	setVSync(1);

	RE_INFO("Logging App info succeeded");

	////////////////////////////////////
	// Create Engine Object (Testing)
	////////////////////////////////////
	Mtx33 test{ 1.0f, 2.0f, 3.0f,
			4.0f, 5.0f, 6.0f,
			7.0f, 8.0f, 9.0f };

	Mtx33 test2 = test * test;

	std::cout << test << std::endl;
	std::cout << test2 << std::endl;


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

	int* value1 = (int*)MemoryManager::instance().operator new(8);
	int* value2 = (int*)MemoryManager::instance().operator new(8);
	MemoryManager::instance().operator delete(value1);
	MemoryManager::instance().operator delete(value2);
	//int* value3 = (int*)MemoryManager::instance().operator new(8);
	//*value1 = 5;

	//gObjectFactory.SaveLevel("Resources/Level 1.json");

	objInLevel = gObjectFactory.GetActiveEntity().size();
	debugStr.clear();
	debugStr.str("");
	debugStr << "Number of entities at end: " << objInLevel;
	RE_INFO(debugStr.str());

	RE_CORE_INFO("Entity generation complete");

	TestSystem sys = TestSystem();
	float wasteTimer;
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
		//Main Debug
		// RE_INFO("INPUT DEBUG");

		// Update engine.
		gEngine.update();

		SwapBuffers(hDC);
		auto stop = timer.now();
		wasteTimer = std::chrono::duration_cast<std::chrono::microseconds>(stop - start).count() / 1000000.0f;
		gDeltaTime = wasteTimer;
		if (gEngine.m_coordinator.FPSChecker())
		{
			config.SetFPS(30);
		}
		else
		{
			config.SetFPS(60);
		}
		while (gDeltaTime <= config.GetFPS())
		{
			stop = timer.now();
			gDeltaTime = std::chrono::duration_cast<std::chrono::microseconds>(stop - start).count() / 1000000.0f;
		}
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
