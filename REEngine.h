#pragma once
#include "Scripts/Coordinator.h"
#include "Scripts/BaseComponent.h"
#include "Scripts/SpriteComponent.h"
#include "Physics/Rigidbody.h"
#include "Physics/Transform.h"
#include "Physics/CircleCollider2D.h"
#include "BoxCollider2D.h"

#include "Physics/PhysicsSystem.h"

class REEngine
{
public:
	Coordinator m_coordinator;
	REEngine() = default;
	~REEngine() = default;

	void init();
	void update();
};