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

				CollisionManager::instance().CheckExitingCollidedPairs<BoxCollider2DComponent, BoxCollider2DComponent>();
			}


			// Collision Impulse and Torque/Contact Resolution (Other resolutionsdone using trigger events: Other weird forces, rest, game logic)
			CollisionManager::instance().ResolveManifolds();

			//auto& collidedPairs = CollisionManager::instance().GetCollidedPairs();

			//for (auto i = collidedPairs.begin(); i != collidedPairs.end(); i++)
			//{
			//	auto& collidedPair = *i;

			//	auto& currRigidbody = g_engine.m_coordinator.GetComponent<RigidbodyComponent>(collidedPair.first);
			//	auto& currTransform = g_engine.m_coordinator.GetComponent<TransformComponent>(collidedPair.first);
			//	auto& currBoxCollider = g_engine.m_coordinator.GetComponent<BoxCollider2DComponent>(collidedPair.first);

			//	// Skip asleep or static colliders.
			//	if (currBoxCollider.GetCollisionMode() == CollisionMode::e_asleep)
			//		continue;

			//	auto& nextBoxCollider = g_engine.m_coordinator.GetComponent<BoxCollider2DComponent>(collidedPair.second);
			//	auto& nextRigidbody = g_engine.m_coordinator.GetComponent<RigidbodyComponent>(collidedPair.second);
			//	auto& nextTransform = g_engine.m_coordinator.GetComponent<TransformComponent>(collidedPair.second);

			//	// Skip asleep or static colliders.
			//	if (nextBoxCollider.GetCollisionMode() == CollisionMode::e_asleep)
			//		continue;

			//	if (currRigidbody.getIsStatic() && nextRigidbody.getIsStatic())
			//		continue;

			//	// Test for AABBs vs AABBs FALSE TEST
			//	if (!CollisionManager::instance().DiscreteAABBvsAABB(currBoxCollider.m_aabb, nextBoxCollider.m_aabb))
			//	{
			//		// If A and/or B is/are a trigger(s), dispatch trigger exit event(s).
			//		if (currBoxCollider.GetCollisionMode() == CollisionMode::e_trigger)
			//		{
			//			EntTriggerExitEvent* ev = new EntTriggerExitEvent{ *iEntity, *iNextEntity };
			//			ev->SetSystemReceivers((int)SystemID::id_LOGICSYSTEM);
			//			EventDispatcher::instance().AddEvent(ev);
			//		}
			//		if (nextBoxCollider.GetCollisionMode() == CollisionMode::e_trigger)
			//		{
			//			EntTriggerExitEvent* ev = new EntTriggerExitEvent{ *iNextEntity, *iEntity };
			//			ev->SetSystemReceivers((int)SystemID::id_LOGICSYSTEM);
			//			EventDispatcher::instance().AddEvent(ev);
			//		}
			//		else
			//		{
			//			EntCollisionExitEvent* ev =
			//				new EntCollisionExitEvent{ *iEntity, *iNextEntity };
			//			ev->SetSystemReceivers((int)SystemID::id_PLAYERCONTROLLERSYSTEM);
			//			EventDispatcher::instance().AddEvent(ev);

			//			CollisionManager::instance().GenerateManifolds(*iEntity, *iNextEntity);
			//		}

			//		collidedPairs.erase(i);
			//	}
			//}

			// Test OBBs vs OBBs collision
			//if (CollisionManager::instance().DiscreteOBBvsOBB(currBoxCollider.m_obb, nextBoxCollider.m_obb))
			//{
			//	std::cout << "Entity " << *iEntity << " OBB collides with Entity " << *iNextEntity << " OBB" << std::endl;
			//}

		}
	}

	void BoxCollisionSystem::Receive(Event* ev)
	{}

	void BoxCollisionSystem::Shutdown()
	{}
}