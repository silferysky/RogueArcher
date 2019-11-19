#pragma once
#include "Coordinator.h"
#include "Config.h"
#include "Vector2D.h"

namespace Rogue
{
	class REConfig;

	class REEngine
	{
		float m_accumulatedTime;
		int	m_stepCount;
		bool m_gameIsRunning;
		float m_timeScale;
		bool m_verticalSync;

		Vec2 m_cursorPos;
		ImVec2 m_viewportCursorPos;

		Timer::ChronoTime m_loopStart;
		Timer::ChronoTime m_loopEnd;

		void RegisterSystems();
		void RegisterComponents();

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

		REConfig config;

		glm::mat4 m_projMat;
		float m_cameraZoom = 1.0f;

		float aspect_ratio;
		float m_size = 2000.0f;

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

		float& GetCameraZoom();
		void SetCameraZoom(const float& cameraZoom);
		void ZoomIn();
		void ZoomOut();

		void SetGameIsRunning(bool set);
		void SetTimeScale(float timeScale);
		void ToggleVSync();
		void SetWorldCursor(const Vec2& pos);
		void SetViewportCursor(const ImVec2& pos);
	};
}