#include <cstdio>
#include <cstdlib>

#include <fstream>
#include "Main.h"
#include "KeyEvent.h"
#include "InputManager.h"
#include "Library.h"

double t = 0.0;
double gdt = 1.0;
//SystemManager *SysManager = new SystemManager();

static const int SCREEN_FULLSCREEN = 0;
static const int SCREEN_WIDTH = 960;
static const int SCREEN_HEIGHT = 540;

//Use for console
int main()
{
	//Logger
	Logger::InitLogger();
	RE_CORE_TRACE("Init Core Logger");

	InputManager* InputMgr = new InputManager();
	MemoryManager memManager;
	RE_INFO("Hello");

	//Main Debug
	int repeat = 0;
	float timer = 0.0f;
 	while (repeat < 5)
	{
		InputMgr->UpdateState();

		if (InputMgr->KeyTriggeredAny())
		{
			++repeat;
			//InputMgr->DebugKeyInputs();
		}
	}

	//InputMger->DebugKeyInputs();
	RE_INFO("PAUSE HERE FOR END");
	RE_ERROR("THIS IS A ERROR MESSAGE BUT NO ERROR");

	RE_INFO("TESTING HERE FOR A RANDOM EVENT");
	KeyPressEvent testEvent((KeyPress)KeyArrowRight, 10);
	RE_INFO(testEvent.ToString());
	RE_INFO("END EVENT TEST");


	RE_INFO("TESTING HERE FOR FILE IO");

	Library testLibrary;
	testLibrary.IOTest();

	RE_INFO("END TEST IO");
	
	RE_INFO("TEST ReMM (Rogue Engine Memory Manager)");
	MemoryManager ReMM;

	System s;
	System s2;
	Entity e;
	Component c;

	std::shared_ptr<System> ptr = std::make_shared<System>(s);

	ReMM.AddIntoMemory(s);
	ReMM.AddIntoMemory(s2);
	ReMM.AddIntoMemory(e);
	ReMM.AddIntoMemory(c);

	std::string str("System Count: ");
	str.append(std::to_string(ReMM.SysListCount()));
	RE_INFO(str);

	str.clear();
	str.append("Use Count: ");
	str.append(std::to_string(ReMM.GetSysList()[0].use_count()));

	RE_INFO(str);

	std::cin.get();
	delete InputMgr;
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
				//InputMger->UpdateState();
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

