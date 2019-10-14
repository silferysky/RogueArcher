#pragma once
#ifndef WINDOWSYSTEM_H
#define WINDOWSYSTEM_H

#define _CRT_SECURE_NO_WARNINGS

#include "Timer.h"
#include "../Source/Main.h"
#include "../Source/REMath.h"
#include "../Source/EventDispatcher.h"
#include "Config.h"

class Timer;
class REConfig;

class WindowSystem : public System, public EventListener
{
	HDC   hDC;				/* device context */
	HGLRC hRC;				/* opengl context */
	HWND  hWnd;				/* window */
	MSG   msg = { 0 };		/* message */
	REConfig* config;
	float gFixedDeltaTime;
	std::chrono::high_resolution_clock timer;
public:
	WindowSystem() = default;
	~WindowSystem();

	void init();
	void update() override;

	void receive(Event* ev)
	{}

	MSG& GetMSG();
};
#endif