#pragma once
#include "Coordinator.h"
#include "Config.h"

namespace Rogue
{
	class REConfig;

	class REEngine
	{
		using ChronoClock = std::chrono::time_point <std::chrono::steady_clock>;

		const float MICRO_TO_SECONDS = 1000000.0f;
		float m_accumulatedTime;
		int	m_stepCount;
		bool m_gameIsRunning;

		ChronoClock m_loopStart;
		ChronoClock m_loopEnd;

		void RegisterSystems();
		void RegisterComponents();

		HDC   hDC;				/* device context */
		HGLRC hRC;				/* opengl context */
		HWND  hWnd;				/* window */
		MSG   msg = { 0 };		/* message */

		int         pf;
		WNDCLASS    wc;
		PIXELFORMATDESCRIPTOR pfd{ 0 };
		HINSTANCE hInstance;

		REConfig config;

		HWND CreateOpenGLWindow(char* title, int x, int y, int width, int height,
			BYTE type, DWORD flags);
	public:
		REEngine();

		Coordinator m_coordinator;

		void init();
		void update();
		void shutdown();

		HWND GetWindowHandler() const;
		float GetAccumulatedTime() const;
		int GetStepCount() const;
		void SetGameIsRunning(bool set);
	};
}