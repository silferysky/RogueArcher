#pragma once

#include "Event.h"
#include "InputData.h"

//Base class for all keyboard events
class KeyEvent : public Event
{
public:
	SET_EVENT_CATEGORY(EventCatInput | EventCatKeyboard)

	KeyPress GetKeyCode() { return KeyCode; };

protected:

	//To be used only by creating other KeyEvents using this base class
	KeyEvent(KeyPress key)
		: KeyCode(key) {}

	KeyPress KeyCode;
};

class KeyPressEvent : public KeyEvent
{
public:

	SET_EVENT_TYPE(KeyPressed)

	KeyPressEvent(KeyPress key, int repeatCount)
		: KeyEvent(key), RepeatCount(repeatCount) {}

	inline int GetRepeatCount() const { return RepeatCount; }

	std::string ToString() const override
	{
		std::stringstream ss;
		ss << "KeyPressEvent: " << KeyCode << " with " << RepeatCount << " repeats.";
		return ss.str();
	}

private:
	int RepeatCount;
};

class KeyReleaseEvent : public KeyEvent
{
public:

	SET_EVENT_TYPE(KeyReleased)

	KeyReleaseEvent(KeyPress key)
		: KeyEvent(key) {}

	std::string ToString() const override
	{
		std::stringstream ss;
		ss << "KeyReleasedEvent: " << KeyCode;
		return ss.str();
	}
};

