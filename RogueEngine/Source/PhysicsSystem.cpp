/* Start Header ************************************************************************/
/*!
\file           PhysicsSystem.cpp
\project        Exale
\author         Lim Jia Hao, l.jiahao, 390006518 (100%)
\par            l.jiahao\@digipen.edu
\date           1 December,2019
\brief          This file contains the function definitions for PhysicsSystem

All content (C) 2019 DigiPen (SINGAPORE) Corporation, all rights
reserved.

Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header **************************************************************************/
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
		m_gravity = Vec2{ 0.0f, -1500.0f };

		// Initialize Force Manager.
		ForceManager::instance().Init();
	}

	void PhysicsSystem::Update()
	{
		g_engine.m_coordinator.InitTimeSystem("Physics System");

		for (int step = 0; step < g_engine.GetStepCount(); ++step)
		{
			// For all entities
			std::set<Entity>::iterator iEntity;
			for (iEntity = m_entities.begin(); iEntity != m_entities.end(); ++iEntity)
			{
#if 0
				std::string name = g_engine.m_coordinator.GetHierarchyInfo(*iEntity).m_objectName;
				bool correct = false;
				if (name == "Ball")
				{
					correct = true;
				}
#endif
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

			ForceManager::instance().UpdateAges();
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
			EntMoveEvent* EvEntMove = dynamic_cast<EntMoveEvent*>(ev);

			Entity player = EvEntMove->GetEntityID();

			if (!g_engine.m_coordinator.ComponentExists<PlayerControllerComponent>(player))
				return;

			PlayerControllerComponent& playerCtrl = g_engine.m_coordinator.GetComponent<PlayerControllerComponent>(player);
			RigidbodyComponent& rigidbody = g_engine.m_coordinator.GetComponent<RigidbodyComponent>(player);
			
			float playerSpeed = playerCtrl.GetMoveSpeed();

			const float forceMultiplier = 10.0f; // How fast the player accelerates
			//const float maxForce = 10000.0f; // An arbitruary number to limit the max force
			Vec2 vecMovement = EvEntMove->GetVecMovement();
			Vec2 targetVel = vecMovement * playerSpeed; // The max speed the player can go
			Vec2 force{}; // The force to apply on the player

			if (vecMovement.y == 0)
				force.x = targetVel.x - rigidbody.getVelocity().x * forceMultiplier;
			else if (vecMovement.x == 0)
				force.y = targetVel.y - rigidbody.getVelocity().y * forceMultiplier;
			else
				force = Vec2(targetVel - rigidbody.getVelocity()) * forceMultiplier;

#if 0
			// Clamp force to prevent force from exceeding an amount
			if (Vec2SqDistance(force) > maxForce * maxForce)
			{
				Vec2 forceNormalized;
				Vec2Normalize(forceNormalized, force);

				force = maxForce * forceNormalized;
			}
#endif
#if 0
			// Clamp force to prevent force from exceeding an amount (For horizontal movement tentatively)
			if (force.x > maxForce)
			{
				force.x = maxForce;
			}
#endif	
			ForceManager::instance().RegisterForce(player, force, g_fixedDeltaTime);

			return;
		}

		case EventType::EvEntityTeleport:
		{
			EntTeleportEvent* EvEntTeleport = dynamic_cast<EntTeleportEvent*>(ev);
			if (g_engine.m_coordinator.ComponentExists<TransformComponent>(EvEntTeleport->GetEntityID()))
				g_engine.m_coordinator.GetComponent<TransformComponent>(EvEntTeleport->GetEntityID()).
					setPosition(EvEntTeleport->GetVecMovement());

			if (g_engine.m_coordinator.ComponentExists<RigidbodyComponent>(EvEntTeleport->GetEntityID()))
			{
				g_engine.m_coordinator.GetComponent<RigidbodyComponent>(EvEntTeleport->GetEntityID()).setVelocity(Vec2());
				g_engine.m_coordinator.GetComponent<RigidbodyComponent>(EvEntTeleport->GetEntityID()).setAcceleration(Vec2());
			}

			return;
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
