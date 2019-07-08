#include <cstdio>
#include <cstdlib>

#include <fstream>
#include <iostream>
#include "Main.h"

double t = 0.0;
double gdt = 0.0;

static const int SCREEN_FULLSCREEN = 0;
static const int SCREEN_WIDTH = 960;
static const int SCREEN_HEIGHT = 540;

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
{
	// Enable run-time memory check for debug builds.
#if defined(DEBUG) | defined(_DEBUG)
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

	GSM_Initialize(GS_TESTLEVEL);

	while (_current != GS_QUIT)
	{
		if (_current != GS_RESTART) // if level is the same as previous
		{
			GSM_Update(); // update game state type
			//fpLoad();     // load all assets required for new game state
		}
		else
		{
			// retains same game state as previous
			_current = _previous;
			_next = _previous;
		}

		//fpInit(); // initialize current game state

		//The game loop
		while (_current == _next)
		{
			//AESysFrameStart();
			//gdt += AEFrameRateControllerGetFrameRate() / 60;

			//If 1/60 of a second has passed
			if (gdt)
			{
				//AEInputUpdate();
				//fpUpdate();
				//fpDraw();

				t += gdt;
				gdt = 0;
			}
			//AESysFrameEnd();
		}

		//fpFree();

		if (_next != GS_RESTART);
			//fpUnload();

		_previous = _current;
		_current = _next;
	}
	//AESysExit();

	return 0;
}

