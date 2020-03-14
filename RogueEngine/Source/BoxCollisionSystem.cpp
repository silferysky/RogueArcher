/* Start Header ************************************************************************/
/*!
\file           BoxCollisionSystem.cpp
\project        Exale
\author         Lim Jia Hao, l.jiahao, 390006518 (100%)
\par            l.jiahao\@digipen.edu
\date           1 December,2019
\brief          This file contains the function definitions for BoxCollisionSystem

All content (C) 2019 DigiPen (SINGAPORE) Corporation, all rights
reserved.

Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header **************************************************************************/
#include "Precompiled.h"
#include "BoxCollisionSystem.h"
#include "SystemManager.h"
#include "EventListener.h"
#include "CollisionSystem.h"
#include "EditorHierarchyInfo.h"
#include "Main.h"

namespace Rogue
{
	BoxCollisionSystem::BoxCollisionSystem()
		: System(SystemID::id_BOXCOLLISIONSYSTEM)
	{}

	void BoxCollisionSystem::Init()
	{
		REGISTER_LISTENER(SystemID::id_BOXCOLLISIONSYSTEM, BoxCollisionSystem::Receive);

		// Add components to signature.
		Signature signature;
		signature.set(g_engine.m_coordinator.GetComponentType<RigidbodyComponent>());
		signature.set(g_engine.m_coordinator.GetComponentType<TransformComponent>());
		signature.set(g_engine.m_coordinator.GetComponentType<BoxCollider2DComponent>());

		// Set Collision system signature.
		g_engine.m_coordinator.SetSystemSignature<BoxCollisionSystem>(signature);
	}

	void BoxCollisionSystem::Update()
	{
		std::set<Entity>::iterator iEntity;

		// Update colliders and partition them.
		for (iEntity = m_entities.begin(); iEntity != m_entities.end(); ++iEntity)
		{
			auto& currTransform = g_engine.m_coordinator.GetComponent<TransformComponent>(*iEntity);
			auto& currBoxCollider = g_engine.m_coordinator.GetComponent<BoxCollider2DComponent>(*iEntity);

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
			auto currColliderOpt = g_engine.m_coordinator.TryGetComponent<ColliderComponent>(*iEntity);

			RE_ASSERT(static_cast<bool>(currColliderOpt), "Entity doesn't have ColliderComponent");

			auto& currCollider = currColliderOpt->get();
			auto& currRigidbody = g_engine.m_coordinator.GetComponent<RigidbodyComponent>(*iEntity);
			auto& currTransform = g_engine.m_coordinator.GetComponent<TransformComponent>(*iEntity);
			auto& currBoxCollider = g_engine.m_coordinator.GetComponent<BoxCollider2DComponent>(*iEntity);

			// Test AABB/OBB Collision
			std::set<Entity>::iterator iNextEntity = iEntity;

			// For each entity, the rest of the entities
			for (++iNextEntity; iNextEntity != m_entities.end(); ++iNextEntity)
			{
				auto nextColliderOpt = g_engine.m_coordinator.TryGetComponent<ColliderComponent>(*iNextEntity);

				RE_ASSERT(static_cast<bool>(nextColliderOpt), "Entity doesn't have ColliderComponent");

				auto& nextCollider = nextColliderOpt->get();

				// Filter colliders
				if (!CollisionManager::instance().FilterColliders(currCollider.GetCollisionMask(), nextCollider.GetCollisionCat()) ||
					!CollisionManager::instance().FilterColliders(currCollider.GetCollisionCat(), nextCollider.GetCollisionMask()))
					continue;

				auto& nextRigidbody = g_engine.m_coordinator.GetComponent<RigidbodyComponent>(*iNextEntity);
				
				// If the bodies are not player
				if (*iEntity != PLAYER_STATUS.GetPlayerEntity() && *iNextEntity != PLAYER_STATUS.GetPlayerEntity())
				{
					// Skip if both static.
					if (currRigidbody.getIsStatic() && nextRigidbody.getIsStatic())
						continue;
				}

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
					{
						CollisionManager::instance().SendEnterEvents(infoA, infoB);
					}
					else
					{
						CollisionManager::instance().SendStayEvents(infoA, infoB);
					}
					// If at least one of them is a trigger, skip resolution
					if (currBoxCollider.GetCollisionMode() == CollisionMode::e_trigger || nextBoxCollider.GetCollisionMode() == CollisionMode::e_trigger)
						continue;

					// Generate manifolds from collided pairs
					CollisionManager::instance().GenerateManifoldAABBvsAABB(*iEntity, *iNextEntity);
				}
				// Remove exiting pairs and send exit events
				CollisionManager::instance().CheckExitingCollidedPairs<BoxCollider2DComponent, BoxCollider2DComponent>(m_entities);
			}
		}
		// Collision Impulse and Torque/Contact Resolution (Other resolutionsdone using trigger events: Other weird forces, rest, game logic)
		CollisionManager::instance().ResolveManifolds();
	}

	void BoxCollisionSystem::Receive(Event& ev)
	{
#if TEST_COLLISION_EVENTS
		if (ev.GetEventCat() & EventCatCollision)
		{
			switch (ev.GetEventType())
			{
			case EventType::EvOnCollisionEnter:
			{
				auto* collisionEnter = dynamic_cast<AABBCollisionEnterEvent&>(ev);

				collisionEnter->GetThis().m_collider.SetIsCollided(true);
				collisionEnter->GetOther().m_collider.SetIsCollided(true);

				return;
			}

			case EventType::EvOnCollisionExit:
			{
				auto* collisionExit = dynamic_cast<AABBCollisionExitEvent&>(ev);

				collisionExit->GetThis().m_collider.SetIsCollided(false);
				collisionExit->GetOther().m_collider.SetIsCollided(false);

				return;
			}
			}
		}
		else if (ev.GetEventCat() & EventCatTrigger)
		{
			switch (ev.GetEventType())
			{
			case EventType::EvOnCollisionEnter:
			{
				auto* collisionEnter = dynamic_cast<AABBTriggerEnterEvent&>(ev);

				collisionEnter->GetThis().m_collider.SetIsCollided(true);
				collisionEnter->GetOther().m_collider.SetIsCollided(true);

				return;
			}

			case EventType::EvOnCollisionExit:
			{
				auto* collisionExit = dynamic_cast<AABBTriggerExitEvent&>(ev);

				collisionExit->GetThis().m_collider.SetIsCollided(false);
				collisionExit->GetOther().m_collider.SetIsCollided(false);

				return;
			}
			}
		}
#endif
	}

	void BoxCollisionSystem::Shutdown()
	{}
}