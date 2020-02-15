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
		for (iEntity = m_entities.begin(); iEntity != m_entities.end(); ++iEntity)
		{
			auto& currTransform = g_engine.m_coordinator.GetComponent<TransformComponent>(*iEntity);
			auto& currBoundingCircle = g_engine.m_coordinator.GetComponent<CircleCollider2DComponent>(*iEntity);

			// Colliders updated.
			CollisionManager::instance().UpdateBoundingCircle(currBoundingCircle.m_collider, currTransform);

			// Spatial partitioning conducted in main collision system.
		}

		for (iEntity = m_entities.begin(); iEntity != m_entities.end(); ++iEntity)
		{
			auto& currCollider = g_engine.m_coordinator.GetComponent<ColliderComponent>(*iEntity);
			auto& currRigidbody = g_engine.m_coordinator.GetComponent<RigidbodyComponent>(*iEntity);
			auto& currTransform = g_engine.m_coordinator.GetComponent<TransformComponent>(*iEntity);
			auto& currBoundingCircle = g_engine.m_coordinator.GetComponent<CircleCollider2DComponent>(*iEntity);
		
			// Test Circle Collisions
			std::set<Entity>::iterator iNextEntity = iEntity;

			for (iNextEntity++; iNextEntity != m_entities.end(); ++iNextEntity)
			{
				auto& nextCollider = g_engine.m_coordinator.GetComponent<ColliderComponent>(*iNextEntity);

				// Filter colliders
				if (!CollisionManager::instance().FilterColliders(currCollider.GetCollisionMask(), nextCollider.GetCollisionCat()) ||
					!CollisionManager::instance().FilterColliders(currCollider.GetCollisionCat(), nextCollider.GetCollisionMask()))
					continue;

				auto& nextRigidbody = g_engine.m_coordinator.GetComponent<RigidbodyComponent>(*iNextEntity);

				if (currRigidbody.getIsStatic() && nextRigidbody.getIsStatic())
					continue;

				auto& nextTransform = g_engine.m_coordinator.GetComponent<TransformComponent>(*iNextEntity);
				auto& nextBoundingCircle = g_engine.m_coordinator.GetComponent<CircleCollider2DComponent>(*iNextEntity);

				if (CollisionManager::instance().DiscreteCircleVsCircle(currBoundingCircle.m_collider, nextBoundingCircle.m_collider))
				{
					// If A and/or B is/are a trigger(s), dispatch trigger event(s).
					/*if (currBoundingCircle.GetCollisionMode() == CollisionMode::e_trigger)
					{
						EntTriggerEnterEvent* ev = new EntTriggerEnterEvent{ *iEntity, *iNextEntity };
						ev->SetSystemReceivers((int)SystemID::id_LOGICSYSTEM);
						EventDispatcher::instance().AddEvent(ev);

						continue;
					}
					if (nextBoundingCircle.GetCollisionMode() == CollisionMode::e_trigger)
					{
						EntTriggerEnterEvent* ev = new EntTriggerEnterEvent{ *iNextEntity, *iEntity };
						ev->SetSystemReceivers((int)SystemID::id_LOGICSYSTEM);
						EventDispatcher::instance().AddEvent(ev);

						continue;
					}*/
					CollisionManager::instance().InsertCirclePair(*iEntity, *iNextEntity);
				}
			}

			// Generate manifolds from collided pairs
			CollisionManager::instance().GenerateCircleManifolds();

			// Collision Response (Contact, forces, rest, Impulse, Torque)
			CollisionManager::instance().ResolveManifolds();
		}
	}

	void CircleCollisionSystem::Receive(Event* ev)
	{}

	void CircleCollisionSystem::Shutdown()
	{}
}