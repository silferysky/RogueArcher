#include "PhysicsSystem.h"
#include "Main.h"
#include "REMath.h"
#include "ComponentList.h"
#include "Timer.h"
#include "KeyEvent.h"
#include "GameEvent.h"
#include "EventDispatcher.h"
#include "Logger.h"
#include "REEngine.h"

namespace Rogue
{
	//-------------------------------------------------------//
	//              PRIVATE MEMBER FUNCTIONS								 //
	//-------------------------------------------------------//
	void PhysicsSystem::applyForces(RigidbodyComponent& rigidbody) // F = ma
	{
		if (allowGravity)
			rigidbody.setAcceleration(rigidbody.getAccForce() * rigidbody.getInvMass() + m_gravity);
		else
			rigidbody.setAcceleration(rigidbody.getAccForce() * rigidbody.getInvMass());
	}

	void PhysicsSystem::integrateAcceleration(RigidbodyComponent& rigidbody, TransformComponent& transform)
	{
		transform.offSetPosition(rigidbody.getVelocity() * g_fixedDeltaTime);

		rigidbody.offSetVelocity(rigidbody.getAcceleration() * g_fixedDeltaTime);
		rigidbody.setVelocity(rigidbody.getVelocity() * rigidbody.getDamping());

	}


	//-------------------------------------------------------//
	//              PUBLIC MEMBER FUNCTIONS					 //
	//-------------------------------------------------------//
	PhysicsSystem::PhysicsSystem(Vec2 gravity) :
		System(SystemID::id_PHYSICSSYSTEM),
		m_gravity{ gravity }, checkAABB{ true }, checkOBB{ true }, allowGravity{ true }
	{}

	void PhysicsSystem::init()
	{
		REGISTER_LISTENER(SystemID::id_PHYSICSSYSTEM, PhysicsSystem::receive);

		// Add components to signature.
		Signature signature;
		signature.set(g_engine.m_coordinator.GetComponentType<RigidbodyComponent>());
		signature.set(g_engine.m_coordinator.GetComponentType<TransformComponent>());
		signature.set(g_engine.m_coordinator.GetComponentType<BoxCollider2DComponent>());
		//signature.set(g_engine.m_coordinator.GetComponentType<CircleCollider2DComponent>());

		// Set physics system signature.
		g_engine.m_coordinator.SetSystemSignature<PhysicsSystem>(signature);
		m_gravity = { 0.0f, -1.0f };
	}

	void PhysicsSystem::update()
	{
		g_engine.m_coordinator.InitTimeSystem("Physics System");
		for (int step = 0; step < g_engine.GetStepCount(); ++step)
		{
			// For all entities
			std::set<Entity>::iterator iEntity;
			for (iEntity = m_entities.begin(); iEntity != m_entities.end(); ++iEntity)
			{
				auto& rigidbody = g_engine.m_coordinator.GetComponent<RigidbodyComponent>(*iEntity);
				auto& transform = g_engine.m_coordinator.GetComponent<TransformComponent>(*iEntity);
				auto& currBoxCollider = g_engine.m_coordinator.GetComponent<BoxCollider2DComponent>(*iEntity);
				//	auto& circleCollider = g_engine.m_coordinator.GetComponent<CircleCollider2DComponent>(*iEntity);

				if (rigidbody.getIsStatic())
					continue;

				applyForces(rigidbody);

				// Reset accForce
				rigidbody.setAccForce(Vec2());

				// Update positions
				integrateAcceleration(rigidbody, transform);
			}
		}

		g_engine.m_coordinator.EndTimeSystem("Physics System");
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
				auto& rigidbody = g_engine.m_coordinator.GetComponent<RigidbodyComponent>(1);
				rigidbody.addForce(Vec2(-1.0f, 0.0f));
				//RE_INFO("Move A Left!");
			}
			else if (EvPressKey->GetKeyCode() == KeyPress::KeyD)
			{
				auto& rigidbody = g_engine.m_coordinator.GetComponent<RigidbodyComponent>(1);
				rigidbody.addForce(Vec2(1.0f, 0.0f));
				//RE_INFO("Move A Right!");
			}
			
