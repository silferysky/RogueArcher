#include <cstdio>
#include <cstdlib>

#include <fstream>
#include <iostream>
#include "Main.h"
#include "InputManager.h"

double t = 0.0;
double gdt = 1.0;
//SystemManager *SysManager = new SystemManager();
InputManager *InputMger = new InputManager();

static const int SCREEN_FULLSCREEN = 0;
static const int SCREEN_WIDTH = 960;
static const int SCREEN_HEIGHT = 540;

//Use for console
int main()
{
	int repeat = 0;
	float timer = 0.0f;
 	while (repeat < 100)
	{
		timer += 0.0000000001f;
		if (timer)
		{
			std::cout << "Count " << repeat << std::endl;
			InputMger->UpdateState();
			timer = 0.0f;
			++repeat;
		}
	}
	InputMger->DebugKeyInputs();
	std::cin.get();
	delete InputMger;
	return 0;
}

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
{
	if (_DEBUG)
	{
		std::cout << "DEBUG STATE ACTIVATE" << std::endl;
	}

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
				InputMger->UpdateState();
				//fpUpdate();
				//fpDraw();

				//t += gdt;
				//gdt = 0;
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
	//delete SysManager;

	std::cin.get();

	return 0;
}

