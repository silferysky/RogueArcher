/*Start Header************************************************************************ /
/*!
\file           CollisionSystem.cpp
\project        Exale
\author         Lim Jia Hao, l.jiahao, 390006518 (100%)
\par            l.jiahao\@digipen.edu
\date           1 December,2019
\brief          This file contains the function definitions for CollisionSystem

All content (C) 2019 DigiPen (SINGAPORE) Corporation, all rights
reserved.

Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header **************************************************************************/
#include "Precompiled.h"
#include "CollisionSystem.h"
#include "SystemManager.h"
#include "EventListener.h"
#include "Main.h"

namespace Rogue
{
	CollisionSystem::CollisionSystem()
		: System(SystemID::id_COLLISIONSYSTEM)
	{}

	void CollisionSystem::Init()
	{
		REGISTER_LISTENER(SystemID::id_COLLISIONSYSTEM, CollisionSystem::Receive);

		// Add components to signature.
		Signature signature;
		signature.set(g_engine.m_coordinator.GetComponentType<RigidbodyComponent>());
		signature.set(g_engine.m_coordinator.GetComponentType<TransformComponent>());
		signature.set(g_engine.m_coordinator.GetComponentType<ColliderComponent>());

		// Set Collision system signature.
		g_engine.m_coordinator.SetSystemSignature<CollisionSystem>(signature);
	}

	void CollisionSystem::Update()
	{
		std::set<Entity>::iterator iEntity;

		// Update colliders and partition them.
		for (iEntity = m_entities.begin(); iEntity != m_entities.end(); ++iEntity)
		{
			auto& currTransform = g_engine.m_coordinator.GetComponent<TransformComponent>(*iEntity);
			if (auto currCollOption = g_engine.m_coordinator.TryGetComponent<BoxCollider2DComponent>(*iEntity))
			{

			}
			// Skip asleep colliders
			if (currBoxCollider.GetCollisionMode() == CollisionMode::e_asleep)
				continue;

			// Update collider
			CollisionManager::instance().UpdateAABB(currBoxCollider.m_aabb, currTransform);

			// Conduct spatial partitioning
		}

		// Test for collisions
		// Loop through entities
		for (iEntity = m_entities.begin(); iEntity != m_entities.end(); ++iEntity)
		{
			auto& currCollider = g_engine.m_coordinator.GetComponent<ColliderComponent>(*iEntity);
			auto& currRigidbody = g_engine.m_coordinator.GetComponent<RigidbodyComponent>(*iEntity);
			auto& currTransform = g_engine.m_coordinator.GetComponent<TransformComponent>(*iEntity);
			auto& currBoxCollider = g_engine.m_coordinator.GetComponent<BoxCollider2DComponent>(*iEntity);

			// Test AABB/OBB Collision
			std::set<Entity>::iterator iNextEntity = iEntity;

			// For each entity, the rest of the entities
			for (iNextEntity++; iNextEntity != m_entities.end(); ++iNextEntity)
			{
				auto& nextCollider = g_engine.m_coordinator.GetComponent<ColliderComponent>(*iNextEntity);

				// Filter colliders
				if (!CollisionManager::instance().FilterColliders(currCollider.GetCollisionMask(), nextCollider.GetCollisionCat()) ||
					!CollisionManager::instance().FilterColliders(currCollider.GetCollisionCat(), nextCollider.GetCollisionMask()))
					continue;

				auto& nextRigidbody = g_engine.m_coordinator.GetComponent<RigidbodyComponent>(*iNextEntity);

				// Skip if both static.
				if (currRigidbody.getIsStatic() && nextRigidbody.getIsStatic())
					continue;

				auto& nextBoxCollider = g_engine.m_coordinator.GetComponent<BoxCollider2DComponent>(*iNextEntity);

				// Skip asleep colliders.
				if (nextBoxCollider.GetCollisionMode() == CollisionMode::e_asleep)
					continue;

				auto& nextTransform = g_engine.m_coordinator.GetComponent<TransformComponent>(*iNextEntity);

				// Test for AABBs vs AABBs
				if (CollisionManager::instance().DiscreteAABBvsAABB(currBoxCollider.m_aabb, nextBoxCollider.m_aabb))
				{
					CollisionInfo<BoxCollider2DComponent> infoA(*iEntity, currBoxCollider, currRigidbody, currTransform);
					CollisionInfo<BoxCollider2DComponent> infoB(*iNextEntity, nextBoxCollider, nextRigidbody, nextTransform);

					// Set booleans for both colliders/triggers
					currBoxCollider.SetIsCollided(true);
					nextBoxCollider.SetIsCollided(true);

					if (CollisionManager::instance().InsertBoxPair(*iEntity, *iNextEntity))
						CollisionManager::instance().SendEnterEvents(infoA, infoB);
					else
						CollisionManager::instance().SendStayEvents(infoA, infoB);

					// If at least one of them is a trigger, skip resolution
					if (currBoxCollider.GetCollisionMode() == CollisionMode::e_trigger || nextBoxCollider.GetCollisionMode() == CollisionMode::e_trigger)
						continue;

					// Generate manifolds from collided pairs
					CollisionManager::instance().GenerateManifoldAABBvsAABB(*iEntity, *iNextEntity);
				}
				// Remove exiting pairs and send exit events
				CollisionManager::instance().CheckExitingCollidedPairs<BoxCollider2DComponent, BoxCollider2DComponent>(m_entities);
			}
			// Collision Impulse and Torque/Contact Resolution (Other resolutionsdone using trigger events: Other weird forces, rest, game logic)
			CollisionManager::instance().ResolveManifolds();
		}
	}

	void CollisionSystem::Receive(Event* ev)
	{
	}

	void CollisionSystem::Shutdown()
	{}
}