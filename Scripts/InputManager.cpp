#include "InputManager.h"

struct KeyboardState;
enum KeyPress;
enum KeyFunction;

InputManager::InputManager()
{
	CurKeyboardState = KeyboardState();
	PrevKeyboardState = KeyboardState();
	GameKeyConfig = std::map<KeyPress, KeyFunction>();
	MenuKeyConfig = std::map<KeyPress, KeyFunction>();
	ResetKeyBind();

	ResetState(&CurKeyboardState);
	ResetState(&PrevKeyboardState);
}

InputManager::~InputManager()
{
}

void InputManager::UpdateState()
{
	//Always do this first
	//Shallow add old keyboardstate details over to keep track of how long button is pressed
	PrevKeyboardState += CurKeyboardState;

	//Ensure state is empty
	ResetState(&CurKeyboardState);
	//Detecting inputs
	AddToState();


	//Do keyfunction inputs here
	HandleState();


	//Always do this last
}

void InputManager::HandleState()
{
	for (int i = 0; i < KeyCount; ++i)
	{
		if (CurKeyboardState.Key[i] > 0)
			CreateKeyPressEvent((KeyPress(i)), CurKeyboardState.Key[i]);
		if (KeyReleased((KeyPress)i))
			CreateKeyReleaseEvent((KeyPress)i);
	}
}

void InputManager::RemakeState()
{
	CurKeyboardState = KeyboardState();
	PrevKeyboardState = KeyboardState();
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
	
	//Handles A to Z
	for (int i = KeyA; i < KeyZ + 1; ++i)
	{
		if (GetAsyncKeyState('A' + i - KeyA))
			++CurKeyboardState.Key[i];
	}

	//Handles 0 to 9
	for (int i = Key0; i < Key9 + 1; ++i)
	{
		if (GetAsyncKeyState('0' + i - Key0))
			++CurKeyboardState.Key[i];
	}

	//Handles Arrow Keys
	for (int i = KeyArrowLeft; i < KeyArrowDown + 1; ++i)
	{
		if (GetAsyncKeyState(VK_LEFT + i - KeyArrowLeft))
			++CurKeyboardState.Key[i];
	}

	//For Mouse Buttons (Scrollwheel skips one)
	if (GetAsyncKeyState(VK_LBUTTON))
		++CurKeyboardState.Key[MB1];
	if (GetAsyncKeyState(VK_RBUTTON))
		++CurKeyboardState.Key[MB2];
	if (GetAsyncKeyState(VK_MBUTTON))
		++CurKeyboardState.Key[MB3];

	//FOr Misc Key Inputs
	if (GetAsyncKeyState(VK_ESCAPE))
		++CurKeyboardState.Key[KeyEsc];
	if (GetAsyncKeyState(VK_BACK))
		++CurKeyboardState.Key[KeyBackspace];
	if (GetAsyncKeyState(VK_TAB))
		++CurKeyboardState.Key[KeyTab];
	if (GetAsyncKeyState(VK_RETURN))
		++CurKeyboardState.Key[KeyEnter];
	if (GetAsyncKeyState(VK_SHIFT))
		++CurKeyboardState.Key[KeyShift];
	if (GetAsyncKeyState(VK_CONTROL))
		++CurKeyboardState.Key[KeyCtrl];
	if (GetAsyncKeyState(VK_SPACE))
		++CurKeyboardState.Key[KeySpace];

	/*for (int i = 0; i < KeyCount; ++i)
	{
		if (CurKeyboardState.Key[i] != 0)
		{
			//Dispatching KeyEvent input
			KeyPressEvent e((KeyPress)i, PrevKeyboardState.Key[i]);
			eventDispatcher.AddEvent(e);

			//Debug only
			//DebugKeyInputs((KeyPress)i);
		}
	}*/
}

void InputManager::DebugKeyInputs()
{
	for (int i = 0; i < KeyCount; ++i)
		DebugKeyInputs((KeyPress)i);
}

void InputManager::DebugKeyInputs(KeyPress key)
{
	int keyCount = PrevKeyboardState.Key[key] + CurKeyboardState.Key[key];
	std::cout << "Key " << key << ": " << keyCount << std::endl;
}

FuncState* InputManager::getFuncState()
{
	return &CurFuncState;
}

bool InputManager::KeyUp(KeyPress checkKey)
{
	if (CurKeyboardState.Key[checkKey] == false)
		return true;
	return false;
}

bool InputManager::KeyDown(KeyPress checkKey)
{
	if (CurKeyboardState.Key[checkKey] > 0)
		return true;
	return false;
}

bool InputManager::KeyDownAny()
{
	for (int i = 0; i < KeyCount; ++i)
	{
		if (KeyDown((KeyPress)i))
			return true;
	}
	return false;
}

bool InputManager::KeyTriggered(KeyPress checkKey)
{
	if (KeyDown(checkKey))
		if (PrevKeyboardState.Key[checkKey] == 0)
			return true;
	return false;
}

bool InputManager::KeyTriggeredAny()
{
	for (int i = 0; i < KeyCount; ++i)
	{
		if (KeyTriggered((KeyPress)i))
			return true;
	}
	return false;
}

bool InputManager::KeyReleased(KeyPress checkKey)
{
	if (PrevKeyboardState.Key[checkKey] > 0)
		if (KeyUp(checkKey))
			return true;
	return false;
}

void InputManager::ResetKeyBind()
{
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

	GameKeyConfig = GameKeyBinding;
	MenuKeyConfig = MenuKeyBinding;
}

void InputManager::CreateKeyPressEvent(KeyPress key, int repeat)
{
	KeyPressEvent* event = new KeyPressEvent(key, repeat);
	EventDispatcher::instance().AddEvent(event);
}

void InputManager::CreateKeyReleaseEvent(KeyPress key)
{
	KeyPressEvent* event = new KeyPressEvent(key);
	EventDispatcher::instance().AddEvent(event);
}