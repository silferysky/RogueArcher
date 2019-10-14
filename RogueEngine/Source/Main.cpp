#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include "Main.h"
#include "TestSystem.h"
#include <chrono>
#include "VSync.h"
#include "SOIL.h"
#include "Config.h"
#include "WindowHelper.h"
#include "MemoryManager.h"
#include "REMath.h"


REEngine gEngine;
float gDeltaTime;
bool gameIsRunning = true;
ObjectFactory gObjectFactory;

//const char* FileName = "/Resources/test.json";
static const int SCREEN_FULLSCREEN = 0;
static const int SCREEN_WIDTH = 960;
static const int SCREEN_HEIGHT = 540;


//Use for console
int APIENTRY
WinMain(HINSTANCE hCurrentInst, HINSTANCE hPreviousInst,
	LPSTR lpszCmdLine, int nCmdShow)
{
	UNREFERENCED_PARAMETER(hPreviousInst);
	UNREFERENCED_PARAMETER(lpszCmdLine);
	UNREFERENCED_PARAMETER(hCurrentInst);
	
	// Enable run-time memory check for debug builds.
	#if defined(DEBUG) | defined(_DEBUG)
	    _CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
	#endif
	RE_INFO("Logging App info succeeded");

	gEngine.init();
	
	//////////////////////////
	//Graphics Debug
	/////////////////////////

	std::cout << glGetString(GL_VERSION) << std::endl;

	//---------------------------------------------------------------//
	//	Generate entities (Can't use object factory in coordinator)	 //
	//---------------------------------------------------------------//
	RE_CORE_INFO("Generating entities...");

	RE_INFO("TEST FILEWRITER");
	BasicIO::WriteJsonFile("Resources/TestJsonFileCreator.json", 1);

	RE_INFO("TEST OBJECT FACTORY");
	std::stringstream debugStr;
	size_t objInLevel = gObjectFactory.GetActiveEntity().size();
	debugStr << "Number of entities at start: " << objInLevel;

	//BasicIO::WriteJsonFile("Resources/Level 1.json", 8);

	RE_INFO(debugStr.str());
	gObjectFactory.LoadLevel("Resources/Level 1.json");

	debugStr.clear();
	debugStr.str("");

	int* value1 = (int*)MemoryManager::instance().operator new(8);
	int* value2 = (int*)MemoryManager::instance().operator new(8);
	MemoryManager::instance().operator delete(value1);
	MemoryManager::instance().operator delete(value2);
	//int* value3 = (int*)MemoryManager::instance().operator new(8);
	//*value1 = 5;

	//gObjectFactory.SaveLevel("Resources/Level 1.json");

	while (gameIsRunning)
	{
		//Main Debug
		// RE_INFO("INPUT DEBUG");

		// Update engine.
		gEngine.update();

		//		void Engine::Update()
		//		{
		//			currentNumberOfSteps = 0; //reset
		//			deltaTime = timeEnd - timeStart; // Compute the actual game loop time
		//			while (accumulatedTime >= gFixedDeltaTime)
		//			{
		//				accumulatedTime -= gFixedDeltaTime;
		//			}
		//			currentNumberOfSteps++;
		//		}
		//		Physics::Update()
		//		{
		//			// Loop used in systems that have time-based formula
		//			for(int step = 0; step < currentNumberOfSteps; ++step
		//				{
		//					// Do euler's stuff
		//				}
		//		}
	}

	std::cin.get();

	return 0;
}

