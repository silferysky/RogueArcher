#pragma once
#include "../Scripts/Main.h"
#include "../Physics/TransformComponent.h"
#include "../Scripts/REMath.h"
#include "../Scripts/EventDispatcher.h"

class GraphicsSystem : public System, public EventListener
{
public:
	GraphicsSystem() = default;
	~GraphicsSystem() = default;

	void init();
	void update() override;

	void receive(Event* ev);
};