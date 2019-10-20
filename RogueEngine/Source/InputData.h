#pragma once

namespace Rogue
{
	enum class KeyPress
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
		KeyArrowLeft,
		KeyArrowUp,
		KeyArrowRight,
		KeyArrowDown,
		KeyEsc,
		KeyBackspace,
		KeyTab,
		KeyEnter,
		KeyShift,
		KeyCtrl,
		KeySpace,
		MB1,
		MB2,
		MB3, //Scrollwheel
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
}