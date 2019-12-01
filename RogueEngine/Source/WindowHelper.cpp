/* Start Header ************************************************************************/
/*!
\file           WindowHelper.cpp
\project        Exale
\author         Javier Foo, javier.foo, 440002318 (100%)
\par            javier.foo\@digipen.edu
\date           1 December,2019
\brief          This file contains the function definitions for WindowHelper

All content (C) 2019 DigiPen (SINGAPORE) Corporation, all rights
reserved.

Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header **************************************************************************/
#include "Precompiled.h"
#include "WindowHelper.h"
#include "wtypes.h"
#include "Main.h"
#include "GLHelper.hpp"
#include "EditorManager.h"
#include "imgui_impl_win32.h"

namespace Rogue
{
	int GetWindowWidth(HWND hWnd)
	{
		RECT rect;
		GetWindowRect(hWnd, &rect);
		return rect.right - rect.left;
	}

	int GetWindowHeight(HWND hWnd)
	{
		RECT rect;
		GetWindowRect(hWnd, &rect);
		return rect.bottom - rect.top;
	}

	LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		IMGUI_IMPL_API LRESULT  ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
		if (ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam))
		{
			return true;
		}
		
		static PAINTSTRUCT ps;
		switch (uMsg)
		{
		case WM_PAINT:
			BeginPaint(hWnd, &ps);
			EndPaint(hWnd, &ps);
			return 0;
		case WM_SIZE:
			glViewport(0, 0, GetWindowWidth(hWnd), GetWindowHeight(hWnd));
			//g_engine.m_coordinator.GetSystem<GraphicsSystem>()->UpdateFrameBuffer();
			PostMessage(hWnd, WM_PAINT, 0, 0);
			return 0;
		case WM_DESTROY:
			PostQuitMessage(0);
			return 0;
		case WM_KEYDOWN:
			//F11
			if (wParam == VK_F11)
			{
				if (GetWindowLongPtr(hWnd, GWL_STYLE) & WS_POPUP)
				{
					SetWindowLongPtr(hWnd, GWL_STYLE, WS_VISIBLE | WS_OVERLAPPEDWINDOW);
					SetWindowPos(hWnd, NULL, 0, 0, 1024, 720, SWP_FRAMECHANGED);
				}
				else
				{ // set full screen
					SetWindowLongPtr(hWnd, GWL_STYLE, WS_VISIBLE | WS_POPUP);
					SetWindowPos(hWnd, HWND_TOP, 0, 0, GetSystemMetrics(SM_CXSCREEN),
						GetSystemMetrics(SM_CYSCREEN), SWP_FRAMECHANGED);
				}
			}
			return 0;
		case WM_CLOSE:
			g_engine.SetGameIsRunning(false);
			return 0;
		case WM_ACTIVATE:
			if (wParam == WA_INACTIVE)
			{
				g_engine.SetIsFocused(false);
				g_engine.SetTimeScale(0.0f);
			}
			else if (wParam == WA_ACTIVE || wParam == WA_CLICKACTIVE)
			{
				g_engine.SetIsFocused(true);
				g_engine.SetTimeScale(1.0f);
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
			g_engine.SetGameIsRunning(false);
			printf("Ctrl-Close event\n\n");
			return FALSE;

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
}
