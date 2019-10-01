#pragma once

#include <queue>
#include <map>
#include <vector>
#include "EventListener.h"
#include "SystemManager.h"

class EventDispatcher :
	public System
{
public:

	EventDispatcher() = default;
	~EventDispatcher() = default;

	void init()
	{
		//instance();
		EventQueue = std::queue<Event*>();
		DelayedEventQueue = std::queue<Event*>();
		ListenerMap = std::map<SystemID, LISTENER_HANDLER>();
	}

	static EventDispatcher& instance()
	{
		static EventDispatcher instance;
		return instance;
	}

	Event* GetQueueHead() { return EventQueue.front(); }
	Event* GetQueueHeadDelayed() { return DelayedEventQueue.front(); }
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
		if (e.GetEventType() == DelayedEventQueue.front()->GetEventType() &&
			e.GetEventName() == DelayedEventQueue.front()->GetEventName())
			isCombiningQueue = true;
	}

	void AddListener(SystemID ID, LISTENER_HANDLER handler)
	{
		ListenerMap.insert(std::pair<SystemID, LISTENER_HANDLER>(ID, handler));
		RE_CORE_INFO("Added new key to ListenerMap");
	}
	//Removes all listeners in map
	void RemoveAllListener()
	{
		ListenerMap.clear();
	}
	void RemoveListener(SystemID ID)
	{
		for (auto it = ListenerMap.begin(); it != ListenerMap.end(); ++it)
		{
			if (it->first == ID)
			{
				ListenerMap.erase(it);
			}
		}
	}

	std::map<SystemID, LISTENER_HANDLER> GetMap()
	{
		return ListenerMap;
	}

	void AddEvent(Event* e)
	{
		EventQueue.push(e);
	}
	void AddEventDelayed(Event* e)
	{
		DelayedEventQueue.push(e);
	}

	void update()
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

	//Dispatch sends it to the relavent system to execute event
	void DispatchEvent(Event* toHandle)
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

private:
	std::queue<Event*> EventQueue;
	std::queue<Event*> DelayedEventQueue;
	std::map<SystemID, LISTENER_HANDLER> ListenerMap;
	bool isCombiningQueue = false;
};