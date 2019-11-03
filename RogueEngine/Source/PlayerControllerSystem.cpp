#pragma once
#include "PlayerControllerSystem.h"
#include "Main.h"
#include "ForceManager.h"
#include "EventDispatcher.h"
#include "ComponentList.h"
#include "KeyEvent.h"
#include "GameEvent.h"
#include "GraphicsEvent.h"

namespace Rogue
{
	PlayerControllerSystem::PlayerControllerSystem()
		:System(SystemID::id_PLAYERCONTROLLERSYSTEM), m_ballTimer{-0.1f}, m_ballCooldown{0.0f}
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
		POINT cursor;
		Vec2 cursorPos;

		if (GetCursorPos(&cursor))
		{
		//	cursorPos.x = cursor.x - GetWindowWidth(g_engine.GetWindowHandler()) / 2.0f;
		//	cursorPos.y = -(cursor.y - GetWindowHeight(g_engine.GetWindowHandler()) / 2.0f);
		}

		cursorPos.x = cursor.x;
		cursorPos.y = cursor.y;

		float x = (2.0f * cursorPos.x) / GetWindowWidth(g_engine.GetWindowHandler()) - 1.0f;
		float y = 1.0f - (2.0f * cursorPos.y) / GetWindowHeight(g_engine.GetWindowHandler());
		float z = 1.0f;

		glm::vec3 rayNDC = glm::vec3(x, y, z);

		glm::vec4 rayClip = glm::vec4(rayNDC.x, rayNDC.y, -1.0f, 1.0f);

		glm::vec4 rayEye = glm::inverse(g_engine.GetProjMat()) * rayClip;

		rayEye = glm::vec4(rayEye.x, rayEye.y, -1.0f, 0.0f);

		glm::mat4 viewMat = g_engine.m_coordinator.GetSystem<CameraSystem>()->GetViewMatrix();

		glm::vec4 rayWorld4D = glm::inverse(viewMat) * rayEye;

		glm::vec3 rayWorld3D{ rayWorld4D.x, rayWorld4D.y, rayWorld4D.z };

		//rayWorld3D = glm::normalize(rayWorld3D);

		//std::cout << "Gimme ray coordinates thanks! " << rayWorld3D.x << ", " << rayWorld3D.y << ", " << rayWorld3D.z << std::endl;

		//auto& trans = g_engine.m_coordinator.GetComponent<TransformComponent>(*m_entities.begin());

		//float direction = Vec2Rotation(cursorPos, Vec2{ 0,0 }); // Player must be center of screen for this to work

		//std::cout << RadiansToDegrees(direction) << " degrees" << std::endl;

		//For PlayerControllerSystem Timer
		if (m_ballTimer > 0.0f)
		{
			m_ballTimer -= g_deltaTime * g_engine.GetTimeScale();
			if (m_ballTimer < 0.0f)
				CreateBallAttack();
			//RE_INFO("BALL TIMER UPDATE");
		}
		else
		{
			m_ballCooldown -= g_deltaTime * g_engine.GetTimeScale();
			//RE_INFO("BALL COOLDOWN UPDATE");
		}

