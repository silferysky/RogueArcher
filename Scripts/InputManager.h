#pragma once
#include <Windows.h>
#include <map>
#include "System.h"
#include <iostream>

enum KeyPress
{
	Key0 = 0,
	Key1,
	Key2,
	Key3,
	Key4,
	Key5,
	Key6,
	Key7,
	Key8,
	Key9,
	KeyA,
	KeyB,
	KeyC,
	KeyD,
	KeyE,
	KeyF,
	KeyG,
	KeyH,
	KeyI,
	KeyJ,
	KeyK,
	KeyL,
	KeyM,
	KeyN,
	KeyO,
	KeyP,
	KeyQ,
	KeyR,
	KeyS,
	KeyT,
	KeyU,
	KeyV,
	KeyW,
	KeyX,
	KeyY,
	KeyZ,
	KeyArrowUp,
	KeyArrowDown,
	KeyArrowLeft,
	KeyArrowRight,
	KeyShift,
	KeyCtrl,
	KeyEnter,
	KeyEsc,
	KeyBackspace,
	MB1,
	MB2,
	MB3, //Scrollwheel
	Spare0,
	Spare1,
	Spare2,
	Spare3,
	Spare4,
	Spare5,
	Spare6,
	Spare7,
	Spare8,
	Spare9,
	KeyCount, //Represents number of buttons
	UNDEF //Represents no button pressed
};

enum KeyFunction
{
	//This stores the default actions
	MoveUp = 0,
	MoveDown,
	MoveLeft,
	MoveRight,
	Jump,
	AttackBasic,
	Teleport,

	//For menu stuff
	MenuOpen,
	MenuSelect,
	MenuBack,
	MenuClose,

	//Misc
	FuncCount
};

struct KeyboardState
{
	//Since COUNT is the last KeyPress, it will always be same value as size of KeyPress
	int Key[KeyCount] = { 0 };
};

struct FuncState
{
	int Func[FuncCount] = { 0 };
};

class InputManager:
	public System
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
	//Functions to check buttons, applies to other controllers
	FuncState* getFuncState();

	//Key Related

	//Checks if key is not pressed
	bool KeyUp(KeyPress checkKey);
	//Checks if key is down
	bool KeyDown(KeyPress checkKey);
	//Checks if any key is down
	bool KeyDownAny();
	//Checks if key is just pressed
	bool KeyTriggered(KeyPress checkKey);

	//Reset Key bindings
	void ResetKeyBind();

private:
	KeyboardState *CurKeyboardState;
	KeyboardState *PrevKeyboardState;
	std::map<KeyPress, KeyFunction> *GameKeyConfig;
	std::map<KeyPress, KeyFunction> *MenuKeyConfig;
	FuncState *CurFuncState;
};