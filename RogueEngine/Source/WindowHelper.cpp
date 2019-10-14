#include <Windows.h>
#include "WindowHelper.h"
#include "wtypes.h"
#include "Main.h"
#include "GLHelper.hpp"

float GetDesktopWidth()
{
	RECT desktop;
	const HWND hDesktop = GetDesktopWindow();
	// Get the size of screen to the desktop
	GetWindowRect(hDesktop, &desktop);
	return float(desktop.right);
}

float GetDesktopHeight()
{
	RECT desktop;
	const HWND hDesktop = GetDesktopWindow();
	// Get the size of screen to the desktop
	GetWindowRect(hDesktop, &desktop);
	return float(desktop.bottom);
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	static PAINTSTRUCT ps;
	switch (uMsg)
	{
	case WM_PAINT:
		BeginPaint(hWnd, &ps);
		EndPaint(hWnd, &ps);
		return 0;
	case WM_SIZE:
		glViewport(0, 0, LOWORD(lParam), HIWORD(lParam));
		PostMessage(hWnd, WM_PAINT, 0, 0);
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	case WM_KEYDOWN:
		if (wParam == 'F')
		{
			if (GetWindowLongPtr(hWnd, GWL_STYLE) & WS_POPUP)
			{
				SetWindowLongPtr(hWnd, GWL_STYLE, WS_VISIBLE | WS_OVERLAPPEDWINDOW);
				SetWindowPos(hWnd, NULL, 0, 0, 600, 400, SWP_FRAMECHANGED);
			}
			else
			{ // set full screen
				SetWindowLongPtr(hWnd, GWL_STYLE, WS_VISIBLE | WS_POPUP);
				SetWindowPos(hWnd, HWND_TOP, 0, 0, GetSystemMetrics(SM_CXSCREEN), 
					GetSystemMetrics(SM_CYSCREEN), SWP_FRAMECHANGED);
			}
		}
		else if (wParam == 'P')
		{
			gEngine.m_coordinator.togglePerformanceChecker();
		}
		else if (wParam == 'O')
		{
			gEngine.m_coordinator.toggleFPSChecker();
		}
	}
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

HWND CreateOpenGLWindow(char* title, int x, int y, int width, int height,
	BYTE type, DWORD flags)
{
	int         pf;
	HDC         hDC;
	HWND        hWnd;
	WNDCLASS    wc;
	PIXELFORMATDESCRIPTOR pfd{ 0 };
	static HINSTANCE hInstance = 0;

	/* only register the window class once - use hInstance as a flag. */
	if (!hInstance) {
		hInstance = GetModuleHandle(NULL);
		wc.style = CS_OWNDC;
		wc.lpfnWndProc = (WNDPROC)WndProc;
		wc.cbClsExtra = 0;
		wc.cbWndExtra = 0;
		wc.hInstance = hInstance;
		wc.hIcon = LoadIcon(NULL, IDI_WINLOGO);
		wc.hCursor = LoadCursor(NULL, IDC_ARROW);
		wc.hbrBackground = NULL;
		wc.lpszMenuName = NULL;
		wc.lpszClassName = "OpenGL";

		if (!RegisterClass(&wc)) {
			MessageBox(NULL, "RegisterClass() failed:  "
				"Cannot register window class.", "Error", MB_OK);
			return NULL;
		}
	}

	hWnd = CreateWindow("OpenGL", title, WS_OVERLAPPEDWINDOW |
		WS_CLIPSIBLINGS | WS_CLIPCHILDREN,
		x, y, width, height, NULL, NULL, hInstance, NULL);

	if (hWnd == NULL) {
		MessageBox(NULL, "CreateWindow() failed:  Cannot create a window.",
			"Error", MB_OK);
		return NULL;
	}

	hDC = GetDC(hWnd);

	memset(&pfd, 0, sizeof(pfd));
	pfd.nSize = sizeof(pfd);
	pfd.nVersion = 1;
	pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | flags | PFD_GENERIC_ACCELERATED | PFD_DOUBLEBUFFER;
	pfd.iPixelType = type;
	pfd.cColorBits = 32;

	pf = ChoosePixelFormat(hDC, &pfd);
	if (pf == 0) {
		MessageBox(NULL, "ChoosePixelFormat() failed:  "
			"Cannot find a suitable pixel format.", "Error", MB_OK);
		return 0;
	}

	if (SetPixelFormat(hDC, pf, &pfd) == FALSE) {
		MessageBox(NULL, "SetPixelFormat() failed:  "
			"Cannot set format specified.", "Error", MB_OK);
		return 0;
	}

	DescribePixelFormat(hDC, pf, sizeof(PIXELFORMATDESCRIPTOR), &pfd);

	//ReleaseDC(hWnd, hDC);

	return hWnd;
}

BOOL WINAPI CtrlHandler(DWORD fdwCtrlType)
{
	switch (fdwCtrlType)
	{
		// Handle the CTRL-C signal. 
	case CTRL_C_EVENT:
		printf("Ctrl-C event\n\n");
		return TRUE;

		// CTRL-CLOSE: confirm that the user wants to exit. 
	case CTRL_CLOSE_EVENT:
		printf("Ctrl-Close event\n\n");
		return TRUE;

		// Pass other signals to the next handler. 
	case CTRL_BREAK_EVENT:
		printf("Ctrl-Break event\n\n");
		return FALSE;

	case CTRL_LOGOFF_EVENT:
		printf("Ctrl-Logoff event\n\n");
		return FALSE;

	case CTRL_SHUTDOWN_EVENT:
		printf("Ctrl-Shutdown event\n\n");
		return FALSE;

	default:
		return FALSE;
	}
}