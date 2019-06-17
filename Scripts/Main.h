#pragma once
#include "GameStateList.h"
#include "GameStateManager.h"
#include <Windows.h>
#include "Resource.h"
#include "SystemManager.h"

#if _DEBUG
#include <iostream>
#endif

//#defines

#define WINAPI      __stdcall
#define APIENTRY    WINAPI

extern double t;
extern double gdt;
extern SystemManager *SysManager;
