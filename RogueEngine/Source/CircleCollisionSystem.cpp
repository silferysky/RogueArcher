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
		g_engine.m_coordinator.InitTimeSystem("Circle Collision System");

		std::set<Entity>::iterator iEntity;
		for (iEntity = m_entities.begin(); iEntity != m_entities.end(); ++iEntity)
		{
			auto& currRigidbody = g_engine.m_coordinator.GetComponent<RigidbodyComponent>(*iEntity);
			auto& currTransform = g_engine.m_coordinator.GetComponent<TransformComponent>(*iEntity);
			auto& currBoundingCircle = g_engine.m_coordinator.GetComponent<CircleCollider2DComponent>(*iEntity);

			// Colliders updated.
			CollisionManager::instance().UpdateBoundingCircle(currBoundingCircle.m_collider, currTransform);

			// Spatial partitioning conducted in main collision system.

			// Test Circle Collisions
			std::set<Entity>::iterator iNextEntity = iEntity;

			for (iNextEntity++; iNextEntity != m_entities.end(); ++iNextEntity)
			{
				auto& nextBoundingCircle = g_engine.m_coordinator.GetComponent<CircleCollider2DComponent>(*iNextEntity);
				auto& nextRigidbody = g_engine.m_coordinator.GetComponent<RigidbodyComponent>(*iNextEntity);
				auto& nextTransform = g_engine.m_coordinator.GetComponent<TransformComponent>(*iNextEntity);

				// Update collidables
				CollisionManager::instance().UpdateBoundingCircle(nextBoundingCircle.m_collider, nextTransform);

				if (currRigidbody.getIsStatic() && nextRigidbody.getIsStatic())
					continue;

				if (CollisionManager::instance().DiscreteCircleVsCircle(currBoundingCircle.m_collider, nextBoundingCircle.m_collider))
				{
					CollisionManager::instance().GenerateManifoldCirclevsCircle(*iEntity, *iNextEntity);
				}
			}

			// Collision Response (Contact, forces, rest, Impulse, Torque)
			CollisionManager::instance().ResolveManifolds();
		}

		g_engine.m_coordinator.EndTimeSystem("Circle Collision System");
	}

	void CircleCollisionSystem::Receive(Event* ev)
	{}

	void CircleCollisionSystem::Shutdown()
	{}
}