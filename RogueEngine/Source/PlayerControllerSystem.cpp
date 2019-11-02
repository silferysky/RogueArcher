#pragma once
#include "PlayerControllerSystem.h"
#include "Main.h"
#include "EventDispatcher.h"
#include "ComponentList.h"
#include "KeyEvent.h"
#include "GameEvent.h"

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
		signature.set(g_engine.m_coordinator.GetComponentType<TransformComponent>());
		signature.set(g_engine.m_coordinator.GetComponentType<RigidbodyComponent>());

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

			if (keycode == KeyPress::Key5)
				g_engine.m_coordinator.ToggleEditorIsRunning();

			if (keycode == KeyPress::Key6)
				g_engine.ToggleVSync();

			if (keycode == KeyPress::Numpad0 && m_entities.size() > 0)
				g_engine.m_coordinator.clone(*m_entities.begin());

			if (keycode == KeyPress::Numpad1)
				g_engine.m_coordinator.cloneArchetypes("Box");

			if (keycode == KeyPress::Numpad2)
				g_engine.m_coordinator.cloneArchetypes("Circle");

			if (keycode == KeyPress::MB2)
			{
				g_engine.SetTimeScale(0.1f);
			}

			if (keycode == KeyPress::KeySpace)
			{
				for (std::set<Entity>::iterator iEntity = m_entities.begin(); iEntity != m_entities.end(); ++iEntity)
				{
					//For 1st entity
					if (iEntity == m_entities.begin() && g_engine.m_coordinator.ComponentExists<RigidbodyComponent>(*iEntity))
					{
						auto& rigidbody = g_engine.m_coordinator.GetComponent<RigidbodyComponent>(*iEntity);
						rigidbody.addForce(Vec2(0.0f, 50000.0f));
					}
				}
			}

			return;
		} //End KeyTriggered
		case EventType::EvKeyPressed:
		{
			KeyPressEvent* EvPressKey = dynamic_cast<KeyPressEvent*>(ev);
			KeyPress keycode = EvPressKey->GetKeyCode();

			for (std::set<Entity>::iterator iEntity = m_entities.begin(); iEntity != m_entities.end(); ++iEntity)
			{
				//For 1st entity
				if (iEntity == m_entities.begin())
				{
					auto& rigidbody = g_engine.m_coordinator.GetComponent<RigidbodyComponent>(*iEntity);

					if (keycode == KeyPress::KeyA)
					{
						rigidbody.addForce(Vec2(-100.0f, 0.0f) * g_deltaTime * 1000.0f);
						//RE_INFO("Move A Left!");
					}
					else if (keycode == KeyPress::KeyD)
					{
						//rigidbody.addForce(Vec2(100.0f, 0.0f) * g_deltaTime * 1000.0f);
						//RE_INFO("Move A Right!");
						Event* ev = new EntMoveEvent{ (int)*iEntity, true, 100.0f, 0.0f };
						ev->SetSystemReceivers((int)SystemID::id_PHYSICSSYSTEM);
						EventDispatcher::instance().AddEvent(ev);
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
					if (keycode == KeyPress::Numpad4)
					{
						transform.offSetScale(Vec2(100.0f, 100.0f) * g_deltaTime * 60.0f);
						//RE_INFO("Scaled Up!");
					}
					else if (keycode == KeyPress::Numpad5)
					{
						transform.offSetScale(Vec2(-100.0f, -100.0f) * g_deltaTime * 60.0f);
						//RE_INFO("Scaled Down!");
					}
					else if (keycode == KeyPress::Numpad6)
					{
						transform.offSetRotation(100.0f * g_deltaTime * 60.0f);
						//RE_INFO("Rotated!");
					}
					else if (keycode == KeyPress::Numpad7)
					{
						transform.setPosition(Vec2(-200.0f, 0.0f));
					}
				}
				//For 2nd Entity
				//To be deleted in future (Since by right only one entityt should exist here for player
				else if (iEntity != m_entities.begin())
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
					if (keycode == KeyPress::Numpad8)
					{
						transform.setPosition(Vec2(200.0f, 0.0f));
					}
				} // Entity 2 
			} // End of Entity for-loop

			return;
		}
		case EventType::EvKeyReleased:
		{
			KeyReleaseEvent* KeyReleaseEv = dynamic_cast<KeyReleaseEvent*>(ev);
			KeyPress keycode = KeyReleaseEv->GetKeyCode();

			if (keycode == KeyPress::MB2)
			{
				g_engine.SetTimeScale(1.0f);
			}
			return;
		}
		} // switch (ev->GetEventType())
	} // Receive

	void PlayerControllerSystem::Shutdown()
	{
	}


}