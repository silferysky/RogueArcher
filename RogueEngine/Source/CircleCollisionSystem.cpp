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
			auto& currCircleCollider = g_engine.m_coordinator.GetComponent<CircleCollider2DComponent>(*iEntity);

			// Colliders updated.
			CollisionSystem::s_collisionManager.UpdateCircleCollider(currCircleCollider.m_collider, currTransform);

			// Spatial partitioning conducted in main collision system.

			// Test Circle Collisions
			std::set<Entity>::iterator iNextEntity = iEntity;


			for (iNextEntity++; iNextEntity != m_entities.end(); ++iNextEntity)
			{
				auto& nextCircleCollider = g_engine.m_coordinator.GetComponent<CircleCollider2DComponent>(*iNextEntity);
				auto& nextRigidbody = g_engine.m_coordinator.GetComponent<RigidbodyComponent>(*iNextEntity);
				auto& nextTransform = g_engine.m_coordinator.GetComponent<TransformComponent>(*iNextEntity);

				if (currRigidbody.getIsStatic() && nextRigidbody.getIsStatic())
					continue;

				//if (CollisionSystem::s_collisionManager.DiscreteCircleVsCircle(currCircleCollider.m_collider, nextCircleCollider.m_collider,
				//	currTransform, nextTransform))
				//{
				//	CollisionSystem::s_collisionManager.GenerateManifoldCirclevsCircle(*iEntity, *iNextEntity);
				//}

				//if (CollisionSystem::s_collisionManager.DiscreteAABBVsCircle(currBoxCollider.m_aabb, nextCircleCollider.m_collider))// To check for AABB vs Circle
				//{
				//	std::cout << "True\n\n" << std::endl;
				//	CollisionSystem::s_collisionManager.GenerateManifoldAABBvsCircle(*iEntity, *iNextEntity);
				//}
				//if (CollisionSystem::s_collisionManager.DiscreteCircleVsAABB(currCircleCollider.m_collider, currBoxCollider.m_aabb))
				//{
				//	std::cout << "True" << std::endl;
				//	CollisionSystem::s_collisionManager.GenerateManifoldCirclevsAABB(*iNextEntity, *iEntity);
				//}
			}

			// Collision Response (Contact, forces, rest, Impulse, Torque)
			CollisionSystem::s_collisionManager.ResolveManifolds();
		}

		g_engine.m_coordinator.EndTimeSystem("Circle Collision System");
	}

	void CircleCollisionSystem::Receive(Event* ev)
	{}

	void CircleCollisionSystem::Shutdown()
	{}
}