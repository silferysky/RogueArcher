#pragma once

#include <queue>
#include "EventListener.h"

static class EventDispatcher :
	public BaseSystem
{
public:

	EventDispatcher() { EventQueue = std::queue<Event>(); }
	~EventDispatcher() { }

	Event& GetQueueHead() { return EventQueue.front(); }
	Event& GetQueueHeadDelayed() { return DelayedEventQueue.front(); }

	void AddEvent(Event& e)
	{
		EventQueue.push(e);
	}
	void AddEventDelayed(Event& e)
	{
		DelayedEventQueue.push(e);
	}

	void CombineQueue()
	{
		while (!DelayedEventQueue.empty())
		{
			EventQueue.push(DelayedEventQueue.front());
			DelayedEventQueue.pop();
		}
	}
	void CombineQueueCmd(Event& e)
	{
		if (e.GetEventType() == DelayedEventQueue.front().GetEventType() &&
			e.GetEventName() == DelayedEventQueue.front().GetEventName())
				isCombiningQueue = true;
	}

	void Update()
	{
		if (isCombiningQueue)
		{
			CombineQueue();
			isCombiningQueue = false;
		}

		//While queue is not empty, handle all events
		while (!EventQueue.empty())
		{
			Event& nextEvent = GetQueueHead();
			if (nextEvent.Handled())
			{
				DispatchEvent(nextEvent);
			}
			EventQueue.pop();
		}
	}

	//Dispatch sends it to the relavent system to execute event
	void DispatchEvent(Event& toHandle)
	{
		//Is better to let individual systems handle events than handle all here
		//Basically: Send a message to the relevant system to activate relavent function

		switch (toHandle.GetEventMsgType())
		{
		case MSG_BROADCAST:
			for (std::map<SYSTEMID, BaseSystem*>::iterator it = EventListener::ListenerMap.begin(); it != EventListener::ListenerMap.end(); ++it) 
			{
				//Send to all
			}
			break;
		case MSG_DIRECT:
			switch (toHandle.GetEventType())
			{
			case EvTypeNone:
				break;
			case EvWindowClose:
				exit(0);
				break;
			case EvWindowResize:
				break;
			case EvWindowFocus:
				break;
			case EvWindowLostFocus:
				break;
			case EvWindowMoved:
				break;
			case EvKeyPressed:
				break;
			case EvKeyReleased:
				break;
			case EvMouseButtonPressed:
				break;
			case EvMouseButtonReleased:
				break;
			case EvMouseMoved:
				break;
			case EvMouseScrolled:
				break;
			default:
			}
			break;
		default:
		}
	}

private:
	std::queue<Event> EventQueue;
	std::queue<Event> DelayedEventQueue;
	bool isCombiningQueue = false;

} eventDispatcher;
