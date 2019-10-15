#include "PhysicsSystem.h"
#include "Main.h"
#include "REMath.h"
#include "EventListener.h"
#include "ComponentList.h"
#include "Timer.h"
#include "Vector2D.h"
#include "SystemManager.h"
#include "KeyEvent.h"
#include "EventDispatcher.h"
#include "Logger.h"

//-------------------------------------------------------//
//              PRIVATE MEMBER FUNCTIONS								 //
//-------------------------------------------------------//
void PhysicsSystem::applyForces(RigidbodyComponent& rigidbody) // F = ma
{
	if(allowGravity)
		rigidbody.setAcceleration(rigidbody.getAccForce() * rigidbody.getInvMass() + m_gravity);
	else
		rigidbody.setAcceleration(rigidbody.getAccForce() * rigidbody.getInvMass());
}

PhysicsSystem::PhysicsSystem(Vec2 gravity)
	:	System(SystemID::id_PHYSICSSYSTEM),
		m_colliderManager{}, m_gravity{gravity},
	  checkAABB{ true }, checkOBB{ true }, allowGravity{ true }
{}

void PhysicsSystem::integrateAcceleration(RigidbodyComponent& rigidbody, TransformComponent& transform)
{
	transform.offSetPosition(rigidbody.getVelocity() * gFixedDeltaTime);

	Vec2 vel = rigidbody.getAcceleration() * gFixedDeltaTime;
	rigidbody.offSetVelocity(vel);
	rigidbody.setVelocity(rigidbody.getVelocity() * rigidbody.getDamping());

}


//-------------------------------------------------------//
//              PUBLIC MEMBER FUNCTIONS					 //
//-------------------------------------------------------//
void PhysicsSystem::init()
{
	LISTENER_HANDLER hand = std::bind(&PhysicsSystem::receive, this, std::placeholders::_1);
	EventDispatcher::instance().AddListener(SystemID::id_PHYSICSSYSTEM, hand);

	// Add components to signature.
	Signature signature;
	signature.set(gEngine.m_coordinator.GetComponentType<RigidbodyComponent>());
	signature.set(gEngine.m_coordinator.GetComponentType<TransformComponent>());
	signature.set(gEngine.m_coordinator.GetComponentType<BoxCollider2DComponent>());
	//signature.set(gEngine.m_coordinator.GetComponentType<CircleCollider2DComponent>());
	
	// Set physics system signature.
	gEngine.m_coordinator.SetSystemSignature<PhysicsSystem>(signature);
	m_gravity = { 0.0f, -1.0f };
}

void PhysicsSystem::update()
{
//	RE_CORE_INFO("Running Update");

	Timer TimerSystem;
	TimerSystem.TimerInit("Physics System");

	for (int step = 0; step < gEngine.GetStepCount(); ++step)
	{
		// For all entities
		std::set<Entity>::iterator iEntity;
		for (iEntity = m_entities.begin(); iEntity != m_entities.end(); ++iEntity)
		{
			auto& rigidbody = gEngine.m_coordinator.GetComponent<RigidbodyComponent>(*iEntity);

			// Skip any static rigidbodies.
			if (rigidbody.getIsStatic())
				continue;

			//		std::cout << "Entity " << *iEntity << std::endl;
			auto& transform = gEngine.m_coordinator.GetComponent<TransformComponent>(*iEntity);
			auto& currBoxCollider = gEngine.m_coordinator.GetComponent<BoxCollider2DComponent>(*iEntity);
			//	auto& circleCollider = gEngine.m_coordinator.GetComponent<CircleCollider2DComponent>(*iEntity);

			// Apply accForce (Forces are added if necessary)	
			applyForces(rigidbody);

			// Update positions
			integrateAcceleration(rigidbody, transform);

			// Update collidables
			m_colliderManager.updateAABB(currBoxCollider.AABB(), transform);
			m_colliderManager.updateOBB(currBoxCollider.OBB(), transform);

			// Conduct spatial partitioning

			// Test AABB/OBB Collision
			std::set<Entity>::iterator iNextEntity = iEntity;

			for (iNextEntity++; iNextEntity != m_entities.end(); ++iNextEntity)
			{
				auto& nextBoxCollider = gEngine.m_coordinator.GetComponent<BoxCollider2DComponent>(*iNextEntity);
				if (m_colliderManager.staticAABBvsAABB(currBoxCollider.AABB(), nextBoxCollider.AABB()))
				{
					if (checkAABB)
						std::cout << "Entity " << *iEntity << " AABB collides with Entity " << *iNextEntity << " AABB" << std::endl;
					checkAABB = false;
				}
				if (m_colliderManager.staticOBBvsOBB(currBoxCollider.OBB(), nextBoxCollider.OBB()))
				{
					if (checkOBB)
						std::cout << "Entity " << *iEntity << " OBB collides with Entity " << *iNextEntity << "OBB" << std::endl;
					checkOBB = false;
				}
			}

			// Collision Response (Contact, forces, etc)
			// Rest, Impulse, Torque

			// Reset accForce
			rigidbody.setAccForce(Vec2());

			//	std::cout << "Entity " << iEntity << "'s pos: " << transform.getPosition() << std::endl;
		}
	}

	TimerSystem.TimerEnd("Physics System");
}

