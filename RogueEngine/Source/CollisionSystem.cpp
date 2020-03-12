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
		for (iEntity = m_entities.begin(); iEntity != m_entities.end(); ++iEntity)
		{
			auto& currRigidbody = g_engine.m_coordinator.GetComponent<RigidbodyComponent>(*iEntity);
			auto& currTransform = g_engine.m_coordinator.GetComponent<TransformComponent>(*iEntity);
			auto& currCollider = g_engine.m_coordinator.GetComponent<ColliderComponent>(*iEntity);

			Shape::Type currColliderType = Shape::Type::e_none;
			Shape::Type nextColliderType = Shape::Type::e_none;

			currColliderType = currCollider.GetShape()->GetType();
			
			// Conduct spatial partitioning

			// Test AABB/OBB Collision
			std::set<Entity>::iterator iNextEntity = iEntity;

			for (iNextEntity++; iNextEntity != m_entities.end(); ++iNextEntity)
			{
				RigidbodyComponent& nextRigidbody = g_engine.m_coordinator.GetComponent<RigidbodyComponent>(*iNextEntity);

				if (currRigidbody.getIsStatic() && nextRigidbody.getIsStatic())
				{
					continue;
				}

				ColliderComponent& nextCollider = g_engine.m_coordinator.GetComponent<ColliderComponent>(*iNextEntity);

				if (!CollisionManager::instance().FilterColliders(currCollider.GetCollisionMask(), nextCollider.GetCollisionCat()) ||
					!CollisionManager::instance().FilterColliders(currCollider.GetCollisionCat(), nextCollider.GetCollisionMask()))
				{
					continue;
				}

				TransformComponent& nextTransform = g_engine.m_coordinator.GetComponent<TransformComponent>(*iNextEntity);

				nextColliderType = nextCollider.GetShape()->GetType();

				if (currColliderType == Shape::Type::e_circle && nextColliderType == Shape::Type::e_box)
				{
					auto& circleA = g_engine.m_coordinator.GetComponent<CircleCollider2DComponent>(*iEntity);
					auto& boxB = g_engine.m_coordinator.GetComponent<BoxCollider2DComponent>(*iNextEntity);

					if (CollisionManager::instance().DiscreteCircleVsAABB(circleA.m_collider, boxB.m_aabb))
					{
						CollisionInfo<CircleCollider2DComponent> infoA(*iEntity, circleA, currRigidbody, currTransform);
						CollisionInfo<BoxCollider2DComponent> infoB(*iNextEntity, boxB, nextRigidbody, nextTransform);

						// Set booleans for both colliders/triggers
						circleA.SetIsCollided(true);
						boxB.SetIsCollided(true);

						if (CollisionManager::instance().InsertBoxPair(*iEntity, *iNextEntity))
						{
							CollisionManager::instance().SendEnterEvents(infoA, infoB);
						}
						else
						{
							CollisionManager::instance().SendStayEvents(infoA, infoB);
						}

						// If at least one of them is a trigger, skip resolution
						if (circleA.GetCollisionMode() == CollisionMode::e_trigger || boxB.GetCollisionMode() == CollisionMode::e_trigger)
							continue;

						// Generate manifolds from collided pairs
						CollisionManager::instance().GenerateManifoldCirclevsAABB(*iEntity, *iNextEntity);
					}
				}
				else if (currColliderType == Shape::Type::e_box && nextColliderType == Shape::Type::e_circle)
				{
					auto& boxA = g_engine.m_coordinator.GetComponent<BoxCollider2DComponent>(*iEntity);
					auto& circleB = g_engine.m_coordinator.GetComponent<CircleCollider2DComponent>(*iNextEntity);

					if (CollisionManager::instance().DiscreteAABBVsCircle(boxA.m_aabb, circleB.m_collider))
					{
						CollisionInfo<BoxCollider2DComponent> infoA(*iEntity, boxA, currRigidbody, currTransform);
						CollisionInfo<CircleCollider2DComponent> infoB(*iNextEntity, circleB, nextRigidbody, nextTransform);

						// Set booleans for both colliders/triggers
						boxA.SetIsCollided(true);
						circleB.SetIsCollided(true);

						if (CollisionManager::instance().InsertBoxPair(*iEntity, *iNextEntity))
						{
							CollisionManager::instance().SendEnterEvents(infoA, infoB);
						}
						else
						{
							CollisionManager::instance().SendStayEvents(infoA, infoB);
						}

						// If at least one of them is a trigger, skip resolution
						if (boxA.GetCollisionMode() == CollisionMode::e_trigger || circleB.GetCollisionMode() == CollisionMode::e_trigger)
							continue;

						// Generate manifolds from collided pairs
						CollisionManager::instance().GenerateManifoldAABBvsCircle(*iEntity, *iNextEntity);
					}
				}
			}

			// Remove exiting pairs and send exit events
			//CollisionManager::instance().CheckExitingCollidedPairs<CircleCollider2DComponent, BoxCollider2DComponent>(m_entities);

			// Collision Response (Contact, forces, rest, Impulse, Torque)
			CollisionManager::instance().ResolveManifolds();
		}
	}

	void CollisionSystem::Receive(Event& ev)
	{
	}

	void CollisionSystem::Shutdown()
	{}
}