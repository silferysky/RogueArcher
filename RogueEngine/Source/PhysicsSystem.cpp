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
//#include "ParentEvent.h"
#include "EventDispatcher.h"
#include "Logger.h"
#include "REEngine.h"

#define MOVE_WITH_FORCE 1

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

		rigidbody.offSetVelocity(rigidbody.getAcceleration() * simulationTime); // Acceleration -> Velocity
		rigidbody.setVelocity(rigidbody.getVelocity() * std::pow(rigidbody.getDamping(), simulationTime)); // Velocity w/ damping
		transform.offSetPosition(rigidbody.getVelocity() * simulationTime); // Velocity -> Position
	}

	Vec2 PhysicsSystem::PlayerMoveByForce(PlayerControllerComponent& playerCtrl, RigidbodyComponent& rigidbody, Vec2 moveDir)
	{
		float playerSpeed = playerCtrl.GetMoveSpeed();
		float forceMultiplier = playerCtrl.GetMoveForceMultiplier(); // How fast the player accelerates
		Vec2 targetVel = moveDir * playerSpeed; // The max speed the player can go
		
		Vec2 force{}; // The force to apply on the player

		if (moveDir.y == 0)
			force.x = (targetVel.x - rigidbody.getVelocity().x) * forceMultiplier;
		else if (moveDir.x == 0)
			force.y = targetVel.y - rigidbody.getVelocity().y * forceMultiplier;
		else
			force = Vec2(targetVel - rigidbody.getVelocity()) * forceMultiplier;

#if 0
		const float maxForce = playerSpeed / g_fixedDeltaTime; // An arbitruary number to limit the max force

		// Clamp force to prevent force from exceeding an amount
		if (Vec2SqLength(force) > maxForce * maxForce)
		{
			force = maxForce * moveDir;
		}
#endif
#if 0
		// Clamp force to prevent force from exceeding an amount (For horizontal movement tentatively)
		if (REAbs(force.x) > maxForce.x)
		{
			force.x = maxForce.x * moveDir.x;
		}
#endif	
		return force;
	}

	void PhysicsSystem::PlayerMoveByVelocity(PlayerControllerComponent& playerCtrl, RigidbodyComponent& rigidbody, const Vec2& vecDir)
	{
		float playerSpeed = playerCtrl.GetMoveSpeed();

		rigidbody.setVelocity(Vec2(playerSpeed / 5.0f * vecDir.x, rigidbody.getVelocity().y));
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
		m_gravity = Vec2{ 0.0f, -1250.0f };

		// Initialize Force Manager.
		ForceManager::instance().Init();
	}

	void PhysicsSystem::Update()
	{
		// Add relevant forces to each rigidbody
		ForceManager::instance().ApplyAllForces();
		
		if (auto playerCtrlOpt = g_engine.m_coordinator.TryGetComponent<PlayerControllerComponent>(PLAYER_STATUS.GetPlayerEntity()))
		{
			PlayerControllerComponent& playerCtrl = playerCtrlOpt->get();
			RigidbodyComponent& rigidbody = g_engine.m_coordinator.GetComponent<RigidbodyComponent>(PLAYER_STATUS.GetPlayerEntity());

			float x = 0.0f;

			if (playerCtrl.GetMoveState() == MoveState::e_left)
				x = -1.0f;
			else if (playerCtrl.GetMoveState() == MoveState::e_right)
				x = 1.0f;

			if (playerCtrl.GetMoveState() != MoveState::e_stop)
			{
#if MOVE_WITH_FORCE
				Vec2 force = PlayerMoveByForce(playerCtrl, rigidbody, Vec2(x, 0.0f));

				for (int steps = 0; steps < g_engine.GetStepCount(); steps++)
				{
					ForceManager::instance().RegisterForce(PLAYER_STATUS.GetPlayerEntity(), force);
				}
#else
				PlayerMoveByVelocity(playerCtrl, rigidbody, vecMovement);
#endif
			}
		}

		// For all entities
		std::set<Entity>::iterator iEntity;
		for (iEntity = m_entities.begin(); iEntity != m_entities.end(); ++iEntity)
		{
			auto& rigidbody = g_engine.m_coordinator.GetComponent<RigidbodyComponent>(*iEntity);

			if (rigidbody.getIsStatic() || !rigidbody.m_componentIsActive) // Skip static bodies
				continue;

			// Stop player
			if (auto playerOpt = g_engine.m_coordinator.TryGetComponent<PlayerControllerComponent>(*iEntity))
				if (playerOpt->get().GetMoveState() == MoveState::e_stop)
					ForceManager::instance().RegisterForce(*iEntity, Vec2(rigidbody.getVelocity().x * -ForceManager::instance().c_stopFactor, 0.0f));
			
					auto& transform = g_engine.m_coordinator.GetComponent<TransformComponent>(*iEntity);

			// Update positions
			Integrate(rigidbody, transform);

			// Reset accForce
			rigidbody.setAccForce(Vec2());
#if 0
			if (g_engine.m_coordinator.GetHierarchyInfo(*iEntity).m_children.size())
			{
				ParentTransformEvent& parentEv = new ParentTransformEvent(*iEntity, MAX_ENTITIES);
				parentEv.SetSystemReceivers((int)SystemID::id_PARENTCHILDSYSTEM);
				EventDispatcher::instance().AddEvent(parentEv);
			}
#endif
		}

		// Increase their ages 
		ForceManager::instance().UpdateAges();
	}

	void PhysicsSystem::Receive(Event& ev)
	{
		switch (ev.GetEventType())
		{
		case EventType::EvKeyTriggered:
		{
			KeyTriggeredEvent& EvTriggeredKey = dynamic_cast<KeyTriggeredEvent&>(ev);
			
			if(EvTriggeredKey.GetKeyCode() == KeyPress::MB2)
				ForceManager::instance().PrintForceVector();

			//if (EvTriggeredKey.GetKeyCode() == KeyPress::Numpad9)
			//	allowGravity = allowGravity ? false : true;

			return;
		}
		case EventType::EvEntityMove:
		{/*
			EntMoveEvent& EvEntMove = dynamic_cast<EntMoveEvent&>(ev);

			Entity player = EvEntMove.GetEntityID();

			if (!g_engine.m_coordinator.ComponentExists<PlayerControllerComponent>(player))
				return;

			PlayerControllerComponent& playerCtrl = g_engine.m_coordinator.GetComponent<PlayerControllerComponent>(player);
			RigidbodyComponent& rigidbody = g_engine.m_coordinator.GetComponent<RigidbodyComponent>(player);

			Vec2 vecMovement = EvEntMove.GetVecMovement();

#ifdef MOVE_WITH_FORCE
			Vec2 force = PlayerMoveByForce(playerCtrl, rigidbody, vecMovement);

			for (int steps = 0; steps < g_engine.GetStepCount(); steps++)
			{
				ForceManager::instance().RegisterForce(player, force);
			}
#else
			PlayerMoveByVelocity(playerCtrl, rigidbody, vecMovement);
#endif*/

			return;
		}

		case EventType::EvEntityTeleport:
		{
			EntTeleportEvent& EvEntTeleport = dynamic_cast<EntTeleportEvent&>(ev);
			Vec2 oldPos{};
			if (g_engine.m_coordinator.ComponentExists<TransformComponent>(EvEntTeleport.GetEntityID()))
			{
				oldPos = g_engine.m_coordinator.GetComponent<TransformComponent>(EvEntTeleport.GetEntityID()).GetPosition();
				g_engine.m_coordinator.GetComponent<TransformComponent>(EvEntTeleport.GetEntityID()).
					setPosition(EvEntTeleport.GetVecMovement());
			}

			if (g_engine.m_coordinator.ComponentExists<RigidbodyComponent>(EvEntTeleport.GetEntityID()))
			{
				ForceManager::instance().ResetPhysics(EvEntTeleport.GetEntityID());

				oldPos = EvEntTeleport.GetVecMovement() - oldPos;
				Vec2Normalize(oldPos, oldPos);
				ForceManager::instance().RegisterForce(EvEntTeleport.GetEntityID(), oldPos * 11000.0f);
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
