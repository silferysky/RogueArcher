#pragma once
#include "Logger.h"
#include "SystemList.h"
#include "EventDispatcher.h"

class TestSystem : public System, public EventListener
{
public:
	TestSystem(SystemID id = SystemID::id_TESTSYSTEM)
		: System()
	{
		m_systemID = id;
		LISTENER_HANDLER hand = std::bind(&TestSystem::receive, this, std::placeholders::_1);
		EventDispatcher::instance().AddListener(id, hand);
	}

	void init() {}
	void update() {}
	void receive(Event* ev) override 
	{
		UNREFERENCED_PARAMETER(ev);
		//RE_INFO(ev->ToString());  
		//RE_CORE_INFO("TEST SYSTEM RECEIVED EVENT"); 
	}
};

