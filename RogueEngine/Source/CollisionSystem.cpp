#include "CollisionSystem.h"
#include "SystemManager.h"
#include "EventListener.h"
#include "Main.h"

CollisionSystem::CollisionSystem()
	: System(SystemID::id_COLLISIONSYSTEM)
{}

void CollisionSystem::init()
{
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
	REGISTER_LISTENER(SystemID::id_COLLISIONSYSTEM, CollisionSystem::receive);

	std::set<Entity>::iterator iEntity;
	for (iEntity = m_entities.begin()++; iEntity != m_entities.end(); ++iEntity)
	{
		auto& rigidbody = gEngine.m_coordinator.GetComponent<RigidbodyComponent>(*iEntity);

		//// Skip any static rigidbodies.
		//if (rigidbody.getIsStatic())
		//{
		//	continue;
		//}
		//		std::cout << "Entity " << *iEntity << std::endl;
		auto& transform = gEngine.m_coordinator.GetComponent<TransformComponent>(*iEntity);
		auto& currBoxCollider = gEngine.m_coordinator.GetComponent<BoxCollider2DComponent>(*iEntity);
		//	auto& circleCollider = gEngine.m_coordinator.GetComponent<CircleCollider2DComponent>(*iEntity);

	// Update collidables
		m_colliderManager.updateAABB(currBoxCollider.AABB(), transform);
		m_colliderManager.updateOBB(currBoxCollider.OBB(), transform);

		// Conduct spatial partitioning

		// Test AABB/OBB Collision
		std::set<Entity>::iterator iNextEntity = iEntity;

		for (iNextEntity++; iNextEntity != m_entities.end(); ++iNextEntity)
		{
			auto& nextBoxCollider = gEngine.m_coordinator.GetComponent<BoxCollider2DComponent>(*iNextEntity);
			auto& currRigidbody = gEngine.m_coordinator.GetComponent<RigidbodyComponent>(*iEntity);
			auto& nextRigidbody = gEngine.m_coordinator.GetComponent<RigidbodyComponent>(*iNextEntity);
			auto& currTransform = gEngine.m_coordinator.GetComponent<TransformComponent>(*iEntity);

			if (m_colliderManager.staticAABBvsAABB(currBoxCollider.AABB(), nextBoxCollider.AABB()))
			{
				//	if (checkAABB)
				std::cout << "Entity " << *iEntity << " AABB collides with Entity " << *iNextEntity << " AABB" << std::endl;

				if (nextRigidbody.getIsStatic())
				{
					currTransform.offSetPosition(Vec2(currRigidbody.getVelocity().x * gFixedDeltaTime, -currRigidbody.getVelocity().y) * gFixedDeltaTime);
					currRigidbody.addForce(Vec2(0.0f, 5.0f));
				}

				//	checkAABB = false;
			}
			if (m_colliderManager.staticOBBvsOBB(currBoxCollider.OBB(), nextBoxCollider.OBB()))
			{
				//	if (checkOBB)
				std::cout << "Entity " << *iEntity << " OBB collides with Entity " << *iNextEntity << " OBB" << std::endl;
				//	checkOBB = false;
			}
		}

		// Collision Response (Contact, forces, etc)
		// Rest, Impulse, Torque
	}
}

void CollisionSystem::receive(Event* ev)
{
}
