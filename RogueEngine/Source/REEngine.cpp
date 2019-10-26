#define _CRT_SECURE_NO_WARNINGS

#include "Logger.h"
#include "InputManager.h"
#include "EventDispatcher.h"
#include "REEngine.h"
#include "GLHelper.hpp"
#include "ComponentList.h"
#include "Main.h"
#include "Editor.h"
#include "PhysicsSystem.h"
#include "GraphicsSystem.h"
#include "LogicSystem.h"
#include "DebugDrawSystem.h"
#include "FontSystem.h"
#include "CollisionSystem.h"
#include "VSync.h"

namespace Rogue
{
	REEngine::REEngine() :
		m_coordinator{}, m_accumulatedTime{ 0.0f }, m_stepCount{ 0 },
		m_gameIsRunning{ true }, m_projMat{ 1.0f }
	{}

	void REEngine::RegisterSystems()
	{
		m_coordinator.RegisterSystem<InputManager>();
		m_coordinator.RegisterSystem<LogicSystem>();
		m_coordinator.RegisterSystem<EventDispatcher>();
		m_coordinator.RegisterSystem<PhysicsSystem>();
		m_coordinator.RegisterSystem<CollisionSystem>();
		m_coordinator.RegisterSystem<GraphicsSystem>();
		m_coordinator.RegisterSystem<DebugDrawSystem>();
		m_coordinator.RegisterSystem<FontSystem>();
		m_coordinator.RegisterSystem<Editor>();
	}

	void REEngine::RegisterComponents()
	{
		m_coordinator.RegisterComponent<SpriteComponent>();
		m_coordinator.RegisterComponent<RigidbodyComponent>();
		m_coordinator.RegisterComponent<TransformComponent>();
		m_coordinator.RegisterComponent<CircleCollider2DComponent>();
		m_coordinator.RegisterComponent<BoxCollider2DComponent>();
		m_coordinator.RegisterComponent<PlayerControllerComponent>();
		m_coordinator.RegisterComponent<LogicComponent>();
		m_coordinator.RegisterComponent<StatsComponent>();
	}

	void REEngine::init()
	{
		config.ConfigInit();

		hWnd = CreateOpenGLWindow(const_cast<char*>(config.GetTitle().c_str()), config.GetX(), config.GetY(),
			config.GetWidth(), config.GetHeight(), 0, config.GetFlags());

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

		setVSync(0);

		// Register all systems.
		RegisterSystems();

		// Register all components
		RegisterComponents();

		// Init systems and system signatures will be set in their respective inits.
		// Other systems and managers will also be initialized here.
		m_coordinator.Init();
	}

	void REEngine::update()
	{
		m_stepCount = 0;
		Timer::ChronoClock mainLoopTimer;
		g_fixedDeltaTime = 1/60.0f;

		while (m_gameIsRunning)
		{
			g_deltaTime = std::chrono::duration_cast<std::chrono::microseconds>(m_loopEnd - m_loopStart).count() / Timer::s_microsecondsPerSecond;

			m_loopStart = mainLoopTimer.now();

			m_stepCount = 0;

			m_accumulatedTime += g_deltaTime;

			while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}

			while (m_accumulatedTime >= g_fixedDeltaTime)
			{
				m_accumulatedTime -= g_fixedDeltaTime;
				m_stepCount++;
			}
			glViewport(0, 0, GetWindowWidth(hWnd), GetWindowHeight(hWnd));

			m_coordinator.Update();

			SwapBuffers(hDC);

			//m_projMat = glm::ortho(-16.0f * 0.5f, 16.0f * 0.5f, -9.0f * 0.5f, 9.0f * 0.5f, -1024.0f, 1024.0f);
			m_projMat = glm::ortho(-GetWindowWidth(hWnd) * 0.5f, GetWindowWidth(hWnd) * 0.5f, -GetWindowHeight(hWnd) * 0.5f, GetWindowHeight(hWnd) * 0.5f, -1024.0f, 1024.0f);

			m_loopEnd = mainLoopTimer.now();
		}
	}

	void REEngine::shutdown()
	{
		//put graphics shutdown here
		wglMakeCurrent(NULL, NULL);
		ReleaseDC(hWnd, hDC);
		wglDeleteContext(hRC);
		DestroyWindow(hWnd);
	}

	HWND REEngine::GetWindowHandler() const
	{
		return hWnd;
	}

	glm::mat4 REEngine::GetProjMat() const
	{
		return m_projMat;
	}

	float REEngine::GetAccumulatedTime() const
	{
		return m_accumulatedTime;
	}

	int REEngine::GetStepCount() const
	{
		return m_stepCount;
	}

	void REEngine::SetGameIsRunning(bool set)
	{
		m_gameIsRunning = set;
	}

	HWND REEngine::CreateOpenGLWindow(char* title, int x, int y, int width, int height,
		BYTE type, DWORD flags)
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

		hWnd = CreateWindow("OpenGL", title, WS_OVERLAPPEDWINDOW |
			WS_CLIPSIBLINGS | WS_CLIPCHILDREN,
			x, y, width, height, NULL, NULL, hInstance, NULL);

		if (hWnd == NULL)
		{
			std::cout << GetLastError() << std::endl;
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
}