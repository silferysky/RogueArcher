#pragma once
#include "Scripts/Coordinator.h"
#include "Scripts/Logger.h"

class REEngine
{
public:
	Logger m_log;
	Coordinator m_coordinator;
	REEngine() = default;
	~REEngine() = default;

	void init();
	void update();
};