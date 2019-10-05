#pragma once

#include <Windows.h>
#include "wtypes.h"
#include "Main.h"

float GetDesktopWidth();
float GetDesktopHeight();

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
HWND CreateOpenGLWindow(char* title, int x, int y, int width, int height,
	BYTE type, DWORD flags);
