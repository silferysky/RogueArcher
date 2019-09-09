#pragma once

#include "RogueEngine.h"
#include "GameStateList.h"
#include "GameStateManager.h"
#include <Windows.h>
#include "Resource.h"

#if _DEBUG
#include <iostream>
#endif

//#defines

#define WINAPI      __stdcall
#define APIENTRY    WINAPI

extern double t;
extern double gdt;

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow);
