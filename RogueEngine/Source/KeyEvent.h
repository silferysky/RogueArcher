#pragma once

#include "Event.h"
#include "InputData.h"

//Base class for all keyboard events
class KeyEvent : public Event
{
public:
	SET_EVENT_CATEGORY(EventCatInput | EventCatKeyboard)

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

	KeyTriggeredEvent(KeyPress key)
		: KeyEvent(key) {}

	std::string ToString() const override
	{
		std::stringstream ss;
		ss << "KeyTriggeredEvent: " << (int)KeyCode;
		return ss.str();
	}
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