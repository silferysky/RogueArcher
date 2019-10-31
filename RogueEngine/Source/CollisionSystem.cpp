#include "CollisionSystem.h"
#include "SystemManager.h"
#include "EventListener.h"
#include "Main.h"

namespace Rogue
{
	CollisionManager CollisionSystem::CollisionSystem::s_collisionManager; // Static object for all collision systems to share

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
		signature.set(g_engine.m_coordinator.GetComponentType<BoxCollider2DComponent>());
		signature.set(g_engine.m_coordinator.GetComponentType<CircleCollider2DComponent>());

		// Set Collision system signature.
		g_engine.m_coordinator.SetSystemSignature<CollisionSystem>(signature);
	}

	void CollisionSystem::Update()
	{
		g_engine.m_coordinator.InitTimeSystem("Collision System");

		std::set<Entity>::iterator iEntity;
		for (iEntity = m_entities.begin(); iEntity != m_entities.end(); ++iEntity)
		{
			auto& currRigidbody = g_engine.m_coordinator.GetComponent<RigidbodyComponent>(*iEntity);
			auto& currTransform = g_engine.m_coordinator.GetComponent<TransformComponent>(*iEntity);
			auto& currBoxCollider = g_engine.m_coordinator.GetComponent<BoxCollider2DComponent>(*iEntity);
			auto& currCircleCollider = g_engine.m_coordinator.GetComponent<CircleCollider2DComponent>(*iEntity);

			
			// Conduct spatial partitioning

			// Test AABB/OBB Collision
			std::set<Entity>::iterator iNextEntity = iEntity;


			//for (iNextEntity++; iNextEntity != m_entities.end(); ++iNextEntity)
			//{
			//	auto& nextBoxCollider = g_engine.m_coordinator.GetComponent<BoxCollider2DComponent>(*iNextEntity);
			//	auto& nextCircleCollider = g_engine.m_coordinator.GetComponent<CircleCollider2DComponent>(*iNextEntity);
			//	auto& nextRigidbody = g_engine.m_coordinator.GetComponent<RigidbodyComponent>(*iNextEntity);
			//	auto& nextTransform = g_engine.m_coordinator.GetComponent<TransformComponent>(*iNextEntity);

			//	if (currRigidbody.getIsStatic() && nextRigidbody.getIsStatic())
			//		continue;

			//	if (s_collisionManager.DiscreteAABBVsCircle(currBoxCollider.m_aabb, nextCircleCollider.m_collider))// To check for AABB vs Circle
			//	{
			//		std::cout << "True\n\n" << std::endl;
			//		s_collisionManager.GenerateManifoldAABBvsCircle(*iEntity, *iNextEntity);
			//	}
			//	if (s_collisionManager.DiscreteCircleVsAABB(currCircleCollider.m_collider, currBoxCollider.m_aabb))
			//	{
			//		std::cout << "True" << std::endl;
			//		s_collisionManager.GenerateManifoldCirclevsAABB(*iNextEntity, *iEntity);
			//	}
			//}

			// Collision Response (Contact, forces, rest, Impulse, Torque)
			s_collisionManager.ResolveManifolds();

		}
		g_engine.m_coordinator.EndTimeSystem("Collision System");
	}

	void CollisionSystem::Receive(Event* ev)
	{}

	void CollisionSystem::Shutdown()
	{}
}