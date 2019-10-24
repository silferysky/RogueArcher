#pragma once

namespace Rogue
{
	float GetWindowWidth(HWND hWnd);
	float GetWindowHeight(HWND hWnd);

	LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	BOOL WINAPI CtrlHandler(DWORD fdwCtrlType);
}