#pragma once

#include <queue>
#include "Event.h"

static class EventDispatcher :
	public System
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
		//Safety check
		if (eventDispatcher.EventQueue->empty())
			return;

		//Queue is not empty here onwards
		Event& nextEvent = GetQueueHead();
		if (nextEvent.Handled())
		{
			DispatchEvent(nextEvent);
		}
		EventQueue->pop();
	}

	//Dispatch sends it to the relavent system to execute event
	void DispatchEvent(Event& toHandle)
	{
		//Is better to let individual systems handle events than handle all here
		//Basically: Send a message to the relevant system to activate relavent function

		switch (toHandle.GetEventType())
		{
		case EventTypeNone:
			break;
		case WindowClose:
			exit(0);
			break;
		case WindowResize:
			break;
		case WindowFocus:
			break;
		case WindowLostFocus:
			break;
		case WindowMoved:
			break;
		case KeyPressed:
			break;
		case KeyReleased:
			break;
		case MouseButtonPressed:
			break;
		case MouseButtonReleaseed:
			break;
		case MouseMoved:
			break;
		case MouseScrolled:
			break;
		default:
			break;
		}
	}

private:
	std::queue<Event> *EventQueue;
} eventDispatcher;
