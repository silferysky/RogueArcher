#pragma once
#ifndef WINDOWSYSTEM_H
#define WINDOWSYSTEM_H

#define _CRT_SECURE_NO_WARNINGS

#include "Timer.h"
#include "../Source/Main.h"
#include "../Source/REMath.h"
#include "../Source/EventDispatcher.h"
#include "Config.h"
#include "REEngine.h"
class Timer;
class REConfig;

class WindowSystem : public System, public EventListener
{
	HDC   hDC;				/* device context */
	HGLRC hRC;				/* opengl context */
	HWND  hWnd;				/* window */
	MSG   msg = { 0 };		/* message */

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


	std::string GetTitle();
	bool GetSound();
	int GetX();
	int GetY();
	int GetHeight();
	int GetWidth();
	BYTE GetByte();
	int GetFlags();
	float GetFPS();

	int SetX(int newx);
	int SetY(int newy);
	int SetHeight(int newheight);
	int SetWidth(int newwidth);
	int SetByte(int newbyte);
	int SetFlags(int newflags);
	float SetFPS(int FPSset);
	bool SetSound(bool NewSound);
private:
	float FPS = 1 / 60;
	int x = 0, y = 0, height = 0, width = 0, flags = 0;
	int byte = 0;
	std::string title = "";
	bool sound = true;
};
#endif