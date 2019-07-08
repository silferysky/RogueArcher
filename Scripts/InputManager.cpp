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

	/*HWND hwnd;
	bool msgChecked = false;
	MSG msg;

	while (!msgChecked)
	{
		//Other checks are here too
		msgChecked = true;

		while (PeekMessage(&msg, hwnd, 0, 0, PM_REMOVE))
		{
			switch (msg.message)
			{
			case MK_LBUTTON:
				break;
			case MK_RBUTTON:
				break;
			case MK_MBUTTON:
				break;
			}
		}
	}*/

	if (GetKeyState('A'))
		++CurKeyboardState->Key[KeyA];
	if (GetKeyState('B'))
		++CurKeyboardState->Key[KeyB];
	if (GetKeyState('C'))
		++CurKeyboardState->Key[KeyC];
	if (GetKeyState('D'))
		++CurKeyboardState->Key[KeyD];
	if (GetKeyState('E'))
		++CurKeyboardState->Key[KeyE];
	if (GetKeyState('F'))
		++CurKeyboardState->Key[KeyF];
	if (GetKeyState('G'))
		++CurKeyboardState->Key[KeyG];
	if (GetKeyState('H'))
		++CurKeyboardState->Key[KeyH];
	if (GetKeyState('I'))
		++CurKeyboardState->Key[KeyI];
	if (GetKeyState('J'))
		++CurKeyboardState->Key[KeyJ];
	if (GetKeyState('K'))
		++CurKeyboardState->Key[KeyK];
	if (GetKeyState('L'))
		++CurKeyboardState->Key[KeyL];
	if (GetKeyState('M'))
		++CurKeyboardState->Key[KeyM];
	if (GetKeyState('N'))
		++CurKeyboardState->Key[KeyN];
	if (GetKeyState('O'))
		++CurKeyboardState->Key[KeyO];
	if (GetKeyState('P'))
		++CurKeyboardState->Key[KeyP];
	if (GetKeyState('Q'))
		++CurKeyboardState->Key[KeyQ];
	if (GetKeyState('R'))
		++CurKeyboardState->Key[KeyR];
	if (GetKeyState('S'))
		++CurKeyboardState->Key[KeyS];
	if (GetKeyState('T'))
		++CurKeyboardState->Key[KeyT];
	if (GetKeyState('U'))
		++CurKeyboardState->Key[KeyU];
	if (GetKeyState('V'))
		++CurKeyboardState->Key[KeyV];
	if (GetKeyState('W'))
		++CurKeyboardState->Key[KeyW];
	if (GetKeyState('X'))
		++CurKeyboardState->Key[KeyX];
	if (GetKeyState('Y'))
		++CurKeyboardState->Key[KeyY];
	if (GetKeyState('Z'))
		++CurKeyboardState->Key[KeyZ];
	if (GetKeyState('0'))
		++CurKeyboardState->Key[Key0];
	if (GetKeyState('1'))
		++CurKeyboardState->Key[Key1];
	if (GetKeyState('2'))
		++CurKeyboardState->Key[Key2];
	if (GetKeyState('3'))
		++CurKeyboardState->Key[Key3];
	if (GetKeyState('4'))
		++CurKeyboardState->Key[Key4];
	if (GetKeyState('5'))
		++CurKeyboardState->Key[Key5];
	if (GetKeyState('6'))
		++CurKeyboardState->Key[Key6];
	if (GetKeyState('7'))
		++CurKeyboardState->Key[Key7];
	if (GetKeyState('8'))
		++CurKeyboardState->Key[Key8];
	if (GetKeyState('9'))
		++CurKeyboardState->Key[Key9];
	if (GetKeyState(VK_LBUTTON))
		++CurKeyboardState->Key[MB1];
	if (GetKeyState(VK_RBUTTON))
		++CurKeyboardState->Key[MB2];
	if (GetKeyState(VK_MBUTTON))
		++CurKeyboardState->Key[MB3];
	if (GetKeyState(VK_UP))
		++CurKeyboardState->Key[KeyArrowUp];
	if (GetKeyState(VK_DOWN))
		++CurKeyboardState->Key[KeyArrowDown];
	if (GetKeyState(VK_LEFT))
		++CurKeyboardState->Key[KeyArrowLeft];
	if (GetKeyState(VK_RIGHT))
		++CurKeyboardState->Key[KeyArrowRight];
	
	if (_DEBUG)
		for (int i = 0; i < UNDEF; ++i)
		{

		}
	
	//Do keyfunction inputs here
	
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