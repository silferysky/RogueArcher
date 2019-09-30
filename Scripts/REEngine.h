#pragma once
#include "Coordinator.h"
#include "Logger.h"

class REEngine
{
public:
	Coordinator m_coordinator;
	REEngine() = default;
	~REEngine() = default;

	void init();
	void update();
};
