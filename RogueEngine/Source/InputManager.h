#pragma once
#include "KeyEvent.h"
#include "SystemManager.h"
#include <map>

namespace Rogue
{
	struct KeyboardState
	{
		//Since COUNT is the last KeyPress, it will always be same value as size of KeyPress
		int Key[(int)KeyPress::KeyCount] = { 0 };

		//overloaded += operator. Checks if rhs has value. If rhs has value, add it, otherwise reset to 0
		void operator+=(KeyboardState& rhs);
	};

	struct KeyboardStateSpecial
	{
		//Since COUNT is the last KeyPress, it will always be same value as size of KeyPress
		int Key[(int)KeyPressSpecial::KeyCount] = { 0 };

		//overloaded += operator. Checks if rhs has value. If rhs has value, add it, otherwise reset to 0
		void operator+=(KeyboardStateSpecial& rhs);
	};

	struct FuncState
	{
		int Func[FuncCount] = { 0 };
	};

	class InputManager
		: public System
	{
	public:
		InputManager();
		~InputManager() = default;

		//Initialization
		void Init();

		//State management

		//Updates current and previous keyboard state
		void Update();
		//Handles the current keyboard state
		void HandleState();
		//Hard reset keyboard states
		void RemakeState();
		//Reset given state to be all null values
		void ResetState(KeyboardState* toReset);
		void ResetState(KeyboardStateSpecial* toReset);
		void ResetState(FuncState* toReset);
		//Add to current state only, not update it
		void AddToState();
		//To display the current keyboard state in debug
		void DebugKeyInputs();
		void DebugKeyInputs(KeyPress key);
		//Functions to check buttons, applies to other controllers
		FuncState* getFuncState();
		bool CheckForSpecialKeys();

		//Key Related

		//For KeyPress
		//Checks if key is not pressed
		bool KeyUp(KeyPress checkKey);
		//Checks if key is down
		bool KeyDown(KeyPress checkKey);
		bool KeyDownAny();
		//Checks if key is just pressed
		bool KeyTriggered(KeyPress checkKey);
		bool KeyTriggeredAny();
		//Checks if key is just released
		bool KeyReleased(KeyPress checkKey);

		//Reset Key bindings
		void ResetKeyBind();

		//Creating Events

		void CreateKeyPressEvent(KeyPress key, int repeat = 0, KeyPressSpecial subkey = KeyPressSpecial::UNDEF);
		void CreateKeyReleaseEvent(KeyPress key, KeyPressSpecial subkey = KeyPressSpecial::UNDEF);
		void CreateKeyTriggeredEvent(KeyPress key, KeyPressSpecial subkey = KeyPressSpecial::UNDEF);

		// Shutdown
		void Shutdown() override;

	private:
		KeyboardState CurKeyboardState;
		KeyboardState PrevKeyboardState;

		//For special keys
		KeyboardStateSpecial CurKeyboardStateSpecial;
		KeyboardStateSpecial PrevKeyboardStateSpecial;
		
		std::map<KeyPress, KeyFunction> GameKeyConfig;
		std::map<KeyPress, KeyFunction> MenuKeyConfig;
		FuncState CurFuncState;
	};
}