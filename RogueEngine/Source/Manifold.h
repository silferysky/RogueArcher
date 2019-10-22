#pragma once
#include "Vector2D.h"
#include "RigidbodyComponent.h"
#include <vector>
#include "OBB.h"
#include "Types.h"

namespace Rogue
{
	struct Manifold
	{
		using VertexList = std::array<Vec2, 2>;

		Vec2 m_point;
		Vec2 m_normal;
		Vec2 m_normal2; // Used for OBB
		float m_penetration;
		float m_restitution;
		
		VertexList m_contactPoints;
		
		Entity m_bodyA;
		Entity m_bodyB;

		Manifold(Entity a = 0, Entity b = 0);
		~Manifold() = default;
	};
}

