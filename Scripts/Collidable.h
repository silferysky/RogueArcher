#pragma once
#include <memory> //unique_ptr
#include "Physics.h"

class Collidable
{
	//std::array<std::unique_ptr<BaseCollider>, MAX_COLLIDERS> m_colliderArray;
public:
	//Collidable() : m_colliderArray{ std::make_unique<BaseCollider>() } {}
	~Collidable() {};
};

