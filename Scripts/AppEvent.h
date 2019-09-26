#pragma once

#include "Event.h"

class WinCloseEvent : public Event
{
public:
	WinCloseEvent(){}
	SET_EVENT_CATEGORY(EventCatApp)
	SET_EVENT_TYPE(WindowClose)
};

class WinResizeEvent : public Event
{
public:
	WinResizeEvent(unsigned int width, unsigned int height)
		: WinWidth(width), WinHeight(height){}

	inline unsigned int GetWidth() { return WinWidth; }
	inline unsigned int GetHeight() { return WinHeight; }

	SET_EVENT_CATEGORY(EventCatApp)
	SET_EVENT_TYPE(WindowResize)

private:
	unsigned int WinWidth, WinHeight;
};

class WinFocusEvent : public Event
{
public:
	WinFocusEvent(){}

	SET_EVENT_CATEGORY(EventCatApp)
	SET_EVENT_TYPE(WindowFocus)
};

class WinLostFocusEvent : public Event
{
public:
	WinLostFocusEvent(){}

	SET_EVENT_CATEGORY(EventCatApp)
	SET_EVENT_TYPE(WindowLostFocus)
};

class WinMovedEvent : public Event
{
public:
	WinMovedEvent(){}

	SET_EVENT_CATEGORY(EventCatApp)
	SET_EVENT_TYPE(WindowMoved)
};

class AppTickEvent : public Event
{
	AppTickEvent() {}

	SET_EVENT_CATEGORY(EventCatApp)
	SET_EVENT_TYPE(AppTick)
};

class AppUpdateEvent : public Event
{
	AppUpdateEvent() {}

	SET_EVENT_CATEGORY(EventCatApp)
	SET_EVENT_TYPE(AppUpdate)
};

class AppRenderEvent : public Event
{
	AppRenderEvent() {}

	SET_EVENT_CATEGORY(EventCatApp)
	SET_EVENT_TYPE(AppRender)
};