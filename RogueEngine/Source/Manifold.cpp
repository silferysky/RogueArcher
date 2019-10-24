#include "Manifold.h"
#include "Main.h"

namespace Rogue
{
	Manifold::Manifold(Entity a, Entity b) :
		m_point{ Vec2() },
		m_normal{ Vec2() },
		m_normal2{ Vec2() },
		m_penetration{ 0.0f },
		m_restitution{ 0.0f },
		m_contactPoints{ VertexList{} },
		m_entityA{ a },
		m_entityB{ b }
	{}

	void Manifold::Resolve()
	{
		auto& bodyA = gEngine.m_coordinator.GetComponent<RigidbodyComponent>(m_entityA);
		auto& bodyB = gEngine.m_coordinator.GetComponent<RigidbodyComponent>(m_entityB);
			
		// Relative velocity
		Vec2 rv = bodyB.getVelocity() - bodyA.getVelocity();
		
		// Relative velocity along the collision normal
		float velOnNormal = Vec2DotProd(rv, m_normal);

		// If velocities are already separating, don't resolve anything.
		if (velOnNormal > 0)
		{
			std::cout << m_entityA << " and " << m_entityB << " separating" << std::endl;
			return;
		}
		// Mixed restitution of both bodies
		m_restitution = REMax(bodyA.getBounciness(), bodyB.getBounciness());

		// Scalar value of impulse
		float impulseMagnitude = -(1 + m_restitution) * velOnNormal;
		impulseMagnitude /= bodyA.getInvMass() + bodyB.getInvMass();

		Vec2 impulse = impulseMagnitude * m_normal;
		bodyA.offSetVelocity(-bodyA.getInvMass() * impulse);
		bodyB.offSetVelocity(bodyB.getInvMass() * impulse);
	}
}