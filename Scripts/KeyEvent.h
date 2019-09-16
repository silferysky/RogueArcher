#pragma once

#include "Event.h"
#include "InputData.h"

//Base class for all keyboard events
class KeyEvent : public Event
{
public:
	SET_EVENT_CATEGORY(EventCatInput | EventCatKeyboard)

	inline KeyPress GetKeyCode() { return KeyCode; };
	inline void SetKeyCode(KeyPress key) { KeyCode = key; }

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

	KeyPressEvent(KeyPress key, int repeatCount = 0)
		: KeyEvent(key), RepeatCount(repeatCount) {}

	inline int GetRepeatCount() const { return RepeatCount; }

	std::string ToString() const override
	{
		std::stringstream ss;
		ss << "KeyPressEvent: " << KeyCode << " with " << RepeatCount << " repeats.";
		return ss.str();
	}

protected:
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

class MousePressEvent : public KeyPressEvent
{
	SET_EVENT_CATEGORY(EventCatMouse | EventCatInput)
	SET_EVENT_TYPE(MouseButtonPressed)

	MousePressEvent(KeyPress key, int repeatCount = 0)
		: KeyPressEvent(key, repeatCount) {}

	std::string ToString() const override
	{
		std::stringstream ss;
		ss << "MousePressEvent: " << KeyCode << " with " << RepeatCount << " repeats.";
		return ss.str();
	}
};

class MouseReleaseEvent : public KeyReleaseEvent
{
public:

	SET_EVENT_CATEGORY(EventCatMouse | EventCatInput)
	SET_EVENT_TYPE(MouseButtonReleaseed)

	MouseReleaseEvent(KeyPress key)
		: KeyReleaseEvent(key) {}

	std::string ToString() const override
	{
		std::stringstream ss;
		ss << "MouseReleaseEvent: " << KeyCode;
		return ss.str();
	}
};

class MouseMoveEvent : public KeyEvent
{
public:

	SET_EVENT_CATEGORY(EventCatMouse)
	SET_EVENT_TYPE(MouseMoved)

	inline float GetX() { return x; }
	inline float GetY() { return y; }

	MouseMoveEvent(float xPos, float yPos, KeyPress key = UNDEF)
		: KeyEvent(key), x(xPos), y(yPos) {}

	std::string ToString() const override
	{
		std::stringstream ss;
		ss << "MouseMoveEvent: " << KeyCode << " to (" << x << "," << y << ")";
		return ss.str();
	}

private:
	float x, y;
};