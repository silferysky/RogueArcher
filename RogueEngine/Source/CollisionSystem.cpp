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

		// Set Collision system signature.
		g_engine.m_coordinator.SetSystemSignature<CollisionSystem>(signature);
	}

	void CollisionSystem::Update()
	{
		g_engine.m_coordinator.InitTimeSystem("Collision System");
		for (int step = 0; step < g_engine.GetStepCount(); ++step)
		{
			std::set<Entity>::iterator iEntity;
			for (iEntity = m_entities.begin(); iEntity != m_entities.end(); ++iEntity)
			{
				auto& currRigidbody = g_engine.m_coordinator.GetComponent<RigidbodyComponent>(*iEntity);
				auto& currTransform = g_engine.m_coordinator.GetComponent<TransformComponent>(*iEntity);
				
				BoxCollider2DComponent* currBoxCollider = nullptr;
				CircleCollider2DComponent* currCircleCollider = nullptr;
				
				if (g_engine.m_coordinator.ComponentExists<BoxCollider2DComponent>(*iEntity))
					currBoxCollider = &g_engine.m_coordinator.GetComponent<BoxCollider2DComponent>(*iEntity);

				if(g_engine.m_coordinator.ComponentExists<CircleCollider2DComponent>(*iEntity))
					currCircleCollider = &g_engine.m_coordinator.GetComponent<CircleCollider2DComponent>(*iEntity);

				// Update collidables
				if (currBoxCollider)
				{
					m_colliderManager.UpdateAABB(currBoxCollider->m_aabb, currTransform);
					m_colliderManager.UpdateOBB(currBoxCollider->m_obb, currTransform);
				}
				// Conduct spatial partitioning

				// Test AABB/OBB Collision
				std::set<Entity>::iterator iNextEntity = iEntity;

				BoxCollider2DComponent* nextBoxCollider = nullptr;
				CircleCollider2DComponent* nextCircleCollider = nullptr;

				for (iNextEntity++; iNextEntity != m_entities.end(); ++iNextEntity)
				{
					auto& nextRigidbody = g_engine.m_coordinator.GetComponent<RigidbodyComponent>(*iNextEntity);
					
					if (currRigidbody.getIsStatic() && nextRigidbody.getIsStatic())
						continue;

					auto& nextTransform = g_engine.m_coordinator.GetComponent<TransformComponent>(*iNextEntity);
					if (g_engine.m_coordinator.ComponentExists<BoxCollider2DComponent>(*iNextEntity))
						nextBoxCollider = &g_engine.m_coordinator.GetComponent<BoxCollider2DComponent>(*iNextEntity);

					if (g_engine.m_coordinator.ComponentExists<CircleCollider2DComponent>(*iNextEntity))
						nextCircleCollider = &g_engine.m_coordinator.GetComponent<CircleCollider2DComponent>(*iNextEntity);


					if (currCircleCollider && nextCircleCollider)
					{
						if (m_colliderManager.DiscreteCircleVsCircle(currCircleCollider->m_collider, nextCircleCollider->m_collider,
							currTransform, nextTransform))
						{
							m_colliderManager.GenerateManifoldCirclevsCircle(*iEntity, *iNextEntity);
						}
					}

					if (currBoxCollider && nextBoxCollider) // If boxcollider exists for both entities
					{
						if (m_colliderManager.DiscreteAABBvsAABB(currBoxCollider->m_aabb, nextBoxCollider->m_aabb))
						{
							//	std::cout << "Entity " << *iEntity << " AABB collides with Entity " << *iNextEntity << " AABB" << std::endl;
							m_colliderManager.GenerateManifoldAABBvsAABB(*iEntity, *iNextEntity);
						}

						if (m_colliderManager.DiscreteOBBvsOBB(currBoxCollider->m_obb, nextBoxCollider->m_obb))
						{
							//	std::cout << "Entity " << *iEntity << " OBB collides with Entity " << *iNextEntity << " OBB" << std::endl;
						}
					}
				}

				// Collision Response (Contact, forces, etc)
				m_colliderManager.ResolveManifolds();

				// Rest, Impulse, Torque
			}
		}
		g_engine.m_coordinator.EndTimeSystem("Collision System");
	}

	void CollisionSystem::Receive(Event* ev)
	{}

	void CollisionSystem::Shutdown()
	{}
}