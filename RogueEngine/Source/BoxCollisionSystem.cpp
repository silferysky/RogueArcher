#include "pch.h"
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
		for (iEntity = m_entities.begin(); iEntity != m_entities.end(); ++iEntity)
		{
			auto& currRigidbody = g_engine.m_coordinator.GetComponent<RigidbodyComponent>(*iEntity);
			auto& currTransform = g_engine.m_coordinator.GetComponent<TransformComponent>(*iEntity);
			auto& currBoxCollider = g_engine.m_coordinator.GetComponent<BoxCollider2DComponent>(*iEntity);

			// Skip asleep colliders
			if (currBoxCollider.GetCollisionMode() == CollisionMode::e_asleep)
				continue;

			// Update collidables
			CollisionManager::instance().UpdateAABB(currBoxCollider.m_aabb, currTransform);
			CollisionManager::instance().UpdateOBB(currBoxCollider.m_obb, currTransform);

			// Conduct spatial partitioning

			// Test AABB/OBB Collision
			std::set<Entity>::iterator iNextEntity = iEntity;

			for (iNextEntity++; iNextEntity != m_entities.end(); ++iNextEntity)
			{
				auto& nextBoxCollider = g_engine.m_coordinator.GetComponent<BoxCollider2DComponent>(*iNextEntity);
				auto& nextRigidbody = g_engine.m_coordinator.GetComponent<RigidbodyComponent>(*iNextEntity);

				if (nextBoxCollider.GetCollisionMode() == CollisionMode::e_asleep)
					continue;

				if (currRigidbody.getIsStatic() && nextRigidbody.getIsStatic())
					continue;

				if (CollisionManager::instance().DiscreteAABBvsAABB(currBoxCollider.m_aabb, nextBoxCollider.m_aabb))
				{
					// If A or B is a trigger, dispatch trigger event.
					if (currBoxCollider.GetCollisionMode() == CollisionMode::e_trigger)
					{
						Event* ev = new EntTriggeredEvent{ *iNextEntity, *iEntity };
						ev->SetSystemReceivers((int)SystemID::id_LOGICSYSTEM);
						EventDispatcher::instance().AddEvent(ev);

						continue;
					}
					if (nextBoxCollider.GetCollisionMode() == CollisionMode::e_trigger)
					{
						Event* ev = new EntTriggeredEvent{ *iEntity, *iNextEntity };
						ev->SetSystemReceivers((int)SystemID::id_LOGICSYSTEM);
						EventDispatcher::instance().AddEvent(ev);

						continue;
					}
					//std::cout << "Entity " << *iEntity << " AABB collides with Entity " << *iNextEntity << " AABB" << std::endl;
					CollisionManager::instance().GenerateManifolds(*iEntity, *iNextEntity);
				}

				//if (CollisionManager::instance().DiscreteOBBvsOBB(currBoxCollider.m_obb, nextBoxCollider.m_obb))
				//{
				//	std::cout << "Entity " << *iEntity << " OBB collides with Entity " << *iNextEntity << " OBB" << std::endl;
				//}

			}

			// Collision Response (Contact, forces, rest, Impulse, Torque)
			CollisionManager::instance().ResolveManifolds();
		}

		g_engine.m_coordinator.EndTimeSystem("Box Collision System");
	}

	void BoxCollisionSystem::Receive(Event* ev)
	{}

	void BoxCollisionSystem::Shutdown()
	{}
}