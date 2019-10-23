#include "CollisionSystem.h"
#include "SystemManager.h"
#include "EventListener.h"
#include "Main.h"

namespace Rogue
{
	CollisionSystem::CollisionSystem()
		: System(SystemID::id_COLLISIONSYSTEM)
	{}

	void CollisionSystem::init()
	{
		REGISTER_LISTENER(SystemID::id_COLLISIONSYSTEM, CollisionSystem::receive);

		// Add components to signature.
		Signature signature;
		signature.set(gEngine.m_coordinator.GetComponentType<RigidbodyComponent>());
		signature.set(gEngine.m_coordinator.GetComponentType<TransformComponent>());
		signature.set(gEngine.m_coordinator.GetComponentType<BoxCollider2DComponent>());
		//signature.set(gEngine.m_coordinator.GetComponentType<CircleCollider2DComponent>());

		// Set Collision system signature.
		gEngine.m_coordinator.SetSystemSignature<CollisionSystem>(signature);
	}

	void CollisionSystem::update()
	{
		for (int step = 0; step < gEngine.GetStepCount(); ++step)
		{
			std::set<Entity>::iterator iEntity;
			for (iEntity = m_entities.begin()++; iEntity != m_entities.end(); ++iEntity)
			{
				auto& rigidbody = gEngine.m_coordinator.GetComponent<RigidbodyComponent>(*iEntity);
				auto& transform = gEngine.m_coordinator.GetComponent<TransformComponent>(*iEntity);
				auto& currBoxCollider = gEngine.m_coordinator.GetComponent<BoxCollider2DComponent>(*iEntity);
				//	auto& circleCollider = gEngine.m_coordinator.GetComponent<CircleCollider2DComponent>(*iEntity);

			// Update collidables
				m_colliderManager.updateAABB(currBoxCollider.m_aabb, transform);
				m_colliderManager.updateOBB(currBoxCollider.m_obb, transform);

				// Conduct spatial partitioning

				// Test AABB/OBB Collision
				std::set<Entity>::iterator iNextEntity = iEntity;

				for (iNextEntity++; iNextEntity != m_entities.end(); ++iNextEntity)
				{
					auto& nextBoxCollider = gEngine.m_coordinator.GetComponent<BoxCollider2DComponent>(*iNextEntity);
					auto& nextRigidbody = gEngine.m_coordinator.GetComponent<RigidbodyComponent>(*iNextEntity);

					if (m_colliderManager.staticAABBvsAABB(currBoxCollider.m_aabb, nextBoxCollider.m_aabb))
					{
					//	std::cout << "Entity " << *iEntity << " AABB collides with Entity " << *iNextEntity << " AABB" << std::endl;

						m_colliderManager.InsertColliderPair(*iEntity, *iNextEntity);
					}

					if (m_colliderManager.staticOBBvsOBB(currBoxCollider.m_obb, nextBoxCollider.m_obb))
					{
					//	std::cout << "Entity " << *iEntity << " OBB collides with Entity " << *iNextEntity << " OBB" << std::endl;
					}

				//	bool a = rigidbody.getIsStatic();
					//bool b = nextRigidbody.getIsStatic();
				}

				m_colliderManager.GenerateManifolds();

				// Collision Response (Contact, forces, etc)
				m_colliderManager.ResolveManifolds();

				// Rest, Impulse, Torque
			}
		}
	}

	void CollisionSystem::receive(Event* ev)
	{
	}
}