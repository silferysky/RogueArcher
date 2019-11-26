#pragma once
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
		Vec2 m_cursorPos;				// Mouse cursor's world position
		ImVec2 m_viewportCursorPos;		// Mouse cursor's editor viewport position

		// Game loop variables
		Timer::ChronoTime m_loopStart;
		Timer::ChronoTime m_loopEnd;
		float m_accumulatedTime;
		int	m_stepCount;
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
		Vec2 GetWorldCursor() const;
		ImVec2 GetViewportCursor() const;
		int GetEngineWindowWidth() const;
		int GetEngineWindowHeight() const;
		Vec2 GetWorldDimensions() const;
		bool GetIsFocused() const;

		void SetGameIsRunning(bool set);
		void SetTimeScale(float timeScale);
		void ToggleVSync();
		void SetWorldCursor(const Vec2& pos);
		void SetViewportCursor(const ImVec2& pos);
		void SetWorldDimensions(const Vec2& dim);
		void SetIsFocused(bool set);
	};
}