#pragma once
#include "Coordinator.h"

class REEngine
{
	float m_accumulatedTime;
	int	m_stepCount;

	bool InitializeOpenGL();
	void RegisterSystems();
	void RegisterComponents();
public:
	Coordinator m_coordinator;

	void init();
	void update();
	void shutdown();
	float GetAccumulatedTime() const;
	int GetStepCount() const;
};