#pragma once
#include "Coordinator.h"
#include "Logger.h"
#include "InputManager.h"
#include "EventDispatcher.h"
#include "glfw3.h"

class REEngine
{
	bool InitializeOpenGL();
	void RegisterSystems();
	void RegisterComponents();
public:
	Coordinator m_coordinator;
	InputManager m_inputManager;
	EventDispatcher m_eventDispatcher;
	REEngine() = default;
	~REEngine() = default;

	void init();
	void update();
};
