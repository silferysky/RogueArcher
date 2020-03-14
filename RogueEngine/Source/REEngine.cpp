/* Start Header ************************************************************************/
/*!
\file           REEngine.cpp
\project        Exale
\author         Loh Kai Yi,kaiyi.loh,390002918 (25%)
				Lim Jia Hao, l.jiahao, 390006518 (25%)
				Chan Wai Kit Terence, c.terence, 440005918 (25%)
				Javier Foo, javier.foo, 440002318 (25%)
\par            kaiyi.loh\@digipen.edu
				l.jiahao\@digipen.edu
				c.terence\@digipen.edu
				javier.foo\@digipen.edu
\date           1 December,2019
\brief          This file contains the function definitions for REEngine

All content (C) 2019 DigiPen (SINGAPORE) Corporation, all rights
reserved.

Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header **************************************************************************/

#include "Precompiled.h"
#include "Logger.h"
#include "EventDispatcher.h"
#include "REEngine.h"
#include "GLHelper.hpp"
#include "ComponentList.h"
#include "Editor.h"
#include "VSync.h"
#include "CameraManager.h"

// Systems
#include "InputManager.h"
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
#include "CursorSystem.h"
#include "PickingSystem.h"
#include "ParticleSystem.h"
#include "ParticleEmitterSystem.h"
#include "UISystem.h"
#include "ParentChildSystem.h"
#include "MaskingSystem.h"
#include "FadeSystem.h"
#include "LightingSystem.h"
#include "SoulSystem.h"

namespace Rogue
{
	REEngine::REEngine() :
		m_coordinator{}, m_accumulatedTime{ 0.0f }, m_timeScale{ 1.0f }, m_stepCount{ 0 },
		m_gameIsRunning{ true }, m_verticalSync{ false }, m_projMat{ 1.0f }, m_isFocused{ true }
	{}

	void REEngine::RegisterSystems()
	{
		// Order of system updates will be manually set in SystemManager.h
		m_coordinator.RegisterSystem<InputManager>();
		m_coordinator.RegisterSystem<LogicSystem>();
		m_coordinator.RegisterSystem<SoulSystem>();
		m_coordinator.RegisterSystem<ParticleSystem>();
		m_coordinator.RegisterSystem<ParticleEmitterSystem>();
		m_coordinator.RegisterSystem<CursorSystem>();
		m_coordinator.RegisterSystem<PickingSystem>();
		m_coordinator.RegisterSystem<PlayerControllerSystem>();
		m_coordinator.RegisterSystem<MenuControllerSystem>();
		m_coordinator.RegisterSystem<PhysicsSystem>();
		m_coordinator.RegisterSystem<CircleCollisionSystem>();
		m_coordinator.RegisterSystem<BoxCollisionSystem>();
		m_coordinator.RegisterSystem<CollisionSystem>(); // Must be after circle and box collision systems
		m_coordinator.RegisterSystem<GraphicsSystem>();
		m_coordinator.RegisterSystem<FadeSystem>();
		m_coordinator.RegisterSystem<MaskingSystem>();
		m_coordinator.RegisterSystem<AnimationSystem>();
		m_coordinator.RegisterSystem<DebugDrawSystem>();
		m_coordinator.RegisterSystem<FontSystem>();
		m_coordinator.RegisterSystem<CameraSystem>();
		m_coordinator.RegisterSystem<AudioSystem>();
		m_coordinator.RegisterSystem<UISystem>();
		m_coordinator.RegisterSystem<LightingSystem>();
		m_coordinator.RegisterSystem<ParentChildSystem>();
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
		m_coordinator.RegisterComponent<ParticleEmitterComponent>();
		m_coordinator.RegisterComponent<ParticleComponent>();
		m_coordinator.RegisterComponent<MaskingComponent>();
		m_coordinator.RegisterComponent<FadeComponent>();
		m_coordinator.RegisterComponent<LightComponent>();
		m_coordinator.RegisterComponent<ForegroundComponent>();
		m_coordinator.RegisterComponent<SoulComponent>();

		//This is a component that isn't directly serialized/deserialized
		m_coordinator.RegisterComponent<TileMapComponent>();
		m_coordinator.RegisterComponent<ChildComponent>();
		m_coordinator.RegisterComponent<SaveComponent>();

		//==================================================
		// MAX_COMPONENTS: 32
		// CURRENT NO.: 24
		//==================================================
	}

	void REEngine::Init()
	{
		config.ConfigInit();
		g_fixedDeltaTime = 1 / 60.0f;

#if	INIT_CONSOLE
		AllocConsole();
		(void)freopen("CONIN$", "r", stdin);
		(void)freopen("CONOUT$", "w", stdout);
		(void)freopen("CONOUT$", "w", stderr);
#endif
#if INIT_CURSOR
		//ShowCursor(true);
#else
		ShowCursor(false);
#endif
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

#if INIT_FULLSCREEN
		SetWindowLongPtr(hWnd, GWL_STYLE, WS_VISIBLE | WS_POPUP);
		SetWindowPos(hWnd, HWND_TOP, 0, 0, GetSystemMetrics(SM_CXSCREEN),
			GetSystemMetrics(SM_CYSCREEN), SWP_FRAMECHANGED);
#endif
#if INIT_VSYNC
		setVSync(true);
#else
		setVSync(false);
#endif

#if INIT_PLAY
		m_coordinator.SetGameState(true);
#endif

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
		static Timer::ChronoClock mainLoopTimer;
		
		while (m_gameIsRunning)
		{
			g_deltaTime = std::chrono::duration_cast<std::chrono::microseconds>(m_loopEnd - m_loopStart).count() / Timer::s_microsecPerSec;
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
		//	if (m_accumulatedTime > 0.2f)
		//		m_accumulatedTime = 0.2f;

			while (m_accumulatedTime >= g_fixedDeltaTime)
			{
				m_accumulatedTime -= g_fixedDeltaTime;
				m_stepCount++;
			}

			m_coordinator.Update();

			m_dimensions = Vec2{ m_size, aspect_ratio * m_size } * 0.5f * CameraManager::instance().GetCameraZoom();
			m_projMat = glm::ortho(-m_dimensions.x, m_dimensions.x, -m_dimensions.y, m_dimensions.y, -24.0f, 24.0f);
			
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

	int REEngine::GetEngineWindowWidth() const
	{
		return GetWindowWidth(hWnd);
	}

	int REEngine::GetEngineWindowHeight() const
	{
		return GetWindowHeight(hWnd);
	}

	Vec2 REEngine::GetWorldDimensions() const
	{
		return m_dimensions;
	}

	bool REEngine::GetIsFocused() const
	{
		return m_isFocused;
	}

	bool REEngine::GetGameIsRunning() const
	{
		return m_gameIsRunning;
	}

	void REEngine::SetGameIsRunning(bool set)
	{
		m_gameIsRunning = set;
	}

	void REEngine::SetTimeScale(float timeScale)
	{
		m_timeScale = timeScale;
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

	void REEngine::SetWorldDimensions(const Vec2& dim)
	{
		m_dimensions = dim;
	}

	void REEngine::SetIsFocused(bool set)
	{
		m_isFocused = set;
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