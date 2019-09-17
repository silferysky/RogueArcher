#pragma once

#include <queue>
#include "Event.h"

static class EventDispatcher :
	public BaseSystem
{

public:

	EventDispatcher() { EventQueue = new std::queue<Event>; }
	~EventDispatcher() { delete EventQueue; }

	Event& GetQueueHead() { return EventQueue->front(); }

	void AddEvent(Event& e)
	{
		EventQueue->push(e);
	}

	void Update()
	{
		//While queue is not empty, handle all events
		while (!eventDispatcher.EventQueue->empty())
		{
			Event& nextEvent = GetQueueHead();
			if (nextEvent.Handled())
			{
				DispatchEvent(nextEvent);
			}
			EventQueue->pop();
		}
	}

	//Dispatch sends it to the relavent system to execute event
	void DispatchEvent(Event& toHandle)
	{
		//Is better to let individual systems handle events than handle all here
		//Basically: Send a message to the relevant system to activate relavent function

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
			break;
		}
	}

private:
	std::queue<Event> *EventQueue;
} eventDispatcher;
