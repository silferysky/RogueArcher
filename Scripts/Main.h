#pragma once

#include "../REEngine.h"
#include "GameStateList.h"
#include <Windows.h>
#include "Resource.h"

#if _DEBUG
#include <iostream>
#endif

//#defines

// Forward Declaration
class REEngine;

//Externs
extern REEngine gEngine;
extern float gDeltaTime;

int APIENTRY WinMain(HINSTANCE hCurrentInst, HINSTANCE hPreviousInst, LPSTR lpszCmdLine, int nCmdShow);
