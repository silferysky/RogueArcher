#define _CRT_SECURE_NO_WARNINGS

#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include "Main.h"
#include "KeyEvent.h"
#include "InputManager.h"
#include "EventDispatcher.h"
#include "TestSystem.h"
#include <chrono>
#include "GraphicsSystem.h"
#include "VSync.h"
#include "SOIL.h"
#include "Config.h"
#include "WindowHelper.h"


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
	HDC   hDC;				/* device context */
	HGLRC hRC;				/* opengl context */
	HWND  hWnd;				/* window */
	MSG   msg;				/* message */
	REConfig config;
	config.ConfigInit();
	hWnd = CreateOpenGLWindow(const_cast<char*>(config.GetTitle().c_str()), config.GetX(), config.GetY(), config.GetWidth(), config.GetHeight(), config.GetByte(), config.GetFlags());
	if (hWnd == NULL)
		exit(1);

	hDC = GetDC(hWnd);
	hRC = wglCreateContext(hDC);
	wglMakeCurrent(hDC, hRC);

	ShowWindow(hWnd, nCmdShow);

	AllocConsole();
	freopen("CONIN$", "r", stdin);
	freopen("CONOUT$", "w", stdout);
	freopen("CONOUT$", "w", stderr);

	setVSync(1);
	//Logger
	Logger::InitLogger();
	//RE_CORE_TRACE("Init Core Logger");
	InputManager* InputMgr = new InputManager();
	RE_INFO("Logging App info succeeded");

	////////////////////////////////////
	// Create Engine Object (Testing)
	////////////////////////////////////

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

	/*debugStr.clear();
	debugStr.str("");
	debugStr << "Entity 1's Signature: " << gEngine.m_coordinator.GetEntityManager().GetSignature(1).to_ulong();
	RE_INFO(debugStr.str());*/

	//gObjectFactory.SaveLevel("Resources/Level 1.json");

	objInLevel = gObjectFactory.GetActiveEntity().size();
	debugStr.clear();
	debugStr.str("");
	debugStr << "Number of entities at end: " << objInLevel;
	RE_INFO(debugStr.str());

	RE_CORE_INFO("Entity generation complete");

	InputManager* inputMgr = new InputManager();
	TestSystem sys = TestSystem();
	float wasteTimer;
	std::chrono::high_resolution_clock timer;
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
		inputMgr->update();
		EventDispatcher::instance().update();

		SwapBuffers(hDC);

		auto stop = timer.now();
		gDeltaTime = std::chrono::duration_cast<std::chrono::microseconds>(stop - start).count() / 1000000.0f;
		wasteTimer = gDeltaTime;
		//config.SetFPS(30);
		while (wasteTimer <= config.GetFPS())
		{
			stop = timer.now();
			wasteTimer = std::chrono::duration_cast<std::chrono::microseconds>(stop - start).count() / 1000000.0f;
		}

	//	std::cout << "FPS: " << 1 / wasteTimer << std::endl;
	}

	std::cin.get();

	delete InputMgr;

	wglMakeCurrent(NULL, NULL);
	ReleaseDC(hWnd, hDC);
	wglDeleteContext(hRC);
	DestroyWindow(hWnd);

	return (int)msg.wParam;
}

