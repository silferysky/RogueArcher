#pragma once
#include "Coordinator.h"
#include "Config.h"

namespace Rogue
{
	class REConfig;

	class REEngine
	{
		float m_accumulatedTime;
		int	m_stepCount;
		bool m_gameIsRunning;

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

		HWND CreateOpenGLWindow(char* title, int x, int y, int width, int height,
			BYTE type, DWORD flags);
	public:
		REEngine();

		Coordinator m_coordinator;

		void init();
		void update();
		void shutdown();

		HWND GetWindowHandler() const;
		glm::mat4 GetProjMat() const;
		float GetAccumulatedTime() const;
		int GetStepCount() const;
		void SetGameIsRunning(bool set);
	};
}