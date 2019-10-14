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
	hWnd = CreateOpenGLWindow(const_cast<char*>(config->GetTitle().c_str()), config->GetX(), config->GetY(), config->GetWidth(), config->GetHeight(), 0, config->GetFlags());
	if (hWnd == NULL)
		exit(1);

	config->ConfigInit();

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

	config->SetFPS(60);

	setVSync(1);
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
	if (gEngine.m_coordinator.FPSChecker())
	{
		config->SetFPS(30);
	}
	else
	{
		config->SetFPS(60);
	}
	while (gDeltaTime <= config->GetFPS())
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