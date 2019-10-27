#pragma once
#include "PlayerControllerSystem.h"
#include "Main.h"
#include "EventDispatcher.h"
#include "ComponentList.h"
#include "KeyEvent.h"
//#include "GameEvent.h"

namespace Rogue
{
	PlayerControllerSystem::PlayerControllerSystem()
		:System(SystemID::id_PLAYERCONTROLLERSYSTEM)
	{
	}

	void PlayerControllerSystem::Init()
	{
		REGISTER_LISTENER(SystemID::id_PLAYERCONTROLLERSYSTEM, PlayerControllerSystem::Receive);

		Signature signature;
		signature.set(g_engine.m_coordinator.GetComponentType<PlayerControllerComponent>());

		g_engine.m_coordinator.SetSystemSignature<PlayerControllerSystem>(signature);
	}

	void PlayerControllerSystem::Update()
	{
		//PlayerControllerSystem does not update, since it only handles player events (aka button pushes that affect player)
	}

	void PlayerControllerSystem::Receive(Event* ev)
	{
		switch (ev->GetEventType())
		{
		case EventType::EvKeyTriggered:
		{
			KeyTriggeredEvent* keytriggeredevent = dynamic_cast<KeyTriggeredEvent*>(ev);
			KeyPress keycode = keytriggeredevent->GetKeyCode();

			if (keycode == KeyPress::KeyEsc)
				g_engine.SetGameIsRunning(false);


			if (keycode == KeyPress::KeyC)
				g_engine.m_coordinator.clone(1);

			if (keycode == KeyPress::KeyB)
				g_engine.m_coordinator.cloneArchetypes("Box");

			if (keycode == KeyPress::KeyN)
				g_engine.m_coordinator.cloneArchetypes("Circle");

			return;
		}
		case EventType::EvKeyPressed:
		{
			KeyPressEvent* EvPressKey = dynamic_cast<KeyPressEvent*>(ev);
			KeyPress keycode = EvPressKey->GetKeyCode();

			std::set<Entity>::iterator iEntity;
			for (iEntity = m_entities.begin(); iEntity != m_entities.end(); ++iEntity)
			{
				//For 1st entity
				if (*iEntity == 1)
				{
					if (g_engine.m_coordinator.CheckIfComponentExists<RigidbodyComponent>(*iEntity))
					{
						if (keycode == KeyPress::KeyA)
						{
							auto& rigidbody = g_engine.m_coordinator.GetComponent<RigidbodyComponent>(*iEntity);
							rigidbody.addForce(Vec2(-100.0f, 0.0f));
							//RE_INFO("Move A Left!");
						}
						else if (keycode == KeyPress::KeyD)
						{
							auto& rigidbody = g_engine.m_coordinator.GetComponent<RigidbodyComponent>(*iEntity);
							rigidbody.addForce(Vec2(100.0f, 0.0f));
							//RE_INFO("Move A Right!");
						}
						else if (keycode == KeyPress::KeyW)
						{
							auto& rigidbody = g_engine.m_coordinator.GetComponent<RigidbodyComponent>(*iEntity);
							rigidbody.addForce(Vec2(0.0f, 100.0f));
							//RE_INFO("Move A Up!");
						}
						else if (keycode == KeyPress::KeyS)
						{
							auto& rigidbody = g_engine.m_coordinator.GetComponent<RigidbodyComponent>(*iEntity);
							rigidbody.addForce(Vec2(0.0f, -100.0f));
							//RE_INFO("Move A Down!");
						}
					}
					if (g_engine.m_coordinator.CheckIfComponentExists<TransformComponent>(*iEntity))
					{
						if (keycode == KeyPress::KeyE)
						{
							auto& transform = g_engine.m_coordinator.GetComponent<TransformComponent>(*iEntity);
							transform.offSetScale(Vec2(100.0f, 100.0f) * g_fixedDeltaTime);
							//RE_INFO("Scaled Up!");
						}
						else if (keycode == KeyPress::KeyQ)
						{
							auto& transform = g_engine.m_coordinator.GetComponent<TransformComponent>(*iEntity);
							transform.offSetScale(Vec2(-100.0f, -100.0f) * g_fixedDeltaTime);
							//RE_INFO("Scaled Down!");
						}
						else if (keycode == KeyPress::KeyR)
						{
							auto& transform = g_engine.m_coordinator.GetComponent<TransformComponent>(*iEntity);
							transform.offSetRotation(100.0f * g_fixedDeltaTime);
							//RE_INFO("Rotated!");
						}
						else if (keycode == KeyPress::KeyK)
						{
							auto& transform = g_engine.m_coordinator.GetComponent<TransformComponent>(*iEntity);
							transform.setPosition(Vec2(-200.0f, 0.0f));
						}
					}

					continue;
				}
				//For 2nd Entity
				else if (*iEntity == 2)
				{
					if (g_engine.m_coordinator.CheckIfComponentExists<RigidbodyComponent>(*iEntity))
					{
						if (keycode == KeyPress::KeyArrowLeft)
						{
							auto& rigidbody = g_engine.m_coordinator.GetComponent<RigidbodyComponent>(*iEntity);
							rigidbody.addForce(Vec2(-100.0f, 0.0f));
							//RE_INFO("Move B Left!");

						}
						else if (keycode == KeyPress::KeyArrowRight)
						{
							auto& rigidbody = g_engine.m_coordinator.GetComponent<RigidbodyComponent>(*iEntity);
							rigidbody.addForce(Vec2(100.0f, 0.0f));
							//RE_INFO("Move B Right!");
						}
						else if (keycode == KeyPress::KeyArrowUp)
						{
							auto& rigidbody = g_engine.m_coordinator.GetComponent<RigidbodyComponent>(*iEntity);
							rigidbody.addForce(Vec2(0.0f, 100.0f));
							//RE_INFO("Move B Up!");

						}
						else if (keycode == KeyPress::KeyArrowDown)
						{
							auto& rigidbody = g_engine.m_coordinator.GetComponent<RigidbodyComponent>(*iEntity);
							rigidbody.addForce(Vec2(0.0f, -100.0f));
							//RE_INFO("Move B Down!");
						}
					}

					if (g_engine.m_coordinator.CheckIfComponentExists<TransformComponent>(*iEntity))
					{
						if (keycode == KeyPress::KeyL)
						{
							auto& transform = g_engine.m_coordinator.GetComponent<TransformComponent>(*iEntity);
							transform.setPosition(Vec2(200.0f, 0.0f));
						}
					}

					continue;
				}
			}
		}
		}
	}

	void PlayerControllerSystem::Shutdown()
	{
	}


}