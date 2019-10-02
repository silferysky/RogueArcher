#pragma once
#ifndef GRAPICSSYSTEM_H
#define GRAPICSSYSTEM_H
#include "Timer.h"
#include "../Scripts/Main.h"
#include "../Physics/TransformComponent.h"
#include "../Scripts/REMath.h"
#include "../Scripts/EventDispatcher.h"

class Timer;
class GraphicsSystem : public System, public EventListener
{
public:
	GraphicsSystem() = default;
	~GraphicsSystem() = default;

	void init();
	void update() override;

	void receive(Event* ev);
};
#endif