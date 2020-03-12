/* Start Header ************************************************************************/
/*!
\file           CircleCollisionSystem.cpp
\project        Exale
\author         Lim Jia Hao, l.jiahao, 390006518 (100%)
\par            l.jiahao\@digipen.edu
\date           1 December,2019
\brief          This file contains the function definitions for CircleCollisionSystem

All content (C) 2019 DigiPen (SINGAPORE) Corporation, all rights
reserved.

Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header **************************************************************************/
#include "Precompiled.h"
#include "CircleCollisionSystem.h"
#include "SystemManager.h"
#include "EventListener.h"
#include "Main.h"
#include "CollisionSystem.h"

namespace Rogue
{
	CircleCollisionSystem::CircleCollisionSystem()
		: System(SystemID::id_CIRCLECOLLISIONSYSTEM)
	{}

	void CircleCollisionSystem::Init()
	{
		REGISTER_LISTENER(SystemID::id_CIRCLECOLLISIONSYSTEM, CircleCollisionSystem::Receive);

		// Add components to signature.
		Signature signature;
		signature.set(g_engine.m_coordinator.GetComponentType<RigidbodyComponent>());
		signature.set(g_engine.m_coordinator.GetComponentType<TransformComponent>());
		signature.set(g_engine.m_coordinator.GetComponentType<CircleCollider2DComponent>());

		// Set Collision system signature.
		g_engine.m_coordinator.SetSystemSignature<CircleCollisionSystem>(signature);
	}

	void CircleCollisionSystem::Update()
	{
		std::set<Entity>::iterator iEntity;

		// Update colliders and partition them.
		for (iEntity = m_entities.begin(); iEntity != m_entities.end(); ++iEntity)
		{
			auto& currTransform = g_engine.m_coordinator.GetComponent<TransformComponent>(*iEntity);
			auto& currCircleCollider = g_engine.m_coordinator.GetComponent<CircleCollider2DComponent>(*iEntity);

			// Skip asleep colliders
			//if (currCircleCollider.GetCollisionMode() == CollisionMode::e_asleep)
			//	continue;

			// Update collider
			CollisionManager::instance().UpdateBoundingCircle(currCircleCollider.m_collider, currTransform);

			// Conduct spatial partitioning
		}

		// Test for collisions
		// Loop through entities
		for (iEntity = m_entities.begin(); iEntity != m_entities.end(); ++iEntity)
		{
			auto currColliderOpt = g_engine.m_coordinator.TryGetComponent<ColliderComponent>(*iEntity);
#if 1
			RE_ASSERT(static_cast<bool>(currColliderOpt), "Entity doesn't have ColliderComponent");

#endif
			auto& currCollider = currColliderOpt->get();
			auto& currRigidbody = g_engine.m_coordinator.GetComponent<RigidbodyComponent>(*iEntity);
			auto& currTransform = g_engine.m_coordinator.GetComponent<TransformComponent>(*iEntity);
			auto& currCircleCollider = g_engine.m_coordinator.GetComponent<CircleCollider2DComponent>(*iEntity);

			// Test AABB/OBB Collision
			std::set<Entity>::iterator iNextEntity = iEntity;

			// For each entity, the rest of the entities
			for (++iNextEntity; iNextEntity != m_entities.end(); ++iNextEntity)
			{
				auto nextColliderOpt = g_engine.m_coordinator.TryGetComponent<ColliderComponent>(*iNextEntity);
#if 1
				RE_ASSERT(static_cast<bool>(nextColliderOpt), "Entity doesn't have ColliderComponent");
#endif
				auto& nextCollider = nextColliderOpt->get();

				// Filter colliders
				if (!CollisionManager::instance().FilterColliders(currCollider.GetCollisionMask(), nextCollider.GetCollisionCat()) ||
					!CollisionManager::instance().FilterColliders(currCollider.GetCollisionCat(), nextCollider.GetCollisionMask()))
					continue;

				auto& nextRigidbody = g_engine.m_coordinator.GetComponent<RigidbodyComponent>(*iNextEntity);

				// Skip if both static.
				if (currRigidbody.getIsStatic() && nextRigidbody.getIsStatic())
					continue;

				auto& nextCircleCollider = g_engine.m_coordinator.GetComponent<CircleCollider2DComponent>(*iNextEntity);

				// Skip asleep colliders.
				//if (nextCircleCollider.GetCollisionMode() == CollisionMode::e_asleep)
				//	continue;

				auto& nextTransform = g_engine.m_coordinator.GetComponent<TransformComponent>(*iNextEntity);

				// Test for AABBs vs AABBs
				if (CollisionManager::instance().DiscreteCircleVsCircle(nextCircleCollider.m_collider, nextCircleCollider.m_collider))
				{
					CollisionInfo<CircleCollider2DComponent> infoA(*iEntity, currCircleCollider, currRigidbody, currTransform);
					CollisionInfo<CircleCollider2DComponent> infoB(*iNextEntity, nextCircleCollider, nextRigidbody, nextTransform);

					// Set booleans for both colliders/triggers
					currCircleCollider.SetIsCollided(true);
					nextCircleCollider.SetIsCollided(true);

					if (CollisionManager::instance().InsertBoxPair(*iEntity, *iNextEntity))
					{
						CollisionManager::instance().SendEnterEvents(infoA, infoB);
					}
					else
					{
						CollisionManager::instance().SendStayEvents(infoA, infoB);
					}

					// If at least one of them is a trigger, skip resolution
					if (currCircleCollider.GetCollisionMode() == CollisionMode::e_trigger || nextCircleCollider.GetCollisionMode() == CollisionMode::e_trigger)
						continue;

					// Generate manifolds from collided pairs
					CollisionManager::instance().GenerateManifoldCirclevsCircle(*iEntity, *iNextEntity);
				}
				// Remove exiting pairs and send exit events
				//CollisionManager::instance().CheckExitingCollidedPairs<CircleCollider2DComponent, CircleCollider2DComponent>(m_entities);
			}
		}
		// Collision Impulse and Torque/Contact Resolution (Other resolutionsdone using trigger events: Other weird forces, rest, game logic)
		CollisionManager::instance().ResolveManifolds();
	}

	void CircleCollisionSystem::Receive(Event& ev)
	{}

	void CircleCollisionSystem::Shutdown()
	{}
}