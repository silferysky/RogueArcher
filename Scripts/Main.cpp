#define _CRT_SECURE_NO_WARNINGS

#include <cstdio>
#include <cstdlib>

#include <fstream>
#include "Main.h"
#include "KeyEvent.h"
#include "InputManager.h"
#include "MemoryManager.h"
#include "Library.h"
#include "FileIO.h"

#include "Quad.h"
#include "SOIL.h"

GLuint texture[1];

double t = 0.0;
double gdt = 1.0;
bool off = true;
//SystemManager *SysManager = new SystemManager();

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
		off = false;
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

	hWnd = CreateOpenGLWindow(const_cast<char*>("Rogue Engine"), 0, 0, 640, 480, PFD_TYPE_RGBA, 0);
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

	if (glewInit() != GLEW_OK)
		return -1;

	//Logger
	Logger::InitLogger();
	RE_CORE_TRACE("Init Core Logger");

	InputManager* InputMgr = new InputManager();
	RE_INFO("Hello");

	//////////////////////////
	//Graphics Debug
	/////////////////////////

	std::cout << glGetString(GL_VERSION) << std::endl;

	float vertex[8] = { -0.5f,  0.5f,
						 0.5f,  0.5f,
						 0.5f, -0.5f,
						-0.5f, -0.5f, };

	Quad test(vertex, "(1.0, 0.0, 0.0, 0.0)");
	test.CreateShaders();

	while (off)
	{
		while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		//Main Debug
		RE_INFO("INPUT DEBUG");

		int repeat = 0;
		float timer = 0.0f;
		while (repeat < 5)
		{
			InputMgr->UpdateState();

			if (InputMgr->KeyTriggeredAny())
			{
				++repeat;
				//InputMgr->DebugKeyInputs();
			}
		}

		glClear(GL_COLOR_BUFFER_BIT);
		test.Draw();
		SwapBuffers(hDC);
	}

	RE_INFO("TESTING HERE FOR A RANDOM EVENT");
	KeyPressEvent testEvent((KeyPress)KeyArrowRight, 10);
	RE_INFO(testEvent.ToString());
	RE_INFO("END EVENT TEST");

	RE_INFO("TESTING HERE FOR FILE IO");

	Library testLibrary;
	testLibrary.IOTest();

	RE_INFO("END TEST IO");
	
	RE_INFO("TEST ReMM (Rogue Engine Memory Manager)");
	MemoryManager ReMM;

	BaseSystem s;
	BaseSystem s2;
	BaseEntity e;
	BaseComponent c;

	std::shared_ptr<BaseSystem> ptr = std::make_shared<BaseSystem>(s);

	ReMM.AddIntoMemory(s);
	ReMM.AddIntoMemory(s2);
	ReMM.AddIntoMemory(e);
	ReMM.AddIntoMemory(c);

	std::string str("System Count: ");
	str.append(std::to_string(ReMM.SysListCount()));
	RE_INFO(str);

	str.clear();
	str.append("Use Count: ");
	str.append(std::to_string(ReMM.GetSysList()[0].use_count()));

	RE_INFO(str);

	std::cin.get();

	delete InputMgr;

	wglMakeCurrent(NULL, NULL);
	ReleaseDC(hWnd, hDC);
	wglDeleteContext(hRC);
	DestroyWindow(hWnd);

	return msg.wParam;
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

