#pragma once
#include <queue>
#include "System.h"

enum EventCategory
{
	AUDIOEVENT = 0,
	PLAYEREVENT,
	ENEMYEVENT,
	MAPEVENT
};

class Event
{
	EventCategory eventCategory;
	int eventType; //This denotes the exact type of event, aka an audio event can be sound/music
	int value[5] = { 0 }; //All values necessary to denote event specifications

public:
	EventCategory getEventCat();
};

class EventManager :
	public System
{
public:
	EventManager();
	~EventManager();

	void Update();

	Event* getQueueHead();
	void HandleEvent(Event* toHandle);

private:
	std::queue<Event*> *EventQueue;
};