void PhysicsSystem::receive(Event* ev)
{
	switch (ev->GetEventType())
	{
	case EventType::EvKeyPressed:
	{
		KeyPressEvent* EvPressKey = dynamic_cast<KeyPressEvent*>(ev);

		if (EvPressKey->GetKeyCode() == KeyPress::KeyA)
		{
			for (auto entity : m_entities)
			{
				if (entity == 1) // Entity A
				{
					auto& rigidbody = gEngine.m_coordinator.GetComponent<RigidbodyComponent>(entity);
					rigidbody.addForce(Vec2(-5.0f, 0.0f));
					RE_INFO("Move A Left!");
				}
			}
		}
		else if (EvPressKey->GetKeyCode() == KeyPress::KeyD)
		{
			for (auto entity : m_entities)
			{
				if (entity == 1) // Entity A
				{
					auto& rigidbody = gEngine.m_coordinator.GetComponent<RigidbodyComponent>(entity);
					rigidbody.addForce(Vec2(5.0f, 0.0f));
					RE_INFO("Move A Right!");
				}
			}
		}
		else if (EvPressKey->GetKeyCode() == KeyPress::KeyW)
		{
			for (auto entity : m_entities)
			{
				if (entity == 1) // Entity A
				{
					auto& rigidbody = gEngine.m_coordinator.GetComponent<RigidbodyComponent>(entity);
					rigidbody.addForce(Vec2(0.0f, 5.0f));
					RE_INFO("Move A Up!");
				}
			}
		}
		else if (EvPressKey->GetKeyCode() == KeyPress::KeyS)
		{
			for (auto entity : m_entities)
			{
				if (entity == 1) // Entity A
				{
					auto& rigidbody = gEngine.m_coordinator.GetComponent<RigidbodyComponent>(entity);
					rigidbody.addForce(Vec2(0.0f, -5.0f));
					RE_INFO("Move A Down!");
				}
			}
		}
		if (EvPressKey->GetKeyCode() == KeyPress::KeyArrowLeft)
		{
			for (auto entity : m_entities)
			{
				if (entity == 2) // Entity B
				{
					auto& rigidbody = gEngine.m_coordinator.GetComponent<RigidbodyComponent>(entity);
					rigidbody.addForce(Vec2(-5.0f, 0.0f));
					RE_INFO("Move B Left!");
				}
			}
		}
		else if (EvPressKey->GetKeyCode() == KeyPress::KeyArrowRight)
		{
			for (auto entity : m_entities)
			{
				if (entity == 2) // Entity B
				{
					auto& rigidbody = gEngine.m_coordinator.GetComponent<RigidbodyComponent>(entity);
					rigidbody.addForce(Vec2(5.0f, 0.0f));
					RE_INFO("Move B Right!");
				}
			}
		}
		else if (EvPressKey->GetKeyCode() == KeyPress::KeyArrowUp)
		{
			for (auto entity : m_entities)
			{
				if (entity == 2) // Entity B
				{
					auto& rigidbody = gEngine.m_coordinator.GetComponent<RigidbodyComponent>(entity);
					rigidbody.addForce(Vec2(0.0f, 5.0f));
					RE_INFO("Move B Up!");
				}
			}
		}
		else if (EvPressKey->GetKeyCode() == KeyPress::KeyArrowDown)
		{
			for (auto entity : m_entities)
			{
				if (entity == 2) // Entity B
				{
					auto& rigidbody = gEngine.m_coordinator.GetComponent<RigidbodyComponent>(entity);
					rigidbody.addForce(Vec2(0.0f, -5.0f));
					RE_INFO("Move B Down!");
				}
			}
		}
		else if (EvPressKey->GetKeyCode() == KeyPress::KeyE)
		{
			auto& transform = gEngine.m_coordinator.GetComponent<TransformComponent>((Entity)(m_entities.size() - 1));
			transform.offSetScale(Vec2(3.0f, 3.0f) * gFixedDeltaTime);
			RE_INFO("Scaled Up!");
		}

		else if (EvPressKey->GetKeyCode() == KeyPress::KeyQ)
		{
			auto& transform = gEngine.m_coordinator.GetComponent<TransformComponent>((Entity)(m_entities.size() - 1));
			transform.offSetScale(Vec2(-3.0f, -3.0f) * gFixedDeltaTime);
			RE_INFO("Scaled Down!");
		}

		else if (EvPressKey->GetKeyCode() == KeyPress::KeyR)
		{
			auto& transform = gEngine.m_coordinator.GetComponent<TransformComponent>((Entity)(m_entities.size() - 1));
			transform.offSetRotation(3.0f * gFixedDeltaTime);
			RE_INFO("Rotated!");
		}
		else if (EvPressKey->GetKeyCode() == KeyPress::KeyK)
		{
			auto& transform = gEngine.m_coordinator.GetComponent<TransformComponent>((Entity)(m_entities.size() - 1));
			transform.setPosition(Vec2(0.0f, 0.0f));
		}
		else if (EvPressKey->GetKeyCode() == KeyPress::KeyL)
		{
			auto& transform = gEngine.m_coordinator.GetComponent<TransformComponent>((Entity)2);
			transform.setPosition(Vec2(0.0f, 0.0f));
		}

		return;
	}
	case EventType::EvKeyTriggered:
	{
		KeyTriggeredEvent* EvTriggeredKey = dynamic_cast<KeyTriggeredEvent*>(ev);

		if (EvTriggeredKey->GetKeyCode() == KeyPress::KeyG)
			allowGravity = allowGravity ? false : true;

		return;
	}
	default:
	{
		return;
	}
	}
}

// Setters
void PhysicsSystem::setGravity(const Vec2& gravity)
{
	m_gravity = gravity;
}

const Vec2& PhysicsSystem::getGravity() const
{
	return m_gravity;
}
