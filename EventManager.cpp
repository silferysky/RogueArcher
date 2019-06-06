#include "EventManager.h"

EventCategory Event::getEventCat()
{
	return eventCategory;
}

EventManager::EventManager()
{
	EventQueue = new std::queue<Event>;
}

EventManager::~EventManager()
{
	delete EventQueue;
}

void EventManager::Update()
{
	Event* nextEvent = getQueueHead();
	if (nextEvent != nullptr)
	{
		HandleEvent(nextEvent);
		EventQueue->pop();
	}
}

Event * EventManager::getQueueHead()
{
	return EventQueue->front;
}

void EventManager::HandleEvent(Event* toHandle)
{
	switch (toHandle->getEventCat())
	{

	default:
	}
}
