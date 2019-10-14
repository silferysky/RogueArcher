#pragma once
#include <functional>
#include "logger.h"
#include "Event.h"

class SystemList;

//Used to override template events
class BaseEventListener
{
public:
	virtual ~BaseEventListener() {};
};

//EventListener denotes the derived class is actively listening for events
class EventListener : public BaseEventListener
{
public:
	virtual ~EventListener() override {};
	virtual void receive(Event* event) = 0;

	SystemList* SysListener = nullptr;
};

using LISTENER_HANDLER = std::function<void(Event*)>;
#define ADD_LISTENER(id, func)	LISTENER_HANDLER hand = std::bind(&func, this, std::placeholders::_1); \
								eventDispatcher.AddListener(id, hand)