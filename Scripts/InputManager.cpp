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

	if (GetAsyncKeyState('A'))
		++CurKeyboardState->Key[KeyA];
	if (GetAsyncKeyState('B'))
		++CurKeyboardState->Key[KeyB];
	if (GetAsyncKeyState('C'))
		++CurKeyboardState->Key[KeyC];
	if (GetAsyncKeyState('D'))
		++CurKeyboardState->Key[KeyD];
	if (GetAsyncKeyState('E'))
		++CurKeyboardState->Key[KeyE];
	if (GetAsyncKeyState('F'))
		++CurKeyboardState->Key[KeyF];
	if (GetAsyncKeyState('G'))
		++CurKeyboardState->Key[KeyG];
	if (GetAsyncKeyState('H'))
		++CurKeyboardState->Key[KeyH];
	if (GetAsyncKeyState('I'))
		++CurKeyboardState->Key[KeyI];
	if (GetAsyncKeyState('J'))
		++CurKeyboardState->Key[KeyJ];
	if (GetAsyncKeyState('K'))
		++CurKeyboardState->Key[KeyK];
	if (GetAsyncKeyState('L'))
		++CurKeyboardState->Key[KeyL];
	if (GetAsyncKeyState('M'))
		++CurKeyboardState->Key[KeyM];
	if (GetAsyncKeyState('N'))
		++CurKeyboardState->Key[KeyN];
	if (GetAsyncKeyState('O'))
		++CurKeyboardState->Key[KeyO];
	if (GetAsyncKeyState('P'))
		++CurKeyboardState->Key[KeyP];
	if (GetAsyncKeyState('Q'))
		++CurKeyboardState->Key[KeyQ];
	if (GetAsyncKeyState('R'))
		++CurKeyboardState->Key[KeyR];
	if (GetAsyncKeyState('S'))
		++CurKeyboardState->Key[KeyS];
	if (GetAsyncKeyState('T'))
		++CurKeyboardState->Key[KeyT];
	if (GetAsyncKeyState('U'))
		++CurKeyboardState->Key[KeyU];
	if (GetAsyncKeyState('V'))
		++CurKeyboardState->Key[KeyV];
	if (GetAsyncKeyState('W'))
		++CurKeyboardState->Key[KeyW];
	if (GetAsyncKeyState('X'))
		++CurKeyboardState->Key[KeyX];
	if (GetAsyncKeyState('Y'))
		++CurKeyboardState->Key[KeyY];
	if (GetAsyncKeyState('Z'))
		++CurKeyboardState->Key[KeyZ];
	if (GetAsyncKeyState('0'))
		++CurKeyboardState->Key[Key0];
	if (GetAsyncKeyState('1'))
		++CurKeyboardState->Key[Key1];
	if (GetAsyncKeyState('2'))
		++CurKeyboardState->Key[Key2];
	if (GetAsyncKeyState('3'))
		++CurKeyboardState->Key[Key3];
	if (GetAsyncKeyState('4'))
		++CurKeyboardState->Key[Key4];
	if (GetAsyncKeyState('5'))
		++CurKeyboardState->Key[Key5];
	if (GetAsyncKeyState('6'))
		++CurKeyboardState->Key[Key6];
	if (GetAsyncKeyState('7'))
		++CurKeyboardState->Key[Key7];
	if (GetAsyncKeyState('8'))
		++CurKeyboardState->Key[Key8];
	if (GetAsyncKeyState('9'))
		++CurKeyboardState->Key[Key9];
	if (GetAsyncKeyState(VK_LBUTTON))
		++CurKeyboardState->Key[MB1];
	if (GetAsyncKeyState(VK_RBUTTON))
		++CurKeyboardState->Key[MB2];
	if (GetAsyncKeyState(VK_MBUTTON))
		++CurKeyboardState->Key[MB3];
	if (GetAsyncKeyState(VK_UP))
		++CurKeyboardState->Key[KeyArrowUp];
	if (GetAsyncKeyState(VK_DOWN))
		++CurKeyboardState->Key[KeyArrowDown];
	if (GetAsyncKeyState(VK_LEFT))
		++CurKeyboardState->Key[KeyArrowLeft];
	if (GetAsyncKeyState(VK_RIGHT))
		++CurKeyboardState->Key[KeyArrowRight];

	if (_DEBUG)
		for (int i = 0; i < Count; ++i)
		{
			int keyCount = 0;
			keyCount += CurKeyboardState->Key[i];
			std::cout << "Key " << i << ": " << keyCount << std::endl;
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
		toReset->Key[i] = 0;
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