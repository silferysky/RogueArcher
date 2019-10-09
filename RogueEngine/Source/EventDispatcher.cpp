#include "EventDispatcher.h"

void EventDispatcher::init()
{
	EventQueue = std::queue<Event*>();
	DelayedEventQueue = std::queue<Event*>();
	ListenerMap = std::map<SystemID, LISTENER_HANDLER>();
}

Event* EventDispatcher::GetQueueHead() { return EventQueue.front(); }

Event* EventDispatcher::GetQueueHeadDelayed() { return DelayedEventQueue.front(); }

void EventDispatcher::CombineQueue()
{
	while (!DelayedEventQueue.empty())
	{
		EventQueue.push(DelayedEventQueue.front());
		DelayedEventQueue.pop();
	}
}

void EventDispatcher::CombineQueueCmd(Event& e)
{
	if (e.GetEventType() == DelayedEventQueue.front()->GetEventType() &&
		e.GetEventName() == DelayedEventQueue.front()->GetEventName())
		isCombiningQueue = true;
}

void EventDispatcher::AddListener(SystemID ID, LISTENER_HANDLER handler)
{
	ListenerMap.insert(std::pair<SystemID, LISTENER_HANDLER>(ID, handler));
	RE_CORE_INFO("Added new key to ListenerMap");
}

void EventDispatcher::RemoveAllListener()
{
	ListenerMap.clear();
}

void EventDispatcher::RemoveListener(SystemID ID)
{
	for (auto it = ListenerMap.begin(); it != ListenerMap.end(); ++it)
	{
		if (it->first == ID)
		{
			ListenerMap.erase(it);
		}
	}
}

std::map<SystemID, LISTENER_HANDLER> EventDispatcher::GetMap()
{
	return ListenerMap;
}

void EventDispatcher::AddEvent(Event* e)
{
	EventQueue.push(e);
}

void EventDispatcher::AddEventDelayed(Event* e)
{
	DelayedEventQueue.push(e);
}

void EventDispatcher::update()
{
	if (isCombiningQueue)
	{
		CombineQueue();
		isCombiningQueue = false;
	}

	//While queue is not empty, handle all events
	while (!EventQueue.empty())
	{
		Event* nextEvent = GetQueueHead();
		DispatchEvent(nextEvent);
		EventQueue.pop();
		delete nextEvent;
	}
}

void EventDispatcher::DispatchEvent(Event* toHandle)
{
	//Is better to let individual systems handle events than handle all here
	//Basically: Send a message to the relevant system to activate relavent function

	auto sysIt = ListenerMap.begin();
	while (sysIt != ListenerMap.end())
	{
		sysIt->second(toHandle);
		++sysIt;
	}
}