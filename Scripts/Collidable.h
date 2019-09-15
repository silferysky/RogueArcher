#pragma once

class Collidable
{
	std::array<std::unique_ptr<BaseCollider>, MAX_COLLIDERS> m_colliderArray;
public:
	Collidable() : m_colliderArray{ std::make_unique<BaseCollider>() }
	{}
	~Collidable();
};

