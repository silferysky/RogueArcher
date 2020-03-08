/*Start Header*************************************************************************/
/*!
\file           Manifold.cpp
\project        Exale
\author         Lim Jia Hao, l.jiahao, 390006518 (100%)
\par            l.jiahao\@digipen.edu
\date           1 December,2019
\brief          This file contains the function definitions for Manifold

All content (C) 2019 DigiPen (SINGAPORE) Corporation, all rights
reserved.

Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header **************************************************************************/
#include "Precompiled.h"
#include "Manifold.h"
 
#include "CollisionManager.h"

namespace Rogue
{
	Manifold::Manifold(Entity a, Entity b) :
		m_point{ Vec2() },
		m_normal{ Vec2() },
		m_normal2{ Vec2() },
		m_penetration{ 0.0f },
		m_restitution{ 0.0f },
		m_contactPoints{ Vec2() },
		m_entityA{ a },
		m_entityB{ b }
	{}

	void Manifold::ResolveImpulse()
	{
		auto& bodyA = g_engine.m_coordinator.GetComponent<RigidbodyComponent>(m_entityA);
		auto& bodyB = g_engine.m_coordinator.GetComponent<RigidbodyComponent>(m_entityB);

		if (bodyA.getIsStatic() && bodyB.getIsStatic())
			return;

		// Relative velocity
		Vec2 rv = bodyB.getVelocity() - bodyA.getVelocity();
		
		// Relative velocity along the collision normal
		float velOnNormal = Vec2DotProduct(rv, m_normal);

		// If velocities are already separating, don't resolve anything.
		if (velOnNormal > 0)
			return;

		// Mixed restitution of both bodies
		m_restitution = REMax(bodyA.getBounciness(), bodyB.getBounciness());

		// Scalar value of impulse
		float impulseMagnitude = -(1.0f + m_restitution) * velOnNormal;
		impulseMagnitude /= bodyA.getInvMass() + bodyB.getInvMass();

		Vec2 impulse = impulseMagnitude * m_normal;
		bodyA.offSetVelocity(-bodyA.getInvMass() * impulse);
		bodyB.offSetVelocity(bodyB.getInvMass() * impulse);
	}

	void Manifold::ResolveFriction()
	{
		auto& bodyA = g_engine.m_coordinator.GetComponent<RigidbodyComponent>(m_entityA);
		auto& bodyB = g_engine.m_coordinator.GetComponent<RigidbodyComponent>(m_entityB);
		auto& transA = g_engine.m_coordinator.GetComponent<TransformComponent>(m_entityA);
		auto& transB = g_engine.m_coordinator.GetComponent<TransformComponent>(m_entityB);

		Vec2 relVel = bodyB.getVelocity() - bodyA.getVelocity();
		Vec2 tangent = relVel - Vec2DotProduct(relVel, m_normal) * m_normal;
		Vec2Normalize(tangent, tangent);

		// Magnitude force along the friction vector
		float jt = -Vec2DotProduct(relVel, tangent);
		jt = jt / (bodyA.getInvMass() + bodyB.getInvMass());

		// Find overall friction
		float mu = (bodyA.getFriction() * bodyA.getFriction() + bodyB.getFriction() * bodyB.getFriction()) * 0.5f;

		// Clamp friction magnitude and create impulse vector
		Vec2 frictionImpulse;

		if (REAbs(jt) < jt * mu)
			frictionImpulse = jt * tangent;
		else
			frictionImpulse = -jt * tangent * mu;

		bodyA.offSetVelocity(-bodyA.getInvMass() * frictionImpulse);
		bodyB.offSetVelocity(bodyB.getInvMass() * frictionImpulse);
	}

	void Manifold::PositionalCorrection()
	{
		auto& bodyA = g_engine.m_coordinator.GetComponent<RigidbodyComponent>(m_entityA);
		auto& bodyB = g_engine.m_coordinator.GetComponent<RigidbodyComponent>(m_entityB);
		auto& transA = g_engine.m_coordinator.GetComponent<TransformComponent>(m_entityA);
		auto& transB = g_engine.m_coordinator.GetComponent<TransformComponent>(m_entityB);

		float correctionFactor = CollisionManager::GetCorrectionFactor();
		float correctionSlop = CollisionManager::GetCorrectionSlop();

		Vec2 correction = REMax(m_penetration - correctionSlop, 0.0f) /
			(bodyA.getInvMass() + bodyB.getInvMass()) * correctionFactor * m_normal;

		if(bodyA.getIsStatic() == false)
			transA.offSetPosition(-bodyA.getInvMass() * correction);
	
		if(bodyB.getIsStatic() == false)
			transB.offSetPosition(bodyB.getInvMass() * correction);
	}
}