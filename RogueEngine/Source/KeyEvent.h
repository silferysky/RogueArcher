/*Start Header*************************************************************************/
/*!
\file           KeyEvent.h
\project        Exale
\author         Chan Wai Kit Terence, c.terence, 440005918 (100%)
\par            c.terence\@digipen.edu
\date           1 December,2019
\brief          This file contains the function definitions for KeyEvent

All content (C) 2019 DigiPen (SINGAPORE) Corporation, all rights
reserved.

Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header **************************************************************************/
#pragma once

#include "Event.h"
#include "InputData.h"

namespace Rogue
{
	//Base class for all keyboard events
	class KeyEvent : public Event
	{
	public:

		virtual KeyPress GetKeyCode() const { return KeyCode; }
		virtual const char* GetKeyName() const { return "None"; }

		//This should not be used
		inline void SetKeyCode(const KeyPress key) { KeyCode = key; }

	protected:

		//To be used only by creating other KeyEvents using this base class
		KeyEvent(KeyPress key)
			: KeyCode(key) {}

		KeyPress KeyCode;
	};

	class KeyPressEvent : public KeyEvent
	{
	public:

		SET_EVENT_TYPE(EvKeyPressed)
		SET_EVENT_CATEGORY(EventCatInput | EventCatKeyboard)

		KeyPressEvent(KeyPress key, int repeatCount = 0)
			: KeyEvent(key), RepeatCount(repeatCount) {}

		inline int GetRepeatCount() const { return RepeatCount; }

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "KeyPressEvent: " << (int)KeyCode << " with " << RepeatCount << " repeats.";
			return ss.str();
		}

	protected:
		int RepeatCount;
	};

	class KeyReleaseEvent : public KeyEvent
	{
	public:

		SET_EVENT_TYPE(EvKeyReleased)
		SET_EVENT_CATEGORY(EventCatInput | EventCatKeyboard)

		KeyReleaseEvent(KeyPress key)
			: KeyEvent(key) {}

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "KeyReleasedEvent: " << (int)KeyCode;
			return ss.str();
		}
	};

	class KeyTriggeredEvent : public KeyEvent
	{
	public:

		SET_EVENT_TYPE(EvKeyTriggered)
		SET_EVENT_CATEGORY(EventCatInput | EventCatKeyboard)

			KeyTriggeredEvent(KeyPress key)
			: KeyEvent(key) {}

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "KeyTriggeredEvent: " << (int)KeyCode;
			return ss.str();
		}
	};

	class KeyPressCombinedEvent : public KeyPressEvent
	{
	public:
		SET_EVENT_TYPE(EvKeyPressed)
		SET_EVENT_CATEGORY(EventCatInput | EventCatKeyboard | EventCatCombinedInput)

		KeyPressCombinedEvent(KeyPress key, KeyPressSub subkey, int repeatCount = 0)
			: KeyPressEvent(key, repeatCount), SubKey{ subkey } { }

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "KeyPressCombinedEvent: " << (int)SubKey << " + " << (int)KeyCode << " with " << RepeatCount << " repeats.";
			return ss.str();
		}

		inline KeyPressSub GetSubKey() const { return SubKey; }

	private:
		KeyPressSub SubKey;
	};

	class KeyTriggeredCombinedEvent : public KeyTriggeredEvent
	{
	public:

		SET_EVENT_TYPE(EvKeyTriggered)
		SET_EVENT_CATEGORY(EventCatInput | EventCatKeyboard | EventCatCombinedInput)

		KeyTriggeredCombinedEvent(KeyPress key, KeyPressSub subkey)
			: KeyTriggeredEvent(key), SubKey{ subkey } { }

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "KeyTriggeredCombinedEvent: " << eventCat << ":" << (int)SubKey << " + " << (int)KeyCode;
			return ss.str();
		}

		inline KeyPressSub GetSubKey() const { return SubKey; }

	private:
		KeyPressSub SubKey;
	};

	class KeyReleasedCombinedEvent : public KeyReleaseEvent
	{
	public:

		SET_EVENT_TYPE(EvKeyReleased)
		SET_EVENT_CATEGORY(EventCatInput | EventCatKeyboard | EventCatCombinedInput)

		KeyReleasedCombinedEvent(KeyPress key, KeyPressSub subkey)
			: KeyReleaseEvent(key), SubKey{ subkey } { }

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "KeyReleaseEvent: " << (int)SubKey << " + " << (int)KeyCode;
			return ss.str();
		}

		inline KeyPressSub GetSubKey() const { return SubKey; }

	private:
		KeyPressSub SubKey;
	};


	class MousePressEvent : public KeyPressEvent
	{
		SET_EVENT_CATEGORY(EventCatMouse | EventCatInput)
		SET_EVENT_TYPE(EvMouseButtonPressed)

			MousePressEvent(KeyPress key, int repeatCount = 0)
			: KeyPressEvent(key, repeatCount) {}

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "MousePressEvent: " << (int)KeyCode << " with " << RepeatCount << " repeats.";
			return ss.str();
		}
	};

	class MouseReleaseEvent : public KeyReleaseEvent
	{
	public:

		SET_EVENT_CATEGORY(EventCatMouse | EventCatInput)
			SET_EVENT_TYPE(EvMouseButtonReleased)

		MouseReleaseEvent(KeyPress key)
			: KeyReleaseEvent(key) {}

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "MouseReleaseEvent: " << (int)KeyCode;
			return ss.str();
		}
	};

	class MouseTriggeredEvent : public KeyTriggeredEvent
	{
	public:

		SET_EVENT_CATEGORY(EventCatMouse | EventCatInput)
			SET_EVENT_TYPE(EvMouseButtonTriggered)

			MouseTriggeredEvent(KeyPress key)
			: KeyTriggeredEvent(key) {}

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "MouseReleaseEvent: " << (int)KeyCode;
			return ss.str();
		}
	};

	class MouseMoveEvent : public KeyEvent
	{
	public:

		SET_EVENT_CATEGORY(EventCatMouse)
		SET_EVENT_TYPE(EvMouseMoved)

		inline float GetX() { return x; }
		inline float GetY() { return y; }

		MouseMoveEvent(float xPos, float yPos, KeyPress key = KeyPress::UNDEF)
			: KeyEvent(key), x(xPos), y(yPos) {}

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "MouseMoveEvent: " << (int)KeyCode << " to (" << x << "," << y << ")";
			return ss.str();
		}

	private:
		float x, y;
	};

	class MouseDoubleClickEvent : public KeyEvent
	{
	public:		
		SET_EVENT_CATEGORY(EventCatMouse)
		SET_EVENT_TYPE(EvMouseDoubleClick)

		MouseDoubleClickEvent(KeyPress key = KeyPress::UNDEF)
			: KeyEvent(key) {}

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "MouseDoubleClickEvent: " << (int)KeyCode << ")";
			return ss.str();
		}
	};
}
