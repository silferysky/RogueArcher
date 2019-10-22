#pragma once
#ifndef WINDOWSYSTEM_H
#define WINDOWSYSTEM_H

#define _CRT_SECURE_NO_WARNINGS

#include "Timer.h"
#include "../Source/Main.h"
#include "../Source/EventDispatcher.h"
#include "Config.h"
#include "REEngine.h"

namespace Rogue
{
	class Timer;
	class REConfig;

	class WindowSystem : public System, public EventListener
	{
		HDC   hDC;				/* device context */
		HGLRC hRC;				/* opengl context */
		HWND  hWnd;				/* window */
		MSG   msg = { 0 };		/* message */

		int         pf;
		WNDCLASS    wc;
		PIXELFORMATDESCRIPTOR pfd{ 0 };
		HINSTANCE hInstance;

		REConfig config;
	public:
		WindowSystem();
		~WindowSystem();

		void init();
		void update() override;

		void receive(Event* ev)
		{}

		HWND CreateOpenGLWindow(char* title, int x, int y, int width, int height,
			BYTE type, DWORD flags);

		MSG& GetMSG();
		HDC& GetHDC();
	};
}
#endif