#pragma once

#include "RogueEngine.h"
#include "Event.h"

class BaseSystem;
//Used to override template events
class BaseEventListener
{
public:
	virtual ~BaseEventListener() {};
};

//EventListener listens to one single event only
template<typename T>
class EventListener : public BaseEventListener
{
public:
	virtual ~EventListener() override {};
	//virtual void Receive(const T& event) = 0;

	BaseSystem* SysListener = nullptr;
};

typedef void(*LISTENER_HANDLER)(const Event&);