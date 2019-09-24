#pragma once

#include "RogueEngine.h"
#include "GameStateList.h"
#include "GameStateManager.h"
#include <Windows.h>
#include "Resource.h"
#include "SystemManager.h"

#if _DEBUG
#include <iostream>
#endif

//#defines
extern double t;
extern double gdt;
extern SystemManager *SysManager;

int APIENTRY WinMain(HINSTANCE hCurrentInst, HINSTANCE hPreviousInst, LPSTR lpszCmdLine, int nCmdShow);
