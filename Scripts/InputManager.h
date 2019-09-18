#pragma once
#include <Windows.h>
#include <map>
#include "BaseSystem.h"
#include "EventDispatcher.h"
#include "KeyEvent.h"
#include "InputData.h"
#include <iostream>

struct KeyboardState
{
	//Since COUNT is the last KeyPress, it will always be same value as size of KeyPress
	int Key[KeyCount] = { 0 };

	//overloaded += operator. Checks if rhs has value. If rhs has value, add it, otherwise reset to 0
	void operator+=(KeyboardState &rhs)
	{
		for (int i = 0; i < KeyCount; ++i)
		{
			if (rhs.Key[i] != 0)
				Key[i] += rhs.Key[i];
			else
				Key[i] = 0;
		}
	}
};

struct FuncState
{
	int Func[FuncCount] = { 0 };
};

class InputManager
	: public BaseSystem
{
public:
	InputManager();
	~InputManager();

	//State management

	//Updates current and previous keyboard state
	void UpdateState();
	//Handles the current keyboard state
	void HandleState();
	//Hard reset keyboard states
	void RemakeState();
	//Reset given state to be all null values
	void ResetState(KeyboardState *toReset);
	void ResetState(FuncState *toReset);
	//Add to current state only, not update it
	void AddToState();
	//To display the current keyboard state in debug
	void DebugKeyInputs();
	void DebugKeyInputs(KeyPress key);
	//Functions to check buttons, applies to other controllers
	FuncState* getFuncState();

	//Key Related

	//Checks if key is not pressed
	bool KeyUp(KeyPress checkKey);
	//Checks if key is down
	bool KeyDown(KeyPress checkKey);
	bool KeyDownAny();
	//Checks if key is just pressed
	bool KeyTriggered(KeyPress checkKey);
	bool KeyTriggeredAny();

	//Reset Key bindings
	void ResetKeyBind();

private:
	KeyboardState CurKeyboardState;
	KeyboardState PrevKeyboardState;
	std::map<KeyPress, KeyFunction> GameKeyConfig;
	std::map<KeyPress, KeyFunction> MenuKeyConfig;
	FuncState CurFuncState;
	int ButtonTrigger;
};