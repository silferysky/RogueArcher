#pragma once
#include "Scripts/Coordinator.h"

class REEngine
{
public:
	Coordinator m_coordinator;
	REEngine() = default;
	~REEngine() = default;

	void init();
	void update();
};