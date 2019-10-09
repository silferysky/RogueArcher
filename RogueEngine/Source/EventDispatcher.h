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

	static EventDispatcher& instance()
	{
		static EventDispatcher instance;
		return instance;
	}

	EventDispatcher() = default;
	~EventDispatcher() = default;

	void init();

	//Queue functions
	Event* GetQueueHead();
	Event* GetQueueHeadDelayed();
	void CombineQueue();
	void CombineQueueCmd(Event& e);

	//Listener functions
	void AddListener(SystemID ID, LISTENER_HANDLER handler);
	void RemoveAllListener();
	void RemoveListener(SystemID ID);
	std::map<SystemID, LISTENER_HANDLER> GetMap();

	void AddEvent(Event* e);
	void AddEventDelayed(Event* e);

	void update();

	//Dispatch sends it to the relavent system to execute event
	void DispatchEvent(Event* toHandle);

private:
	std::queue<Event*> EventQueue;
	std::queue<Event*> DelayedEventQueue;
	std::map<SystemID, LISTENER_HANDLER> ListenerMap;
	bool isCombiningQueue = false;
};