		//To update all timed entities
		for (auto timedEntityIt = m_timedEntities.begin(); timedEntityIt != m_timedEntities.end(); ++timedEntityIt)
		{
			timedEntityIt->m_durationLeft -= g_deltaTime * g_engine.GetTimeScale();
			if (timedEntityIt->m_durationLeft < 0.0f)
			{
				ClearTimedEntities();

				if (m_timedEntities.size() == 0)
					break;
			}
		}
	}

	void PlayerControllerSystem::Receive(Event* ev)
	{
		switch (ev->GetEventType())
		{
		case EventType::EvMouseMoved:
		{
			MouseMoveEvent* mouseMove = dynamic_cast<MouseMoveEvent*>(ev);
			KeyPress keycode = mouseMove->GetKeyCode();

		}

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

			//Statement here to make sure all of the other commands only apply if game is not running
			if (!g_engine.m_coordinator.GameIsActive())
				return;

			if (keycode == KeyPress::MB1)
			{
				if (m_entities.size() && m_timedEntities.size())
				{
					//By right correct way of doing this
					//CreateTeleportEvent(g_engine.m_coordinator.GetComponent<TransformComponent>(m_timedEntities.begin()->m_entity).getPosition());
					g_engine.m_coordinator.GetComponent<TransformComponent>(*m_entities.begin()).setPosition(
						g_engine.m_coordinator.GetComponent<TransformComponent>(m_timedEntities.begin()->m_entity).getPosition());

					ClearTimedEntities();
					CameraShakeEvent* cameraShakeEvent = new CameraShakeEvent(15.0f);
					EventDispatcher::instance().AddEvent(cameraShakeEvent);
				}
			}

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
			//Statement here to make sure all of the other commands only apply if game is not running
			if (!g_engine.m_coordinator.GameIsActive())
				return;

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
						rigidbody.addForce(Vec2(100.0f, 0.0f) * g_deltaTime * 1000.0f);
						//RE_INFO("Move A Right!");
						//Event* ev = new EntMoveEvent{ *iEntity, true, 100.0f, 0.0f };
						//ev->SetSystemReceivers((int)SystemID::id_PHYSICSSYSTEM);
						//EventDispatcher::instance().AddEvent(ev);
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

			if (!g_engine.m_coordinator.GameIsActive())
				return;

			if (keycode == KeyPress::MB1)
			{
				if (!m_timedEntities.size() && m_ballTimer < 0.0f && m_ballCooldown < 0.0f)
				{
					m_ballTimer = 1.0f;
					m_ballCooldown = 1.0f;
					RE_INFO("CLICKCLICK");

					if (g_engine.m_coordinator.ComponentExists<AnimationComponent>(*m_entities.begin()))
						g_engine.m_coordinator.GetComponent<AnimationComponent>(*m_entities.begin()).setIsAnimating(true);
				}
			}

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

	std::vector<TimedEntity> PlayerControllerSystem::GetTimedEntities() const
	{
		return m_timedEntities;
	}

	void PlayerControllerSystem::AddToTimedEntities(TimedEntity newEnt)
	{
		m_timedEntities.push_back(newEnt);
	}

	void PlayerControllerSystem::AddToTimedEntities(Entity entity, float duration)
	{
		TimedEntity newEntity(entity, duration);
		m_timedEntities.push_back(newEntity);
	}

	void PlayerControllerSystem::ClearTimedEntities()
	{
		for (TimedEntity entity : m_timedEntities)
		{
			g_engine.m_coordinator.DestroyEntity(entity.m_entity);
		}

		auto& activeObjects = g_engine.m_coordinator.GetActiveObjects();
		for (auto iterator = activeObjects.begin(); iterator != activeObjects.end(); ++iterator)
		{
			if (m_timedEntities.size() && iterator->m_Entity == m_timedEntities.begin()->m_entity)
			{
				iterator = activeObjects.erase(iterator);
				break;
			}
		}

		m_timedEntities.clear();
	}

	void PlayerControllerSystem::CreateTeleportEvent(Vec2 newPosition)
	{
		EntTeleportEvent* event = new EntTeleportEvent(*m_entities.begin(), newPosition);
		event->SetSystemReceivers((int)SystemID::id_PHYSICSSYSTEM);
		EventDispatcher::instance().AddEvent(event);
	}

	void PlayerControllerSystem::CreateBallAttack()
	{
		std::ostringstream strstream;
		Entity ball = g_engine.m_coordinator.CreateEntity();

		//For Cursor position
		POINT cursor;
		Vec2 cursorPos;

		if (GetCursorPos(&cursor))
		{
			cursorPos.x = cursor.x - GetWindowWidth(g_engine.GetWindowHandler()) / 2.0f;
			cursorPos.y = -(cursor.y - GetWindowHeight(g_engine.GetWindowHandler()) / 2.0f);
		}

		Vec2Normalize(cursorPos, cursorPos);

		//Creating Components
		//Transform
		Vec2 tempVec{};
		if (m_entities.size() && g_engine.m_coordinator.ComponentExists<TransformComponent>(*m_entities.begin()))
			tempVec = g_engine.m_coordinator.GetComponent<TransformComponent>(*m_entities.begin()).getPosition();
		TransformComponent& transform = g_engine.m_coordinator.CreateComponent<TransformComponent>(ball);
		strstream	<< tempVec.x + cursorPos.x * POSITION_RELATIVITY << ";"
					<< tempVec.y + cursorPos.y * POSITION_RELATIVITY << ";"
					<< "50;50;0";
		transform.Deserialize(strstream.str());

		SpriteComponent& sprite = g_engine.m_coordinator.CreateComponent<SpriteComponent>(ball);
		sprite.Deserialize("Resources/Assets/Projectile.png;1");

		RigidbodyComponent& rigidbody = g_engine.m_coordinator.CreateComponent<RigidbodyComponent>(ball);
		rigidbody.Deserialize("0;0;0;0;1;1;0");
		//ForceManager::instance().RegisterForce(ball, Vec2(cursorPos.x * FORCE_FACTOR, cursorPos.y * FORCE_FACTOR), 1.0f);
		rigidbody.addForce(Vec2(cursorPos.x * FORCE_FACTOR, cursorPos.y * FORCE_FACTOR));

		BoxCollider2DComponent& boxCollider = g_engine.m_coordinator.CreateComponent<BoxCollider2DComponent>(ball);
		boxCollider.Deserialize("0;0;0;0;0");

		HierarchyInfo newInfo{};
		newInfo.m_Entity = ball;
		newInfo.m_objectName = "Ball";
		g_engine.m_coordinator.GetEntityManager().m_getActiveObjects().push_back(newInfo);

		AddToTimedEntities(ball, 1.0f);
	}


}