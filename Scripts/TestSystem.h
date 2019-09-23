#pragma once
#include "Logger.h"
#include "BaseSystem.h"
#include "EventDispatcher.h"

class TestSystem : public BaseSystem
{
public:
	TestSystem(SYSTEMID id = (SYSTEMID)1) : BaseSystem(id) { /*eventDispatcher.AddListener(id, (LISTENER_HANDLER)Receive); */}
	void Receive(const Event& ev) { RE_INFO(ev.ToString());  RE_CORE_INFO("TEST SYSTEM RECEIVED"); }
};
