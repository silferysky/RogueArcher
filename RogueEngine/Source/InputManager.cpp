#include "Precompiled.h"
#include "InputManager.h"
#include "InputData.h"
#include "BaseSystem.h"
#include "Windows.h"
#include "EventDispatcher.h"
#include "Timer.h"
#include "Main.h"

namespace Rogue
{
	struct KeyboardState;
	enum class KeyPress;
	enum KeyFunction;

	void KeyboardState::operator+=(KeyboardState& rhs)
	{
		for (int i = 0; i < (int)KeyPress::KeyCount; ++i)
		{
			if (rhs.Key[i] != 0)
				Key[i] += rhs.Key[i];
			else
				Key[i] = 0;
		}
	}

	InputManager::InputManager()
		:System(SystemID::id_INPUTMANAGER)
	{
		//Init();
	}

	void InputManager::Init()
	{
		CurKeyboardState = KeyboardState();
		PrevKeyboardState = KeyboardState();
		GameKeyConfig = std::map<KeyPress, KeyFunction>();
		MenuKeyConfig = std::map<KeyPress, KeyFunction>();
		ResetKeyBind();

		ResetState(&CurKeyboardState);
		ResetState(&PrevKeyboardState);

		Signature signature;
		g_engine.m_coordinator.SetSystemSignature<InputManager>(signature);
	}

	void InputManager::Update()
	{
		Timer TimerSystem;
		TimerSystem.TimerInit("Input System");
		//Always do this first
		//Shallow add old keyboardstate details over to keep track of how long button is pressed
		PrevKeyboardState += CurKeyboardState;

		//Ensure state is empty
		ResetState(&CurKeyboardState);
		//Detecting inputs
		AddToState();


		//Do keyfunction inputs here
		HandleState();
		TimerSystem.TimerEnd("Input System");

		//Always do this last
	}

	void InputManager::HandleState()
	{
		for (int i = 0; i < (int)KeyPress::KeyCount; ++i)
		{
			if (KeyTriggered(static_cast<KeyPress>(i)))
				CreateKeyTriggeredEvent(static_cast<KeyPress>(i));
			else if (CurKeyboardState.Key[i] > 0)
				CreateKeyPressEvent((KeyPress(i)), CurKeyboardState.Key[i]);
			else if (KeyReleased((KeyPress)i))
				CreateKeyReleaseEvent((KeyPress)i);
		}
	}

	void InputManager::RemakeState()
	{
		CurKeyboardState = KeyboardState();
		PrevKeyboardState = KeyboardState();
	}

	void InputManager::ResetState(KeyboardState* toReset)
	{
		for (int i = 0; i < (int)KeyPress::KeyCount; ++i)
		{
			toReset->Key[i] = 0;
		}
	}

	void InputManager::ResetState(FuncState* toReset)
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
		for (int i = (int)KeyPress::KeyA; i < (int)KeyPress::KeyZ + 1; ++i)
		{
			if (GetAsyncKeyState('A' + i - (int)KeyPress::KeyA))
				++CurKeyboardState.Key[i];
		}

		//Handles 0 to 9
		for (int i = (int)KeyPress::Key0; i < (int)KeyPress::Key9 + 1; ++i)
		{
			if (GetAsyncKeyState('0' + i - (int)KeyPress::Key0))
				++CurKeyboardState.Key[i];
		}

		//Handles Arrow Keys
		for (int i = (int)KeyPress::KeyArrowLeft; i < (int)KeyPress::KeyArrowDown + 1; ++i)
		{
			if (GetAsyncKeyState(VK_LEFT + i - (int)KeyPress::KeyArrowLeft))
				++CurKeyboardState.Key[i];
		}

		//Handles Numpad 0 to 9
		for (int i = (int)KeyPress::Numpad0; i < (int)KeyPress::Numpad9 + 1; ++i)
		{
			if (GetAsyncKeyState(VK_NUMPAD0 + i - (int)KeyPress::Numpad0))
				++CurKeyboardState.Key[i];
		}

		//Handles F1 to F12
		for (int i = (int)KeyPress::KeyF1; i < (int)KeyPress::KeyF12; ++i)
		{
			if (GetAsyncKeyState(VK_F1 + i - (int)KeyPress::KeyF1))
				++CurKeyboardState.Key[i];
		}

		//For Mouse Buttons (Scrollwheel skips one)
		if (GetAsyncKeyState(VK_LBUTTON))
			++CurKeyboardState.Key[(int)KeyPress::MB1];
		if (GetAsyncKeyState(VK_RBUTTON))
			++CurKeyboardState.Key[(int)KeyPress::MB2];
		if (GetAsyncKeyState(VK_MBUTTON))
			++CurKeyboardState.Key[(int)KeyPress::MB3];

