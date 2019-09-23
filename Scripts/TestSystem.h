#pragma once
#include "Logger.h"
#include "BaseSystem.h"
#include "Event.h"

class TestSystem : public BaseSystem
{
public:
	TestSystem(SYSTEMID id = (SYSTEMID)1) : BaseSystem(id) {}
	virtual void Receive(const Event& event) { RE_CORE_INFO("TEST SYSTEM RECEIVED"); }
};