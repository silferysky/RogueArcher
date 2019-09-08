#pragma once
#include <queue>

enum EventCategory
{
	AudioEvent = 0,
	PlayerEvent,
	EnemyEvent,
	MapEvent
};

class Event
{
	EventCategory eventCategory;
	int eventType; //This denotes the exact type of event, aka an audio event can be sound/music
	int value[5] = { 0 }; //All values necessary to denote event specifications

public:
	EventCategory getEventCat();
};

class EventManager
{
public:
	EventManager();
	~EventManager();

	void Update();

	Event* getQueueHead();
	void HandleEvent(Event* toHandle);

private:
	std::queue<Event> *EventQueue;
};

