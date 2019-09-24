#pragma once
#include "Logger.h"
#include "BaseSystem.h"
#include "EventDispatcher.h"

class TestSystem : public BaseSystem, public EventListener
{
public:
	TestSystem(SYSTEMID id = (SYSTEMID)1) 
		: BaseSystem(id) 
	{
		LISTENER_HANDLER hand = std::bind(&TestSystem::Receive, this, std::placeholders::_1);
		eventDispatcher.AddListener(id, hand); 
	}
	void Receive(const Event& ev) override { RE_INFO(ev.ToString());  RE_CORE_INFO("TEST SYSTEM RECEIVED"); }
};

