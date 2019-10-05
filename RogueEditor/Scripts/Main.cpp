#include <cstdio>
#include <cstdlib>

#include <fstream>
#include "Main.h"
#include "KeyEvent.h"
#include "InputManager.h"
#include "MemoryManager.h"
#include "FileIO.h"
#include "Quad.h"
#include "SOIL.h"
#include "imgui.h"
#include "imgui_impl_opengl3.h"
#include "imgui_impl_glfw.h"
#include "ImguiLayer.h"
GLuint texture[1];

double t = 0.0;
double gdt = 1.0;
//SystemManager *SysManager = new SystemManager();

static const int SCREEN_FULLSCREEN = 0;
static const int SCREEN_WIDTH = 960;
static const int SCREEN_HEIGHT = 540;

//Use for console
int main()
{
	std::ifstream ifs{ "Resources/test.json" };
	if (!ifs.is_open())
	{
		std::cerr << "Could not open file for reading!\n";
		return EXIT_FAILURE;
	}

	rapidjson::IStreamWrapper Wrapper{ ifs };

	rapidjson::Document doc{};
	doc.ParseStream(Wrapper);
	if (doc.HasParseError())
	{
		std::cout << "Error  : " << doc.GetParseError() << '\n'
			<< "Offset : " << doc.GetErrorOffset() << '\n';
		return EXIT_FAILURE;
	}
	int hi;
	hi = doc["Position Y"].GetInt();
	std::cout << "Position Y: " << hi << std::endl;
	//WriteToFile("Resources/test.json", "angle z", 324.67f);
	ImGuiLayer::ImguiLayer Editor;
	Editor.StartWindow();
	Editor.UpdateWindow();
	Editor.CloseWindow();
	

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

		if (_next != GS_RESTART)
		{
			//fpUnload();
		}

		_previous = _current;
		_current = _next;
	}
	//AESysExit();
	//delete SysManager;

	std::cin.get();

	return 0;
}

