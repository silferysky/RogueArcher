

#include "Precompiled.h"
#include "Logger.h"
#include "InputManager.h"
#include "EventDispatcher.h"
#include "REEngine.h"
#include "GLHelper.hpp"
#include "ComponentList.h"
#include "Main.h"
#include "Editor.h"
#include "PhysicsSystem.h"
#include "CollisionTagSystem.h"
#include "CircleCollisionSystem.h"
#include "BoxCollisionSystem.h"
#include "CollisionSystem.h"
#include "GraphicsSystem.h"
#include "LogicSystem.h"
#include "DebugDrawSystem.h"
#include "FontSystem.h"
#include "PlayerControllerSystem.h"
#include "MenuControllerSystem.h"
#include "AnimationSystem.h"
#include "CameraSystem.h"
#include "AudioSystem.h"
#include "VSync.h"
#include "CursorSystem.h"
#include "PickingSystem.h"

namespace Rogue
{
	REEngine::REEngine() :
		m_coordinator{}, m_accumulatedTime{ 0.0f }, m_timeScale{ 1.0f }, m_stepCount{ 0 },
		m_gameIsRunning{ true }, m_verticalSync{ false }, m_projMat{ 1.0f }
	{}

	void REEngine::RegisterSystems()
	{
		m_coordinator.RegisterSystem<InputManager>();
		m_coordinator.RegisterSystem<LogicSystem>();
		m_coordinator.RegisterSystem<CursorSystem>();
		m_coordinator.RegisterSystem<PickingSystem>();
		m_coordinator.RegisterSystem<PlayerControllerSystem>();
		m_coordinator.RegisterSystem<MenuControllerSystem>();
		m_coordinator.RegisterSystem<PhysicsSystem>();
		m_coordinator.RegisterSystem<CircleCollisionSystem>();
		m_coordinator.RegisterSystem<BoxCollisionSystem>();
		m_coordinator.RegisterSystem<CollisionSystem>();
		m_coordinator.RegisterSystem<GraphicsSystem>();
		m_coordinator.RegisterSystem<AnimationSystem>();
		m_coordinator.RegisterSystem<DebugDrawSystem>();
		m_coordinator.RegisterSystem<FontSystem>();
		m_coordinator.RegisterSystem<CameraSystem>();
		m_coordinator.RegisterSystem<AudioSystem>();
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
		m_coordinator.RegisterComponent<AnimationComponent>();
		m_coordinator.RegisterComponent<CameraComponent>();
		m_coordinator.RegisterComponent<AudioEmitterComponent>();
		m_coordinator.RegisterComponent<UIComponent>();
		m_coordinator.RegisterComponent<CursorComponent>();
		m_coordinator.RegisterComponent<ColliderComponent>();
		m_coordinator.RegisterComponent<TextComponent>();
		m_coordinator.RegisterComponent<BackgroundComponent>();
	}

	void REEngine::Init()
	{
		config.ConfigInit();

		AllocConsole();
		(void)freopen("CONIN$", "r", stdin);
		(void)freopen("CONOUT$", "w", stdout);
		(void)freopen("CONOUT$", "w", stderr);

		hWnd = CreateOpenGLWindow(const_cast<char*>(config.GetTitle().c_str()), config.GetX(), config.GetY(),
			config.GetWidth(), config.GetHeight(), 0, config.GetFlags());

		if (hWnd == NULL)
			exit(1);

		hDC = GetDC(hWnd);
		hRC = wglCreateContext(hDC);
		wglMakeCurrent(hDC, hRC);

		ShowWindow(hWnd, SW_SHOW);

		aspect_ratio = (float)config.GetHeight() / config.GetWidth();

		//Ensures program closes properly 
		SetConsoleCtrlHandler(CtrlHandler, true);

		setVSync(false);

		// Register all components
		RegisterComponents();

		// Register all systems.
		RegisterSystems();

		// Init systems and system signatures will be set in their respective inits.
		// Other systems and managers will also be initialized here.
		m_coordinator.Init();
	}

	void REEngine::Update()
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

			// Avoid spiral of death and clamp dt, thus clamping
			// how many times the UpdatePhysics can be called in
			// a single game loop.
			if (m_accumulatedTime > 0.2f)
				m_accumulatedTime = 0.2f;

			while (m_accumulatedTime >= g_fixedDeltaTime)
			{
				m_accumulatedTime -= g_fixedDeltaTime;
				m_stepCount++;
			}
			m_coordinator.Update();

			//m_projMat = glm::ortho(-16.0f * 0.5f, 16.0f * 0.5f, -9.0f * 0.5f, 9.0f * 0.5f, -1024.0f, 1024.0f);
			
			auto m_dimensions = Vec2{ m_size, aspect_ratio * m_size } * 0.5;
			m_projMat = glm::ortho(-m_dimensions.x * m_cameraZoom, m_dimensions.x * m_cameraZoom, -m_dimensions.y * m_cameraZoom, m_dimensions.y * m_cameraZoom, -1024.0f, 1024.0f);
			
			m_loopEnd = mainLoopTimer.now();
		}
	}

	void REEngine::Shutdown()
	{
		m_coordinator.Shutdown();
		wglMakeCurrent(NULL, NULL);
		ReleaseDC(hWnd, hDC);
		wglDeleteContext(hRC);
		DestroyWindow(hWnd);
	}

	void REEngine::SwapBuffer()
	{
		SwapBuffers(hDC);
	}

	HWND REEngine::GetWindowHandler() const
	{
		return hWnd;
	}

	MSG REEngine::GetWindowMessage() const
	{
		return msg;
	}

	glm::mat4 REEngine::GetProjMat() const
	{
		return m_projMat;
	}

	float REEngine::GetAccumulatedTime() const
	{
		return m_accumulatedTime;
	}

	float REEngine::GetTimeScale() const
	{
		return m_timeScale;
	}

	int REEngine::GetStepCount() const
	{
		return m_stepCount;
	}

	Vec2 REEngine::GetWorldCursor() const
	{
		return m_cursorPos;
	}

	ImVec2 REEngine::GetViewportCursor() const
	{
		return m_viewportCursorPos;
	}

	int REEngine::GetEngineWindowWidth() const
	{
		return GetWindowWidth(hWnd);
	}

	int REEngine::GetEngineWindowHeight() const
	{
		return GetWindowHeight(hWnd);
	}

	void REEngine::SetGameIsRunning(bool set)
	{
		m_gameIsRunning = set;
	}

	void REEngine::SetTimeScale(float timeScale)
	{
		m_timeScale = timeScale;
	}

	void REEngine::ZoomIn()
	{
		if (m_cameraZoom > 0.0f)
			m_cameraZoom -= 0.01f;
	}

	void REEngine::ZoomOut()
	{
		m_cameraZoom += 0.01f;
	}

	float& REEngine::GetCameraZoom()
	{
		return m_cameraZoom;
	}

	void REEngine::SetCameraZoom(const float& cameraZoom)
	{
		m_cameraZoom = cameraZoom;
	}

	void REEngine::ToggleVSync()
	{
		if (m_verticalSync)
		{
			m_verticalSync = false;
			setVSync(false);
		}
		else // Vsync is off
		{
			m_verticalSync = true;
			setVSync(true);
		}
	}

	void REEngine::SetWorldCursor(const Vec2& pos)
	{
		m_cursorPos = pos;
	}

	void REEngine::SetViewportCursor(const ImVec2& pos)
	{
		m_viewportCursorPos = pos;
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