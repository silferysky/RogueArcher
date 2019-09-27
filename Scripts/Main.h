#pragma once

#include "../REEngine.h"
#include "GameStateList.h"
#include "GameStateManager.h"
#include <Windows.h>
#include "Resource.h"
#include "SystemManager.h"

//Temp for test
#include "ComponentList.h"

#if _DEBUG
#include <iostream>
#endif

//#defines
extern double t;
extern double gdt;
extern REEngine gEngine;

int APIENTRY WinMain(HINSTANCE hCurrentInst, HINSTANCE hPreviousInst, LPSTR lpszCmdLine, int nCmdShow);
