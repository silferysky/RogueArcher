#include "WindowSystem.h"
#include "WindowHelper.h"
#include "VSync.h"

WindowSystem::~WindowSystem()
{
	wglMakeCurrent(NULL, NULL);
	ReleaseDC(hWnd, hDC);
	wglDeleteContext(hRC);
	DestroyWindow(hWnd);
}

void WindowSystem::init()
{
	RESerialiser Serialiser;
	rapidjson::Document Windows = RESerialiser::DeserialiseFromFile("Resources/Windows.json");

	x = Windows["x"].GetInt();
	y = Windows["y"].GetInt();
	height = Windows["height"].GetInt();
	width = Windows["width"].GetInt();
	byte = Windows["byte"].GetInt();
	flags = Windows["flags"].GetInt();
	title = Windows["title"].GetString();
	sound = Windows["sound"].GetBool();
	hWnd = CreateOpenGLWindow(const_cast<char*>("hi"), x,y, width, height, 0, flags);

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

	//setVSync(1);
}

void WindowSystem::update()
{
	while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	auto start = timer.now();
	auto stop = timer.now();
	gFixedDeltaTime = std::chrono::duration_cast<std::chrono::microseconds>(stop - start).count() / 1000000.0f;
	gDeltaTime = gFixedDeltaTime;

	while (gDeltaTime <= GetFPS())
	{
		stop = timer.now();
		gDeltaTime = std::chrono::duration_cast<std::chrono::microseconds>(stop - start).count() / 1000000.0f;
	}
	if (gEngine.m_coordinator.performanceChecker())
	{
		std::cout << "FPS: " << 1 / gDeltaTime << std::endl;
	}

	SwapBuffers(hDC);
}

MSG& WindowSystem::GetMSG()
{
	return msg;
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