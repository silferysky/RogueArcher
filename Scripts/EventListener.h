#pragma once

#include <functional>
#include "RogueEngine.h"
#include "Event.h"

class BaseSystem;
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
	virtual void Receive(Event* event) = 0;

	BaseSystem* SysListener = nullptr;
};

using LISTENER_HANDLER = std::function<void(Event*)>;
#define ADD_LISTENER(id, func)	LISTENER_HANDLER hand = std::bind(&func, this, std::placeholders::_1); \
								eventDispatcher.AddListener(id, hand)