#pragma once

#include <queue>
#include <map>
#include <vector>
#include "EventListener.h"

static class EventDispatcher :
	public BaseSystem
{
public:

	EventDispatcher() 
	{ 
		EventQueue = std::queue<Event>();
		DelayedEventQueue = std::queue<Event>();
		ListenerMap = std::map<SYSTEMID, std::vector<BaseEventListener*>>();

	}
	~EventDispatcher() { }

	Event& GetQueueHead() { return EventQueue.front(); }
	Event& GetQueueHeadDelayed() { return DelayedEventQueue.front(); }
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

	template <typename T>
	void AddListener(BaseSystem* sys, EventListener<T> lis)
	{
		SYSTEMID ID = sys->ID();

		//Add a map key if not found
		if (ListenerMap.find(ID) == ListenerMap.end())
		{
			std::vector<BaseEventListener*> newListener();
			ListenerMap.insert(ID, newListener);
			RE_CORE_INFO("Added new key to ListenerMap");
		}

		//Adding value into vector
		lis->SysListener = sys;
		ListenerMap[ID].push_back(lis);
		RE_CORE_INFO("Added new data to ListenerMap key");
	}
	//Removes all listeners in map
	void RemoveAllListener()
	{
		for (int i = 0; i < (int)LASTSYS; ++i)
		{
			RemoveAllListener((SYSTEMID)i);
		}
	}
	//Removes all listeners in a system
	void RemoveAllListener(SYSTEMID ID)
	{
		//Saving an unsigned int to check if value is removed
		size_t i = ListenerMap[ID].size();
		//Clear vector in map
		ListenerMap[ID].clear();

		if (i > ListenerMap[ID].size())
			RE_CORE_INFO("Removed key from ListenerMap");
		else
			RE_CORE_INFO("Attempted but failed to remove key from ListenerMap");
	}
	template <typename T>
	void RemoveListener(SYSTEMID ID, EventListener<T> lis)
	{
		for (auto it = ListenerMap[ID].begin(); it != ListenerMap[ID].end(); ++it)
		{
			//Convert to same type
			auto baseLis = reinterpret_cast<BaseEventListener*>(lis);
			if (*it == baseLis)
			{
				ListenerMap[ID].erase(it);
				return;
			}
		}

	}

	std::map<SYSTEMID, std::vector<BaseEventListener*>> GetMap()
	{
		return ListenerMap;
	}

	void AddEvent(Event& e)
	{
		EventQueue.push(e);
	}
	void AddEventDelayed(Event& e)
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
			Event& nextEvent = GetQueueHead();
			if (!nextEvent.Handled())
			{
				DispatchEvent(nextEvent);
			}
			EventQueue.pop();
		}
	}

	//Dispatch sends it to the relavent system to execute event
	template <typename T>
	void DispatchEvent(const T& toHandle)
	{
		//Is better to let individual systems handle events than handle all here
		//Basically: Send a message to the relevant system to activate relavent function

		auto sysIt = ListenerMap.begin();
		while (sysIt != ListenerMap.end())
		{
			//baseLis is expected to be baseListener
			for (auto *baseLis : sysIt->second)
			{
				auto *lis = reinterpret_cast<EventListener<T>*>(baseLis);
				lis->Receive(toHandle);
			}
			//Move on to next system
			++sysIt;
		}
	}

private:
	std::queue<Event> EventQueue;
	std::queue<Event> DelayedEventQueue;
	std::map<SYSTEMID, std::vector<BaseEventListener*>> ListenerMap;
	bool isCombiningQueue = false;

} eventDispatcher;
