#pragma once

namespace Rogue
{
	int GetWindowWidth(HWND hWnd);
	int GetWindowHeight(HWND hWnd);

	LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	BOOL WINAPI CtrlHandler(DWORD fdwCtrlType);
}