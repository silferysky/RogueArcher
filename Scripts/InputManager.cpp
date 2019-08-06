#include "InputManager.h"

struct KeyboardState;
enum KeyPress;
enum KeyFunction;

InputManager::InputManager()
{
	CurKeyboardState = new KeyboardState;
	PrevKeyboardState = new KeyboardState;
	GameKeyConfig = new std::map<KeyPress, KeyFunction>;
	MenuKeyConfig = new std::map<KeyPress, KeyFunction>;
	ResetKeyBind();
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

	AddToState();

	//Do keyfunction inputs here
	HandleState();
}

void InputManager::HandleState()
{
	for (int i = 0; i < KeyCount; ++i)
	{
		int tempPos = -1;
		//tempPos = KeyConfig
	}
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
	for (int i = 0; i < KeyCount; ++i)
	{
		toReset->Key[i] = 0;
	}
}

void InputManager::ResetState(FuncState * toReset)
{
	for (int i = 0; i < FuncCount; ++i)
	{
		toReset->Func[i] = 0;
	}
}

void InputManager::AddToState()
{
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
	if (GetAsyncKeyState(VK_ESCAPE))
		++CurKeyboardState->Key[KeyEsc];
	if (GetAsyncKeyState(VK_RETURN))
		++CurKeyboardState->Key[KeyEnter];
	if (GetAsyncKeyState(VK_SHIFT))
		++CurKeyboardState->Key[KeyShift];
	if (GetAsyncKeyState(VK_CONTROL))
		++CurKeyboardState->Key[KeyCtrl];
	if (GetAsyncKeyState(VK_BACK))
		++CurKeyboardState->Key[KeyBackspace];
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

}

void InputManager::DebugKeyInputs()
{
	for (int i = 0; i < KeyCount; ++i)
	{
		int keyCount = 0;
		keyCount += CurKeyboardState->Key[i];
		std::cout << "Key " << (KeyPress)i << ": " << keyCount << std::endl;
	}
}

FuncState* InputManager::getFuncState()
{
	return CurFuncState;
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
	for (int i = 0; i < KeyCount; ++i)
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

void InputManager::ResetKeyBind()
{
	delete GameKeyConfig;
	delete MenuKeyConfig;
	std::map<KeyPress, KeyFunction> GameKeyBinding;
	std::map<KeyPress, KeyFunction> MenuKeyBinding;

	//For Game Key Config Binding
	GameKeyBinding[KeyArrowUp] = MoveUp;
	GameKeyBinding[KeyArrowDown] = MoveDown;
	GameKeyBinding[KeyArrowLeft] = MoveLeft;
	GameKeyBinding[KeyArrowRight] = MoveRight;
	GameKeyBinding[KeyZ] = AttackBasic;
	GameKeyBinding[KeyX] = Jump;
	GameKeyBinding[KeyC] = Teleport;
	GameKeyBinding[KeyEsc] = MenuOpen;

	//For Menu Key Config Binding
	MenuKeyBinding[KeyEsc] = MenuClose;
	MenuKeyBinding[KeyZ] = MenuSelect;
	MenuKeyBinding[KeyEnter] = MenuSelect;
	MenuKeyBinding[KeyX] = MenuBack;
	MenuKeyBinding[KeyBackspace] = MenuBack;

	GameKeyConfig = &GameKeyBinding;
	MenuKeyConfig = &MenuKeyBinding;
}
