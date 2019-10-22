#include "Manifold.h"

Rogue::Manifold::Manifold(Entity a, Entity b) :
	m_point{ Vec2() },
	m_normal{ Vec2() },
	m_normal2{ Vec2() },
	m_penetration{ 0.0f },
	m_restitution{ 0.0f },
	m_contactPoints{ VertexList{} },
	m_bodyA{ 0 },
	m_bodyB{ 0 }
{}
