#pragma once
#include "Scripts/Coordinator.h"
#include "Scripts/BaseComponent.h"
#include "Scripts/BaseSystem.h"

class REEngine
{
public:
	Coordinator RECoordinator;
	REEngine() = default;
	~REEngine() = default;

	void init();
	void update();
};