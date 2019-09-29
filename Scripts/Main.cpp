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
#include "Quad.h"
#include "SOIL.h"
#include "Config.h"


REEngine gEngine;
float gDeltaTime;
bool gameIsRunning = true;
ObjectFactory gObjectFactory;

//const char* FileName = "/Resources/test.json";
static const int SCREEN_FULLSCREEN = 0;
static const int SCREEN_WIDTH = 960;
static const int SCREEN_HEIGHT = 540;


LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	static PAINTSTRUCT ps;
	switch (uMsg)
	{
	case WM_PAINT:
		BeginPaint(hWnd, &ps);
		EndPaint(hWnd, &ps);
		return 0;
	case WM_SIZE:
		glViewport(0, 0, LOWORD(lParam), HIWORD(lParam));
		PostMessage(hWnd, WM_PAINT, 0, 0);
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	case WM_RBUTTONUP:
		gameIsRunning = false;
		break;
	}
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

HWND CreateOpenGLWindow(char* title, int x, int y, int width, int height,
	BYTE type, DWORD flags)
{
	int         pf;
	HDC         hDC;
	HWND        hWnd;
	WNDCLASS    wc;
	PIXELFORMATDESCRIPTOR pfd{ 0 };
	static HINSTANCE hInstance = 0;

	/* only register the window class once - use hInstance as a flag. */
	if (!hInstance) {
		hInstance = GetModuleHandle(NULL);
		wc.style = CS_OWNDC;
		wc.lpfnWndProc = (WNDPROC)WndProc;
		wc.cbClsExtra = 0;
		wc.cbWndExtra = 0;
		wc.hInstance = hInstance;
		wc.hIcon = LoadIcon(NULL, IDI_WINLOGO);
		wc.hCursor = LoadCursor(NULL, IDC_ARROW);
		wc.hbrBackground = NULL;
		wc.lpszMenuName = NULL;
		wc.lpszClassName = "OpenGL";

		if (!RegisterClass(&wc)) {
			MessageBox(NULL, "RegisterClass() failed:  "
				"Cannot register window class.", "Error", MB_OK);
			return NULL;
		}
	}

	hWnd = CreateWindow("OpenGL", title, WS_OVERLAPPEDWINDOW |
		WS_CLIPSIBLINGS | WS_CLIPCHILDREN,
		x, y, width, height, NULL, NULL, hInstance, NULL);

	if (hWnd == NULL) {
		MessageBox(NULL, "CreateWindow() failed:  Cannot create a window.",
			"Error", MB_OK);
		return NULL;
	}

	hDC = GetDC(hWnd);

	memset(&pfd, 0, sizeof(pfd));
	pfd.nSize = sizeof(pfd);
	pfd.nVersion = 1;
	pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | flags | PFD_GENERIC_ACCELERATED | PFD_DOUBLEBUFFER;
	pfd.iPixelType = type;
	pfd.cColorBits = 32;

	pf = ChoosePixelFormat(hDC, &pfd);
	if (pf == 0) {
		MessageBox(NULL, "ChoosePixelFormat() failed:  "
			"Cannot find a suitable pixel format.", "Error", MB_OK);
		return 0;
	}

	if (SetPixelFormat(hDC, pf, &pfd) == FALSE) {
		MessageBox(NULL, "SetPixelFormat() failed:  "
			"Cannot set format specified.", "Error", MB_OK);
		return 0;
	}

	DescribePixelFormat(hDC, pf, sizeof(PIXELFORMATDESCRIPTOR), &pfd);

	//ReleaseDC(hWnd, hDC);

	return hWnd;
}


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
	hWnd = CreateOpenGLWindow(const_cast<char*>("Rogue Engine"), config.GetX(), config.GetY(), config.GetWidth(), config.GetHeight(), config.GetByte(), config.GetFlags());
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
	//Logger::InitLogger();
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
	size_t objInLevel = gEngine.m_coordinator.Size("Entity");
	debugStr << "Number of entities at start: " << objInLevel;

	RE_INFO(debugStr.str());
	gObjectFactory.LoadLevel("Resources/Level 1.json");
	gObjectFactory.SaveLevel("Resources/Level 1.json");

	objInLevel = gEngine.m_coordinator.Size("Entity");
	debugStr.clear();
	debugStr.str("");
	debugStr << "Number of entities at end: " << objInLevel;
	RE_INFO(debugStr.str());

	RE_CORE_INFO("Entity generation complete");


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

		int repeat = 0;
		//float timer2 = 0.0f;
		while (repeat < 5)
		{
			InputMgr->UpdateState();
			EventDispatcher::instance().Update();

			//if (InputMgr->KeyTriggeredAny())
			{
				//InputMgr->DebugKeyInputs();
				++repeat;
			}
		}
		glDisable(GL_DEPTH_TEST);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
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

		//std::cout << "FPS: " << 1 / wasteTimer << std::endl;
	}

	RE_INFO("TESTING HERE FOR A EVENT DEBUG");
	KeyPressEvent testEvent(KeyPress::KeyArrowRight, 10);
	RE_INFO(testEvent.ToString());
	RE_INFO(testEvent.GetEventName());
	RE_INFO("END EVENT TEST");

	RE_INFO("MANUAL TEST EVENT DISPATCHER");
	TestSystem testSys = TestSystem((SYSTEMID)2);
	testSys.Receive(&testEvent);

	RE_INFO("EVENT DISPATCHER TEST");
	EventDispatcher::instance().AddEvent(&testEvent);
	EventDispatcher::instance().Update();
	RE_INFO("EVENT DISPATCHER END");

	std::cin.get();

	delete InputMgr;

	wglMakeCurrent(NULL, NULL);
	ReleaseDC(hWnd, hDC);
	wglDeleteContext(hRC);
	DestroyWindow(hWnd);

	return (int)msg.wParam;
}

/* int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
{
	if (_DEBUG)
	{
		std::cout << "DEBUG STATE ACTIVATE" << std::endl;
	}

	// Enable run-time memory check for debug builds.
#if defined(DEBUG) | defined(_DEBUG)
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

	GSM_Initialize(GS_TESTLEVEL);

	while (_current != GS_QUIT)
	{
		if (_current != GS_RESTART) // if level is the same as previous
		{
			GSM_Update(); // update game state type
			//fpLoad();     // load all assets required for new game state
		}
		else
		{
			// retains same game state as previous
			_current = _previous;
			_next = _previous;
		}

		//fpInit(); // initialize current game state

		//The game loop
		while (_current == _next)
		{
			//AESysFrameStart();
			//gdt += AEFrameRateControllerGetFrameRate() / 60;

			//If 1/60 of a second has passed
			if (gdt)
			{
				//AEInputUpdate();
				//InputMger->UpdateState();
				//fpUpdate();
				//fpDraw();

				//t += gdt;
				//gdt = 0;
			}
			//AESysFrameEnd();
		}

		//fpFree();

		if (_next != GS_RESTART)
		{
			//fpUnload();
		}

		_previous = _current;
		_current = _next;
	}
	//AESysExit();
	//delete SysManager;

	std::cin.get();

	return 0;
} */

