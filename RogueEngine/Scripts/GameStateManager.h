#include "GameStateList.h"
#pragma once

//typedef void(*FPU)(double gDeltaTime);

typedef void(*FP)(void);

extern int _current, _previous, _next;                         // current game state status

extern FP fpLoad, fpInit, fpUpdate, fpDraw, fpFree, fpUnload;  // function pointers

//extern FPU fpUpdate;
// ----------------------------------------------------------------------------
// This function sets up the game state manager
// It should be called once after system initialization
// It starts up a game state upon initialization
// ----------------------------------------------------------------------------
void GSM_Initialize(int startingState);

// ----------------------------------------------------------------------------
// This function updates the game states of the game state manager
// It should be called for as long as the game runs
// It checks and updates function pointers accordingly
// ----------------------------------------------------------------------------
void GSM_Update();