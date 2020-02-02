/* Start Header ************************************************************************/
/*!
\file           InputData.h
\project        Exale
\author         Chan Wai Kit Terence, c.terence, 440005918 (100%)
\par            c.terence\@digipen.edu
\date           1 December,2019
\brief          This file contains the function definitions for InputData

All content (C) 2019 DigiPen (SINGAPORE) Corporation, all rights
reserved.

Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header **************************************************************************/
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
		KeyDelete,
		KeyTab,
		KeyCtrl,
		KeyShift,
		KeyAlt,
		KeyEnter,
		KeySpace,
		Numpad0,
		Numpad1,
		Numpad2,
		Numpad3,
		Numpad4,
		Numpad5,
		Numpad6,
		Numpad7,
		Numpad8,
		Numpad9,
		KeyF1,
		KeyF2,
		KeyF3,
		KeyF4,
		KeyF5,
		KeyF6,
		KeyF7,
		KeyF8,
		KeyF9,
		KeyF10,
		KeyF11,
		KeyF12,
		MB1,
		MB2,
		MB3, //Scrollwheel
		KeyCount, //Represents number of buttons
		UNDEF //Represents no button pressed
	};

	enum class KeyPressSub
	{
		KeyCtrl = 0,
		KeyShift,
		KeyAlt,
		KeyCount,
		UNDEF
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