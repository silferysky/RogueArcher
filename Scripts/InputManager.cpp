#include "InputManager.h"

struct KeyboardState;
enum KeyPress;
enum KeyFunction;

InputManager::InputManager()
{
	CurKeyboardState = new KeyboardState;
	PrevKeyboardState = new KeyboardState;
}


InputManager::~InputManager()
{
	delete CurKeyboardState;
	delete PrevKeyboardState;
}

void InputManager::UpdateState()
{
	//Always do this first
	*PrevKeyboardState = *CurKeyboardState;
	ResetState(CurKeyboardState);

	//Creates/Update the current state
	//Reads in and puts the input in

	//MSG msg;
	//while (GetMessage())
}

void InputManager::HandleState()
{
	//TODO Movement/Action/Menu code
}

void InputManager::RemakeState()
{
	delete CurKeyboardState;
	delete PrevKeyboardState;
	CurKeyboardState = new KeyboardState;
	PrevKeyboardState = new KeyboardState;
}

void InputManager::ResetState(KeyboardState *toReset)
{
	for (int i = 0; i < Count; ++i)
	{
		toReset->Key[i] = UNDEF;
	}
}

bool InputManager::KeyUp(KeyPress checkKey)
{
	if (CurKeyboardState->Key[checkKey] == false)
		return true;
	return false;
}

bool InputManager::KeyDown(KeyPress checkKey)
{
	if (CurKeyboardState->Key[checkKey] > 0)
		return true;
	return false;
}

bool InputManager::KeyDownAny()
{
	for (int i = 0; i < Count; ++i)
	{
		if (CurKeyboardState->Key[i] > 0)
			return true;
	}
	return false;
}

bool InputManager::KeyTriggered(KeyPress checkKey)
{
	if (CurKeyboardState->Key[checkKey] > 0 && PrevKeyboardState->Key[checkKey] == 0)
		return true;
	return false;
}