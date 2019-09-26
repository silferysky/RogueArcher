#pragma once

#include <queue>
#include <map>
#include <vector>
#include "EventListener.h"

class EventDispatcher :
	public BaseSystem
{
public:

	EventDispatcher() 
	{ 
		EventQueue = std::queue<Event*>();
		DelayedEventQueue = std::queue<Event*>();
		ListenerMap = std::map<SYSTEMID, LISTENER_HANDLER>();

	}
	~EventDispatcher() { }

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

	void AddListener(SYSTEMID ID, LISTENER_HANDLER handler)
	{
		ListenerMap.insert(std::pair<SYSTEMID, LISTENER_HANDLER>(ID, handler));
		RE_CORE_INFO("Added new key to ListenerMap");
	}
	//Removes all listeners in map
	void RemoveAllListener()
	{
		ListenerMap.clear();
	}
	void RemoveListener(SYSTEMID ID)
	{
		for (auto it = ListenerMap.begin(); it != ListenerMap.end(); ++it)
		{
			if (it->first == ID)
			{
				ListenerMap.erase(it);
			}
		}
	}

	std::map<SYSTEMID, LISTENER_HANDLER> GetMap()
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
	std::map<SYSTEMID, LISTENER_HANDLER> ListenerMap;
	bool isCombiningQueue = false;
};