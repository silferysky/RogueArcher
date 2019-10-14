#pragma once
#include "Coordinator.h"
#include "Logger.h"
#include "InputManager.h"
#include "EventDispatcher.h"

class REEngine
{
	float m_accumulatedTime;
	int	m_stepCount;

	void RegisterSystems();
	void RegisterComponents();
public:
	Coordinator m_coordinator;
	REEngine() = default;
	~REEngine() = default;

	void init();
	void update();
	void shutdown();
	float GetAccumulatedTime() const;
	int GetStepCount() const;
};
