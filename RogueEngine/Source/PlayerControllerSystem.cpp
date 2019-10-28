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

			if (keycode == KeyPress::MB2)
			{
				g_engine.SetTimeScale(0.5f);
			}

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
					auto& rigidbody = g_engine.m_coordinator.GetComponent<RigidbodyComponent>(*iEntity);

					if (keycode == KeyPress::KeyA)
					{
						rigidbody.addForce(Vec2(-100.0f, 0.0f) * g_deltaTime * 1000.0f);
						//RE_INFO("Move A Left!");
					}
					else if (keycode == KeyPress::KeyD)
					{
						rigidbody.addForce(Vec2(100.0f, 0.0f) * g_deltaTime * 1000.0f);
						//RE_INFO("Move A Right!");
					}
					else if (keycode == KeyPress::KeyW)
					{
						rigidbody.addForce(Vec2(0.0f, 100.0f) * g_deltaTime * 1000.0f);
						//RE_INFO("Move A Up!");
					}
					else if (keycode == KeyPress::KeyS)
					{
						rigidbody.addForce(Vec2(0.0f, -100.0f) * g_deltaTime * 1000.0f);
						//RE_INFO("Move A Down!");
					}

					auto& transform = g_engine.m_coordinator.GetComponent<TransformComponent>(*iEntity);
					if (keycode == KeyPress::KeyE)
					{
						transform.offSetScale(Vec2(100.0f, 100.0f) * g_deltaTime * 60.0f);
						//RE_INFO("Scaled Up!");
					}
					else if (keycode == KeyPress::KeyQ)
					{
						transform.offSetScale(Vec2(-100.0f, -100.0f) * g_deltaTime * 60.0f);
						//RE_INFO("Scaled Down!");
					}
					else if (keycode == KeyPress::KeyR)
					{
						transform.offSetRotation(100.0f * g_deltaTime * 60.0f);
						//RE_INFO("Rotated!");
					}
					else if (keycode == KeyPress::KeyK)
					{
						transform.setPosition(Vec2(-200.0f, 0.0f));
					}
				}

				//For 2nd Entity
				else if (*iEntity == 2)
				{
					auto& rigidbody = g_engine.m_coordinator.GetComponent<RigidbodyComponent>(*iEntity);
					if (keycode == KeyPress::KeyArrowLeft)
					{
						rigidbody.addForce(Vec2(-100.0f, 0.0f) * g_deltaTime * 1000.0f);
						//RE_INFO("Move B Left!");

					}
					else if (keycode == KeyPress::KeyArrowRight)
					{
						rigidbody.addForce(Vec2(100.0f, 0.0f) * g_deltaTime * 1000.0f);
						//RE_INFO("Move B Right!");
					}
					else if (keycode == KeyPress::KeyArrowUp)
					{
						rigidbody.addForce(Vec2(0.0f, 100.0f) * g_deltaTime * 1000.0f);
						//RE_INFO("Move B Up!");

					}
					else if (keycode == KeyPress::KeyArrowDown)
					{
						rigidbody.addForce(Vec2(0.0f, -100.0f) * g_deltaTime * 1000.0f);
						//RE_INFO("Move B Down!");
					}


					auto& transform = g_engine.m_coordinator.GetComponent<TransformComponent>(*iEntity);
					if (keycode == KeyPress::KeyL)
					{
						transform.setPosition(Vec2(200.0f, 0.0f));
					}

					break;
				} // Entity 2
			} // End of Entity for-loop

			break;
		} // case EventType::EvKeyPressed:

		//case EventType::EvMouseButtonPressed:
		//{
		//	MousePressEvent* MousePressEv = dynamic_cast<MousePressEvent*>(ev);
		//	KeyPress keycode = MousePressEv->GetKeyCode();

		//	if (keycode == KeyPress::MB2)
		//	{
		//		g_engine.SetTimeScale(0.5f);
		//	}

		//	break;
		//}

		case EventType::EvKeyReleased:
		{
			KeyReleaseEvent* KeyReleaseEv = dynamic_cast<KeyReleaseEvent*>(ev);
			KeyPress keycode = KeyReleaseEv->GetKeyCode();

			if (keycode == KeyPress::MB2)
			{
				g_engine.SetTimeScale(1.0f);
			}
			
			break;
		}
		} // switch (ev->GetEventType())
	} // Receive

	void PlayerControllerSystem::Shutdown()
	{
	}


}