		//FOr Misc Key Inputs
		if (GetAsyncKeyState(VK_ESCAPE))
			++CurKeyboardState.Key[(int)KeyPress::KeyEsc];
		if (GetAsyncKeyState(VK_BACK))
			++CurKeyboardState.Key[(int)KeyPress::KeyBackspace];
		if (GetAsyncKeyState(VK_TAB))
			++CurKeyboardState.Key[(int)KeyPress::KeyTab];
		if (GetAsyncKeyState(VK_RETURN))
			++CurKeyboardState.Key[(int)KeyPress::KeyEnter];
		if (GetAsyncKeyState(VK_SHIFT))
			++CurKeyboardState.Key[(int)KeyPress::KeyShift];
		if (GetAsyncKeyState(VK_CONTROL))
			++CurKeyboardState.Key[(int)KeyPress::KeyCtrl];
		if (GetAsyncKeyState(VK_SPACE))
			++CurKeyboardState.Key[(int)KeyPress::KeySpace];

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
		for (int i = 0; i < (int)KeyPress::KeyCount; ++i)
			DebugKeyInputs((KeyPress)i);
	}

	void InputManager::DebugKeyInputs(KeyPress key)
	{
		int keyCount = PrevKeyboardState.Key[(int)key] + CurKeyboardState.Key[(int)key];
		std::cout << "Key " << (int)key << ": " << keyCount << std::endl;
	}

	FuncState* InputManager::getFuncState()
	{
		return &CurFuncState;
	}

	bool InputManager::KeyUp(KeyPress checkKey)
	{
		if (CurKeyboardState.Key[(int)checkKey] == false)
			return true;
		return false;
	}

	bool InputManager::KeyDown(KeyPress checkKey)
	{
		if (CurKeyboardState.Key[(int)checkKey] > 0)
			return true;
		return false;
	}

	bool InputManager::KeyDownAny()
	{
		for (int i = 0; i < (int)KeyPress::KeyCount; ++i)
		{
			if (KeyDown((KeyPress)i))
				return true;
		}
		return false;
	}

	bool InputManager::KeyTriggered(KeyPress checkKey)
	{
		if (KeyDown(checkKey))
			if (PrevKeyboardState.Key[(int)checkKey] == 0)
				return true;
		return false;
	}

	bool InputManager::KeyTriggeredAny()
	{
		for (int i = 0; i < (int)KeyPress::KeyCount; ++i)
		{
			if (KeyTriggered((KeyPress)i))
				return true;
		}
		return false;
	}

	bool InputManager::KeyReleased(KeyPress checkKey)
	{
		if (PrevKeyboardState.Key[(int)checkKey] > 0)
			if (KeyUp(checkKey))
				return true;
		return false;
	}

	void InputManager::ResetKeyBind()
	{
		std::map<KeyPress, KeyFunction> GameKeyBinding;
		std::map<KeyPress, KeyFunction> MenuKeyBinding;

		//For Game Key Config Binding
		GameKeyBinding[KeyPress::KeyArrowUp] = MoveUp;
		GameKeyBinding[KeyPress::KeyArrowDown] = MoveDown;
		GameKeyBinding[KeyPress::KeyArrowLeft] = MoveLeft;
		GameKeyBinding[KeyPress::KeyArrowRight] = MoveRight;
		GameKeyBinding[KeyPress::KeyZ] = AttackBasic;
		GameKeyBinding[KeyPress::KeyX] = Jump;
		GameKeyBinding[KeyPress::KeyC] = Teleport;
		GameKeyBinding[KeyPress::KeyEsc] = MenuOpen;

		//For Menu Key Config Binding
		MenuKeyBinding[KeyPress::KeyEsc] = MenuClose;
		MenuKeyBinding[KeyPress::KeyZ] = MenuSelect;
		MenuKeyBinding[KeyPress::KeyEnter] = MenuSelect;
		MenuKeyBinding[KeyPress::KeyX] = MenuBack;
		MenuKeyBinding[KeyPress::KeyBackspace] = MenuBack;

		GameKeyConfig = GameKeyBinding;
		MenuKeyConfig = MenuKeyBinding;
	}

	void InputManager::CreateKeyPressEvent(KeyPress key, int repeat)
	{
		KeyPressEvent* event = new KeyPressEvent(key, repeat);
		event->SetSystemReceivers((int)SystemID::id_PHYSICSSYSTEM);
		event->SetSystemReceivers((int)SystemID::id_PLAYERCONTROLLERSYSTEM);
		event->SetSystemReceivers((int)SystemID::id_CAMERASYSTEM);
		EventDispatcher::instance().AddEvent(event);
	}

	void InputManager::CreateKeyReleaseEvent(KeyPress key)
	{
		KeyReleaseEvent* event = new KeyReleaseEvent(key);
		event->SetSystemReceivers((int)SystemID::id_PHYSICSSYSTEM);
		event->SetSystemReceivers((int)SystemID::id_PLAYERCONTROLLERSYSTEM);
		EventDispatcher::instance().AddEvent(event);
	}

	void InputManager::CreateKeyTriggeredEvent(KeyPress key)
	{
		KeyTriggeredEvent* event = new KeyTriggeredEvent(key);
		event->SetSystemReceivers((int)SystemID::id_PHYSICSSYSTEM);
		event->SetSystemReceivers((int)SystemID::id_PLAYERCONTROLLERSYSTEM);
		event->SetSystemReceivers((int)SystemID::id_CAMERASYSTEM);
		event->SetSystemReceivers((int)SystemID::id_AUDIOSYSTEM);
		event->SetSystemReceivers((int)SystemID::id_GRAPHICSSYSTEM);
		event->SetSystemReceivers((int)SystemID::id_DEBUGDRAWSYSTEM);
		event->SetSystemReceivers((int)SystemID::id_EDITOR);
		EventDispatcher::instance().AddEvent(event);
	}

	void InputManager::Shutdown()
	{}
}