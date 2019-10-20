#pragma once
#include "REEngine.h"

class CollisionSystem :
	public System, public EventListener
{
	CollisionManager m_colliderManager;
public:
	CollisionSystem();

	void init() override;
	void update() override;
	void receive(Event* ev) override;
};