#include "Precompiled.h"
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
	//              PRIVATE MEMBER FUNCTIONS				 //
	//-------------------------------------------------------//

	void PhysicsSystem::Integrate(RigidbodyComponent& rigidbody, TransformComponent& transform) const
	{
		float simulationTime = g_fixedDeltaTime * g_engine.GetTimeScale(); // To support slow motion

		if (allowGravity)
			rigidbody.setAcceleration(rigidbody.getAccForce() * rigidbody.getInvMass() + m_gravity * rigidbody.getGravityScale());
		else
			rigidbody.setAcceleration(rigidbody.getAccForce() * rigidbody.getInvMass());

		transform.offSetPosition(rigidbody.getVelocity() * simulationTime);

		rigidbody.offSetVelocity(rigidbody.getAcceleration() * simulationTime);
		rigidbody.setVelocity(rigidbody.getVelocity() * std::pow(rigidbody.getDamping(), simulationTime));

	}


	//-------------------------------------------------------//
	//              PUBLIC MEMBER FUNCTIONS					 //
	//-------------------------------------------------------//
	PhysicsSystem::PhysicsSystem(Vec2 gravity) :
		System(SystemID::id_PHYSICSSYSTEM),
		m_gravity{ gravity }, checkAABB{ true }, checkOBB{ true }, allowGravity{ true }
	{}

	void PhysicsSystem::Init()
	{
		REGISTER_LISTENER(SystemID::id_PHYSICSSYSTEM, PhysicsSystem::Receive);

		// Add components to signature.
		Signature signature;
		signature.set(g_engine.m_coordinator.GetComponentType<RigidbodyComponent>());
		signature.set(g_engine.m_coordinator.GetComponentType<TransformComponent>());
		
		// Set physics system signature.
		g_engine.m_coordinator.SetSystemSignature<PhysicsSystem>(signature);
		m_gravity = Vec2{ 0.0f, -500.0f };

		// Initialize Force Manager.
		ForceManager::instance().Init();
	}

	void PhysicsSystem::Update()
	{
		g_engine.m_coordinator.InitTimeSystem("Physics System");

		for (int step = 0; step < g_engine.GetStepCount(); ++step)
		{
			ForceManager::instance().UpdateAges();

			// For all entities
			std::set<Entity>::iterator iEntity;
			for (iEntity = m_entities.begin(); iEntity != m_entities.end(); ++iEntity)
			{
				auto& rigidbody = g_engine.m_coordinator.GetComponent<RigidbodyComponent>(*iEntity);

				if (rigidbody.getIsStatic())
					continue;

				auto& transform = g_engine.m_coordinator.GetComponent<TransformComponent>(*iEntity);

				// Add relevant forces to each rigidbody
				ForceManager::instance().AddForce(*iEntity, rigidbody);

				// Update positions
				Integrate(rigidbody, transform);

				// Reset accForce
				rigidbody.setAccForce(Vec2());
			}
		}
		g_engine.m_coordinator.EndTimeSystem("Physics System");
	}

	void PhysicsSystem::Receive(Event* ev)
	{
		switch (ev->GetEventType())
		{
		case EventType::EvKeyTriggered:
		{
			KeyTriggeredEvent* EvTriggeredKey = dynamic_cast<KeyTriggeredEvent*>(ev);

			//if (EvTriggeredKey->GetKeyCode() == KeyPress::Numpad9)
			//	allowGravity = allowGravity ? false : true;

			return;
		}
		case EventType::EvEntityMove:
		{
			//EntMoveEvent* EvEntMove = dynamic_cast<EntMoveEvent*>(ev);
			//ForceManager::instance().RegisterForce(EvEntMove->GetEntityID(),
				//EvEntMove->GetVecMovement(),
				//g_fixedDeltaTime);
			return;
		}
		case EventType::EvEntityTeleport:
		{
			EntTeleportEvent* EvEntTeleport = dynamic_cast<EntTeleportEvent*>(ev);
			g_engine.m_coordinator.GetComponent<TransformComponent>(EvEntTeleport->GetEntityID()).
				setPosition(EvEntTeleport->GetVecMovement());
		}
		default:
		{
			return;
		}
		}
	}

	bool PhysicsSystem::getToggleGravity()
	{
		return allowGravity;
	}

	void PhysicsSystem::setToggleGravity(bool gravity)
	{
		allowGravity = gravity;
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

	void PhysicsSystem::Shutdown()
	{}
}
