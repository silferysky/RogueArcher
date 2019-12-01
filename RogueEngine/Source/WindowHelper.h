/* Start Header ************************************************************************/
/*!
\file           WindowHelper.h
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
#pragma once

namespace Rogue
{
	int GetWindowWidth(HWND hWnd);
	int GetWindowHeight(HWND hWnd);

	LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	BOOL WINAPI CtrlHandler(DWORD fdwCtrlType);
}