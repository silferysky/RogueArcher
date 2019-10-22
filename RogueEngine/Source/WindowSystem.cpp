#include "WindowSystem.h"
#include "WindowHelper.h"
#include "VSync.h"

#include "FileIO.h"

namespace Rogue
{
	WindowSystem::WindowSystem()
		: System(SystemID::id_WINDOWSYSTEM) {}


	WindowSystem::~WindowSystem()
	{
		wglMakeCurrent(NULL, NULL);
		ReleaseDC(hWnd, hDC);
		wglDeleteContext(hRC);
		DestroyWindow(hWnd);
	}

	void WindowSystem::init()
	{
		Signature signature;
		gEngine.m_coordinator.SetSystemSignature<WindowSystem>(signature);

		/* rapidjson::Document Windows = RESerialiser::DeserialiseFromFile("Resources/Windows.json");

		x = Windows["x"].GetInt();
		y = Windows["y"].GetInt();
		height = Windows["height"].GetInt();
		width = Windows["width"].GetInt();
		byte = Windows["byte"].GetInt();
		flags = Windows["flags"].GetInt();
		title = Windows["title"].GetString();
		sound = Windows["sound"].GetBool();

		hWnd = CreateOpenGLWindow(const_cast<char*>("hi"), x, y, width, height, 0, flags);

		if (hWnd == NULL)
			exit(1);

		hDC = GetDC(hWnd);
		hRC = wglCreateContext(hDC);
		wglMakeCurrent(hDC, hRC);

		ShowWindow(hWnd, SW_SHOW);

		AllocConsole();
		(void)freopen("CONIN$", "r", stdin);
		(void)freopen("CONOUT$", "w", stdout);
		(void)freopen("CONOUT$", "w", stderr);

		//Ensures program closes properly 
		SetConsoleCtrlHandler(CtrlHandler, true);

		setVSync(1); */
	}

	void WindowSystem::update()
	{
		/* while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		} */

		/* auto start = timer.now();
		auto stop = timer.now();
		gFixedDeltaTime = std::chrono::duration_cast<std::chrono::microseconds>(stop - start).count() / 1000000.0f;
		gDeltaTime = gFixedDeltaTime;

		while (gDeltaTime <= GetFPS())
		{
			stop = timer.now();
			gDeltaTime = std::chrono::duration_cast<std::chrono::microseconds>(stop - start).count() / 1000000.0f;
		} */
	}

	HWND WindowSystem::CreateOpenGLWindow(char* title, int x, int y, int width, int height,
		BYTE type, DWORD flags)
	{
		/* only register the window class once */
		if (!hInstance)
		{
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

			if (!RegisterClass(&wc))
			{
				MessageBox(NULL, "RegisterClass() failed:  "
					"Cannot register window class.", "Error", MB_OK);
				return NULL;
			}
		}

		hWnd = CreateWindow("OpenGL", title, WS_OVERLAPPEDWINDOW |
			WS_CLIPSIBLINGS | WS_CLIPCHILDREN,
			x, y, width, height, NULL, NULL, hInstance, NULL);

		if (hWnd == NULL)
		{
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
		if (pf == 0)
		{
			MessageBox(NULL, "ChoosePixelFormat() failed:  "
				"Cannot find a suitable pixel format.", "Error", MB_OK);
			return 0;
		}

		if (SetPixelFormat(hDC, pf, &pfd) == FALSE)
		{
			MessageBox(NULL, "SetPixelFormat() failed:  "
				"Cannot set format specified.", "Error", MB_OK);
			return 0;
		}

		DescribePixelFormat(hDC, pf, sizeof(PIXELFORMATDESCRIPTOR), &pfd);

		//ReleaseDC(hWnd, hDC);

		return hWnd;
	}

	MSG& WindowSystem::GetMSG()
	{
		return msg;
	}

	HDC& WindowSystem::GetHDC()
	{
		return hDC;
	}

	std::string WindowSystem::GetTitle()
	{
		return title;
	}

	bool WindowSystem::GetSound()
	{
		return sound;
	}


	int WindowSystem::GetX()
	{
		return x;
	}

	int WindowSystem::GetY()
	{
		return y;
	}

	int WindowSystem::GetHeight()
	{
		return height;
	}

	int WindowSystem::GetWidth()
	{
		return width;
	}

	BYTE WindowSystem::GetByte()
	{
		return BYTE(byte);
	}

	int WindowSystem::GetFlags()
	{
		return flags;
	}

	float WindowSystem::GetFPS()
	{
		return FPS;
	}

	int WindowSystem::SetX(int newx)
	{
		x = newx;
		return x;
	}

	int WindowSystem::SetY(int newy)
	{
		y = newy;
		return y;
	}

	int WindowSystem::SetHeight(int newheight)
	{
		height = newheight;
		return height;
	}

	int WindowSystem::SetWidth(int newwidth)
	{
		width = newwidth;
		return width;
	}

	int WindowSystem::SetByte(int newbyte)
	{
		byte = newbyte;
		return byte;
	}

	int WindowSystem::SetFlags(int newflags)
	{
		flags = newflags;
		return flags;
	}

	float WindowSystem::SetFPS(int FPSset)
	{
		FPS = 1 / float(FPSset);
		return FPS;
	}

	bool WindowSystem::SetSound(bool NewSound)
	{
		sound = NewSound;
		return NewSound;
	}
}