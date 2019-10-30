#pragma once
#include "Vector2D.h"
#include <array>
#include "Types.h"

namespace Rogue
{
	struct Manifold
	{
		Vec2 m_point;
		Vec2 m_normal;
		Vec2 m_normal2; // Used for OBB
		float m_penetration;
		float m_restitution;

		std::array<Vec2, 2> m_contactPoints;

		Entity m_entityA;
		Entity m_entityB;

		Manifold(Entity a, Entity b);
		~Manifold() = default;

		void Resolve();
		void PositionalCorrection();
	};
}
