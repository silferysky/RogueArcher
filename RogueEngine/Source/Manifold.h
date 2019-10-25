#pragma once
#include "Vector2D.h"
#include <vector>
#include "Types.h"

namespace Rogue
{
	struct Manifold
	{
		using VertexList = std::vector<Vec2>;

		Vec2 m_point;
		Vec2 m_normal;
		Vec2 m_normal2; // Used for OBB
		float m_penetration;
		float m_restitution;

		VertexList m_contactPoints;

		Entity m_entityA;
		Entity m_entityB;

		Manifold(Entity a, Entity b);
		~Manifold() = default;

		void Resolve();
		void PositionalCorrection();
	};
}
