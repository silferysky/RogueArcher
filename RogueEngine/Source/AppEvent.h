/*Start Header*************************************************************************/
/*!
\file           AppEvent.h
\project        Exale
\author         Chan Wai Kit Terence, c.terence, 440005918 (100%)
\par            c.terence\@digipen.edu
\date           1 December,2019
\brief          This file contains the function definitions for AppEvent

All content (C) 2019 DigiPen (SINGAPORE) Corporation, all rights
reserved.

Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header **************************************************************************/
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
			: WinWig_DeltaTimeh(width), WinHeight(height) {}

		inline unsigned int GetWig_DeltaTimeh() { return WinWig_DeltaTimeh; }
		inline unsigned int GetHeight() { return WinHeight; }

		SET_EVENT_CATEGORY(EventCatApp)
			SET_EVENT_TYPE(EvWindowResize)

	private:
		unsigned int WinWig_DeltaTimeh, WinHeight;
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