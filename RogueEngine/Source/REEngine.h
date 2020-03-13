/* Start Header ************************************************************************/
/*!
\file           REEngine.h
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
#pragma once

#define DEMO_MODE 0

#if DEMO_MODE
	#define INIT_FULLSCREEN 1
	#define INIT_VSYNC 1
	#define INIT_CONSOLE 0
	#define INIT_CURSOR 0
	#define INIT_EDITOR 0
	#define INIT_PLAY 1
#else
	#define INIT_FULLSCREEN 0
	#define INIT_VSYNC 0
	#define INIT_CONSOLE 1
	#define INIT_CURSOR 1
	#define INIT_EDITOR 1
	#define INIT_PLAY 0
#endif

#if defined(DEBUG) | defined(_DEBUG)
	#define ENABLE_LOGGER 1
	#if ENABLE_LOGGER
		#define ENABLE_CORE_LOGGING 1
	#else
	#define ENABLE_CORE_LOGGING 1
	#endif
	#define ENABLE_ASSERT 1
#elif DEMO_MODE
	#define ENABLE_LOGGER 0
	#if ENABLE_LOGGER
		#define ENABLE_CORE_LOGGING 0
	#else
		#define ENABLE_CORE_LOGGING 0 
	#endif
	#define ENABLE_ASSERT 0
#else
	#define ENABLE_LOGGER 1
	#if ENABLE_LOGGER
		#define ENABLE_CORE_LOGGING 0
	#else
		#define ENABLE_CORE_LOGGING 0
	#endif
	#define ENABLE_ASSERT 1
#endif

#include "Coordinator.h"
#include "Config.h"
#include "Vector2D.h"
namespace Rogue
{
	class REConfig;

	class REEngine
	{
		/*********************************
		******* Engine variables *********
		**********************************/
		// General variables
		float m_timeScale;
		// Game loop variables
		int m_stepCount;
		Timer::ChronoTime m_loopStart;
		Timer::ChronoTime m_loopEnd;
		float m_accumulatedTime;
		bool m_gameIsRunning;

		/*********************************
		******* Window Variables *********
		**********************************/
		HDC   hDC;				/* device context */
		HGLRC hRC;				/* opengl context */
		HWND  hWnd;				/* window */
		MSG   msg = { 0 };		/* message */

		int         pf;
		WNDCLASS    wc;
		PIXELFORMATDESCRIPTOR pfd{ 0 };
		HINSTANCE hInstance;

		REConfig config;			// Config class
		glm::mat4 m_projMat;		// Projection matrix
		float m_cameraZoom = 1.0f;	// Camera zoom
		bool m_verticalSync;		// Vsync
		float m_size = 2000.0f;		// World size of viewport
		Vec2 m_dimensions;			// Viewport world dimensions
		float aspect_ratio;			// Width and height ratio
		bool m_isFocused;

		/*********************************
		******* Private Functions *********
		**********************************/
		void RegisterSystems();
		void RegisterComponents();
		HWND CreateOpenGLWindow(char* title, int x, int y, int width, int height,
			BYTE type, DWORD flags);
	public:
		REEngine();

		Coordinator m_coordinator;

		void Init();
		void Update();
		void Shutdown();

		void SwapBuffer();

		HWND GetWindowHandler() const;
		MSG GetWindowMessage() const;
		glm::mat4 GetProjMat() const;
		float GetAccumulatedTime() const;
		float GetTimeScale() const;
		int GetStepCount() const;
		int GetEngineWindowWidth() const;
		int GetEngineWindowHeight() const;
		Vec2 GetWorldDimensions() const;
		bool GetIsFocused() const;
		bool GetGameIsRunning() const;

		void SetGameIsRunning(bool set);
		void SetTimeScale(float timeScale);
		void ToggleVSync();
		void SetWorldDimensions(const Vec2& dim);
		void SetIsFocused(bool set);
	};
}