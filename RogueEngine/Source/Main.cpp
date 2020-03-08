/* Start Header ************************************************************************/
/*!
\file           Main.cpp
\project        Exale
\author         Loh Kai Yi,kaiyi.loh,390002918 (25%)
				Lim Jia Hao, l.jiahao, 390006518 (25%)
				Chan Wai Kit Terence, c.terence, 440005918 (25%)
				Javier Foo, javier.foo, 440002318 (25%)
\par            kaiyi.loh\@digipen.edu
				l.jiahao\@digipen.edu
				c.terence\@digipen.edu
				javier.foo\@digipen.edu
\date           1 December,2019
\brief          This file contains the main loop of the engine

All content (C) 2019 DigiPen (SINGAPORE) Corporation, all rights
reserved.

Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header **************************************************************************/
#include "Precompiled.h"

Rogue::REEngine g_engine;
float g_deltaTime;
float g_fixedDeltaTime;

//Use for console
int APIENTRY

WinMain(HINSTANCE hCurrentInst, HINSTANCE hPreviousInst,
	LPSTR lpszCmdLine, int nCmdShow)
{
	// Enable run-time memory check for debug builds.
#if defined(DEBUG) | defined(_DEBUG)
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

	UNREFERENCED_PARAMETER(hPreviousInst);
	UNREFERENCED_PARAMETER(lpszCmdLine);
	UNREFERENCED_PARAMETER(hCurrentInst);

	g_engine.Init();
	
	g_engine.Update();

	g_engine.Shutdown();

	return 0;

}