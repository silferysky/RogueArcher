#define _CRT_SECURE_NO_WARNINGS

#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <sstream>
#include <chrono>
#include <Windows.h>

#include "Main.h"
#include "Logger.h"
#include "VSync.h"	
#include "SOIL.h"
#include "Config.h"
#include "WindowHelper.h"
#include "MemoryManager.h"
#include "GameStateList.h"
#include "Resource.h"
#include "ComponentList.h"
#include "BasicIO.h"
#include "GLHelper.hpp"

#if _DEBUG
#include <iostream>
#endif
#include "REMath.h"
#include "REEditor.h"
#include "Editor.h"

Rogue::REEngine g_engine;
float g_deltaTime;
float g_fixedDeltaTime;
bool EditorMode = false;

//const char* FileName = "/Resources/test.json";
static const int SCREEN_FULLSCREEN = 0;
static const int SCREEN_WIDTH = 960;
static const int SCREEN_HEIGHT = 540;

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

	//if (EditorMode)
	//{
	//	Rogue::Editor Editor;
	//	Editor.init();
	//	Editor.update();
	//	Editor.Shutdown();
	//
	//	return (int)msg.wParam;
	//} 

	g_engine.init();
	
	//BasicIO::WriteLevelJsonFile("Resources/TestJsonFileCreator.json", 1);
	//BasicIO::WriteArchetypeJsonFile("Resources/TestArchetypeJsonFile.json", 3);
	//BasicIO::WriteLevelJsonFile("Resources/Level 1.json", 8);

	// Update engine.
	g_engine.update();

	g_engine.shutdown();

	return 0;

}