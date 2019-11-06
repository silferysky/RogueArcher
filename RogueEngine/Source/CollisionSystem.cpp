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
		signature.set(g_engine.m_coordinator.GetComponentType<ColliderComponent>());

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
			auto& currCollider = g_engine.m_coordinator.GetComponent<ColliderComponent>(*iEntity);

			Shape::Type currColliderType = Shape::Type::e_none;
			Shape::Type nextColliderType = Shape::Type::e_none;

			if (currCollider.GetShape()->GetType() == Shape::Type::e_circle)
			{
				currColliderType = Shape::Type::e_circle;
			}
			else if (currCollider.GetShape()->GetType() == Shape::Type::e_box)
			{
				currColliderType = Shape::Type::e_box;
			}
			
			// Conduct spatial partitioning

			// Test AABB/OBB Collision
			std::set<Entity>::iterator iNextEntity = iEntity;

			for (iNextEntity++; iNextEntity != m_entities.end(); ++iNextEntity)
			{
				auto& nextRigidbody = g_engine.m_coordinator.GetComponent<RigidbodyComponent>(*iNextEntity);

				if (currRigidbody.getIsStatic() && nextRigidbody.getIsStatic())
					continue;

				auto& nextCollider = g_engine.m_coordinator.GetComponent<ColliderComponent>(*iNextEntity);
				auto& nextTransform = g_engine.m_coordinator.GetComponent<TransformComponent>(*iNextEntity);

				if (nextCollider.GetShape()->GetType() == Shape::Type::e_circle)
				{
					nextColliderType = Shape::Type::e_circle;
				}
				else if (nextCollider.GetShape()->GetType() == Shape::Type::e_box)
				{
					nextColliderType = Shape::Type::e_box;
				}

				if (currColliderType == Shape::Type::e_circle && nextColliderType == Shape::Type::e_box)
				{
					auto& circleA = g_engine.m_coordinator.GetComponent<CircleCollider2DComponent>(*iEntity);
					auto& boxB = g_engine.m_coordinator.GetComponent<BoxCollider2DComponent>(*iNextEntity);
					
					if (s_collisionManager.DiscreteCircleVsAABB(circleA.m_collider, boxB.m_aabb))
						s_collisionManager.GenerateManifoldCirclevsAABB(*iEntity, *iNextEntity);
				}
				else if (currColliderType == Shape::Type::e_box && nextColliderType == Shape::Type::e_circle)
				{
					auto& boxA = g_engine.m_coordinator.GetComponent<BoxCollider2DComponent>(*iEntity);
					auto& circleB = g_engine.m_coordinator.GetComponent<CircleCollider2DComponent>(*iNextEntity);

					if (s_collisionManager.DiscreteAABBVsCircle(boxA.m_aabb, circleB.m_collider))
						s_collisionManager.GenerateManifoldAABBvsCircle(*iEntity, *iNextEntity);
				}
			}

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