#include "GameStateManager.h"
#include "GameStateList.h"
#include <iostream>
#include <fstream>

int _current = 0, _previous = 0, _next = 0;  // current game state status

FP fpLoad = 0, fpInit = 0, fpUpdate = 0, fpDraw = 0, fpFree = 0, fpUnload = 0; // function pointers

//extern FPU fpUpdate = 0;
// ----------------------------------------------------------------------------
// This function sets up the game state manager
// It should be called once after system initialization
// It starts up a game state upon initialization
// ----------------------------------------------------------------------------
void GSM_Initialize(int startingState)
{
	_current = _previous = _next = startingState;
}

// ----------------------------------------------------------------------------
// This function updates the game states of the game state manager
// It should be called for as long as the game runs
// It checks and updates function pointers accordingly
// ----------------------------------------------------------------------------
void GSM_Update()
{
	switch (_current)
	{
	case GS_TESTLEVEL:   // if test level is selected
		/*fpLoad = TestLevel_Load;
		fpInit = TestLevel_Init;
		fpUpdate = TestLevel_Update;
		fpDraw = TestLevel_Draw;
		fpFree = TestLevel_Free;
		fpUnload = TestLevel_Unload;*/
		break;
	case GS_QUIT:    // if quit is selected
		_current = GS_QUIT;
		break;
	case GS_RESTART: // if level has been restarted
		_previous = _current;
		_current = _next;
		break;
	default:
#if _DEBUG
		std::cout << "INVALID STATE SELECTED, PLEASE RESTART" << std::endl;
#endif
		break;
	}
}