			else if (EvPressKey->GetKeyCode() == KeyPress::KeyW)
			{
				auto& rigidbody = g_engine.m_coordinator.GetComponent<RigidbodyComponent>(1);
				rigidbody.addForce(Vec2(0.0f, 1.0f));
				//RE_INFO("Move A Up!");
			}
			else if (EvPressKey->GetKeyCode() == KeyPress::KeyS)
			{
				auto& rigidbody = g_engine.m_coordinator.GetComponent<RigidbodyComponent>(1);
				rigidbody.addForce(Vec2(0.0f, -1.0f));
				//RE_INFO("Move A Down!");

			}
			if (EvPressKey->GetKeyCode() == KeyPress::KeyArrowLeft)
			{
				auto& rigidbody = g_engine.m_coordinator.GetComponent<RigidbodyComponent>(2);
				rigidbody.addForce(Vec2(-1.0f, 0.0f));
				//RE_INFO("Move B Left!");

			}
			else if (EvPressKey->GetKeyCode() == KeyPress::KeyArrowRight)
			{
				auto& rigidbody = g_engine.m_coordinator.GetComponent<RigidbodyComponent>(2);
				rigidbody.addForce(Vec2(1.0f, 0.0f));
				//RE_INFO("Move B Right!");
			}
			else if (EvPressKey->GetKeyCode() == KeyPress::KeyArrowUp)
			{
				auto& rigidbody = g_engine.m_coordinator.GetComponent<RigidbodyComponent>(2);
				rigidbody.addForce(Vec2(0.0f, 1.0f));
				//RE_INFO("Move B Up!");

			}
			else if (EvPressKey->GetKeyCode() == KeyPress::KeyArrowDown)
			{
				auto& rigidbody = g_engine.m_coordinator.GetComponent<RigidbodyComponent>(2);
				rigidbody.addForce(Vec2(0.0f, -1.0f));
				//RE_INFO("Move B Down!");
			}
			else if (EvPressKey->GetKeyCode() == KeyPress::KeyE)
			{
				auto& transform = g_engine.m_coordinator.GetComponent<TransformComponent>((Entity)(1));
				transform.offSetScale(Vec2(1.0f, 1.0f) * g_fixedDeltaTime);
				//RE_INFO("Scaled Up!");
			}

			else if (EvPressKey->GetKeyCode() == KeyPress::KeyQ)
			{
				auto& transform = g_engine.m_coordinator.GetComponent<TransformComponent>((Entity)(1));
				transform.offSetScale(Vec2(-1.0f, -1.0f) * g_fixedDeltaTime);
				//RE_INFO("Scaled Down!");
			}

			else if (EvPressKey->GetKeyCode() == KeyPress::KeyR)
			{
				auto& transform = g_engine.m_coordinator.GetComponent<TransformComponent>((Entity)1);
				transform.offSetRotation(1.0f * g_fixedDeltaTime);
				//RE_INFO("Rotated!");
			}
			else if (EvPressKey->GetKeyCode() == KeyPress::KeyK)
			{
				auto& transform = g_engine.m_coordinator.GetComponent<TransformComponent>((Entity)1);
				transform.setPosition(Vec2(-2.0f, 0.0f));
			}
			else if (EvPressKey->GetKeyCode() == KeyPress::KeyL)
			{
				auto& transform = g_engine.m_coordinator.GetComponent<TransformComponent>((Entity)2);
				transform.setPosition(Vec2(2.0f, 0.0f));
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
		case EventType::EvEntityMove:
		{
			EntMoveEvent* EvEntMove = dynamic_cast<EntMoveEvent*>(ev);
			
			if (gEngine.m_coordinator.CheckIfComponentExists<TransformComponent>(EvEntMove->GetEntityID()))
			{

			}
			
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
}
