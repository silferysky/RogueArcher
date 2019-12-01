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
		g_engine.m_coordinator.InitTimeSystem("Box Collision System");

		std::set<Entity>::iterator iEntity;

		// Update colliders and partition them.
		for (iEntity = m_entities.begin(); iEntity != m_entities.end(); ++iEntity)
		{
			auto& currTransform = g_engine.m_coordinator.GetComponent<TransformComponent>(*iEntity);
			auto& currBoxCollider = g_engine.m_coordinator.GetComponent<BoxCollider2DComponent>(*iEntity);

			// Skip asleep colliders
			if (currBoxCollider.GetCollisionMode() == CollisionMode::e_asleep)
				continue;

			// Update collidables
			CollisionManager::instance().UpdateAABB(currBoxCollider.m_aabb, currTransform);
			CollisionManager::instance().UpdateOBB(currBoxCollider.m_obb, currTransform);

			// Conduct spatial partitioning
		}

		// Test for collisions
		// Loop through entities
		for (iEntity = m_entities.begin(); iEntity != m_entities.end(); ++iEntity)
		{
			auto& currRigidbody = g_engine.m_coordinator.GetComponent<RigidbodyComponent>(*iEntity);
			auto& currTransform = g_engine.m_coordinator.GetComponent<TransformComponent>(*iEntity);
			auto& currBoxCollider = g_engine.m_coordinator.GetComponent<BoxCollider2DComponent>(*iEntity);

			// Test AABB/OBB Collision
			std::set<Entity>::iterator iNextEntity = iEntity;

			// For each entity, the rest of the entities
			for (iNextEntity++; iNextEntity != m_entities.end(); ++iNextEntity)
			{
				auto& nextBoxCollider = g_engine.m_coordinator.GetComponent<BoxCollider2DComponent>(*iNextEntity);
				auto& nextRigidbody = g_engine.m_coordinator.GetComponent<RigidbodyComponent>(*iNextEntity);
				auto& nextTransform = g_engine.m_coordinator.GetComponent<TransformComponent>(*iNextEntity);

				// Skip asleep or static colliders.
				if (nextBoxCollider.GetCollisionMode() == CollisionMode::e_asleep)
					continue;

				if (currRigidbody.getIsStatic() && nextRigidbody.getIsStatic())
					continue;

				// Test for AABBs vs AABBs
				if (CollisionManager::instance().DiscreteAABBvsAABB(currBoxCollider.m_aabb, nextBoxCollider.m_aabb))
				{
					//CollisionManager::instance().InsertColliderPair(*iEntity, *iNextEntity);

					// If A and/or B is/are a trigger(s), dispatch trigger event(s).
					if (currBoxCollider.GetCollisionMode() == CollisionMode::e_trigger)
					{
						EntTriggerEnterEvent* ev = new EntTriggerEnterEvent{ *iEntity, *iNextEntity };
						ev->SetSystemReceivers((int)SystemID::id_LOGICSYSTEM);
						EventDispatcher::instance().AddEvent(ev);

						continue;
					}
					if (nextBoxCollider.GetCollisionMode() == CollisionMode::e_trigger)
					{
						EntTriggerEnterEvent* ev = new EntTriggerEnterEvent{ *iNextEntity, *iEntity };
						ev->SetSystemReceivers((int)SystemID::id_LOGICSYSTEM);
						EventDispatcher::instance().AddEvent(ev);

						continue;
					}

					if (g_engine.m_coordinator.GetHierarchyInfo(*iEntity).m_tag == "Player" && g_engine.m_coordinator.GetHierarchyInfo(*iNextEntity).m_tag == "Ground"
						|| g_engine.m_coordinator.GetHierarchyInfo(*iNextEntity).m_tag == "Player" && g_engine.m_coordinator.GetHierarchyInfo(*iEntity).m_tag == "Ground")
					{
						EntCollisionStayEvent* ev =
							new EntCollisionStayEvent
						{
							*iEntity, *iNextEntity,
							currTransform.GetPosition(), nextTransform.GetPosition(),
							CollisionManager::instance().GetColliderScale(currBoxCollider.m_aabb, currTransform),
							CollisionManager::instance().GetColliderScale(nextBoxCollider.m_aabb, nextTransform)
						};

						ev->SetSystemReceivers((int)SystemID::id_PLAYERCONTROLLERSYSTEM);
						EventDispatcher::instance().AddEvent(ev);
					}


					CollisionManager::instance().GenerateManifoldAABBvsAABB(*iEntity, *iNextEntity);
				}

				// Test OBBs vs OBBs collision
				//if (CollisionManager::instance().DiscreteOBBvsOBB(currBoxCollider.m_obb, nextBoxCollider.m_obb))
				//{
				//	std::cout << "Entity " << *iEntity << " OBB collides with Entity " << *iNextEntity << " OBB" << std::endl;
				//}

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

		g_engine.m_coordinator.EndTimeSystem("Box Collision System");
	}

	void BoxCollisionSystem::Receive(Event* ev)
	{}

	void BoxCollisionSystem::Shutdown()
	{}
}