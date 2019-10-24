#pragma once

#include "Event.h"

namespace Rogue
{
	class WinCloseEvent : public Event
	{
	public:
		WinCloseEvent() {}
		SET_EVENT_CATEGORY(EventCatApp)
			SET_EVENT_TYPE(EvWindowClose)
	};

	class WinResizeEvent : public Event
	{
	public:
		WinResizeEvent(unsigned int width, unsigned int height)
			: WinWigDeltaTimeh(width), WinHeight(height) {}

		inline unsigned int GetWigDeltaTimeh() { return WinWigDeltaTimeh; }
		inline unsigned int GetHeight() { return WinHeight; }

		SET_EVENT_CATEGORY(EventCatApp)
			SET_EVENT_TYPE(EvWindowResize)

	private:
		unsigned int WinWigDeltaTimeh, WinHeight;
	};

	class WinFocusEvent : public Event
	{
	public:
		WinFocusEvent() {}

		SET_EVENT_CATEGORY(EventCatApp)
			SET_EVENT_TYPE(EvWindowFocus)
	};

	class WinLostFocusEvent : public Event
	{
	public:
		WinLostFocusEvent() {}

		SET_EVENT_CATEGORY(EventCatApp)
			SET_EVENT_TYPE(EvWindowLostFocus)
	};

	class WinMovedEvent : public Event
	{
	public:
		WinMovedEvent() {}

		SET_EVENT_CATEGORY(EventCatApp)
			SET_EVENT_TYPE(EvWindowMoved)
	};

	class EvAppTickEvent : public Event
	{
		EvAppTickEvent() {}

		SET_EVENT_CATEGORY(EventCatApp)
			SET_EVENT_TYPE(EvAppTick)
	};

	class EvAppUpdateEvent : public Event
	{
		EvAppUpdateEvent() {}

		SET_EVENT_CATEGORY(EventCatApp)
			SET_EVENT_TYPE(EvAppUpdate)
	};

	class EvAppRenderEvent : public Event
	{
		EvAppRenderEvent() {}

		SET_EVENT_CATEGORY(EventCatApp)
			SET_EVENT_TYPE(EvAppRender)
	};
}