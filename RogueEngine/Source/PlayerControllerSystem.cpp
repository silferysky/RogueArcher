#pragma once
#include "Precompiled.h"
#include "PlayerControllerSystem.h"
#include "Main.h"
#include "ForceManager.h"
#include "EventDispatcher.h"
#include "ComponentList.h"
#include "KeyEvent.h"
#include "GameEvent.h"
#include "GraphicsEvent.h"
#include "PickingManager.h"
#include "MenuControllerSystem.h"

namespace Rogue
{
	PlayerControllerSystem::PlayerControllerSystem()
		:System(SystemID::id_PLAYERCONTROLLERSYSTEM), m_isInLight{ 0.0f }, m_grounded{ false }
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
		//For PlayerControllerSystem Timer
		/*if (m_ballTimer > 0.0f)
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
		}*/

		if (!g_engine.m_coordinator.GameIsActive())
		{
			if (m_timedEntities.size())
				ClearTimedEntities();
			return;
		}

		//if (!m_timedEntities.size())
		//{
			//m_ballCooldown -= g_deltaTime * g_engine.GetTimeScale();
			//if (m_ballCooldown < 0.0f)
			//{
			//	ClearTimedEntities();
			//}
		//}
		m_isInLight -= g_deltaTime * g_engine.GetTimeScale();

		//To update all timed entities
		/*for (auto timedEntityIt = m_timedEntities.begin(); timedEntityIt != m_timedEntities.end(); ++timedEntityIt)
		{
			timedEntityIt->m_durationLeft -= g_deltaTime * g_engine.GetTimeScale();
			if (timedEntityIt->m_durationLeft < 0.0f)
			{
				ClearTimedEntities();

				if (m_timedEntities.size() == 0)
					break;
			}
		}*/

		std::cout << m_grounded << std::endl;

		const float c_stopFactor = 10.0f;

		for (Entity entity : m_entities)
		{
			auto& ctrl = g_engine.m_coordinator.GetComponent<PlayerControllerComponent>(entity);
			auto& rigidbody = g_engine.m_coordinator.GetComponent<RigidbodyComponent>(entity);
			 
			if(ctrl.GetMoveState() == MoveState::e_stop)
				ForceManager::instance().RegisterForce(entity, Vec2(rigidbody.getVelocity().x * -c_stopFactor, 0.0f));
		}

	}

	void PlayerControllerSystem::Receive(Event* ev)
	{
		//Statement here to make sure all commands only apply if game is not running
		if (!g_engine.m_coordinator.GameIsActive())
			return;

		if (m_entities.begin() == m_entities.end())
			return;

		switch (ev->GetEventType())
		{
		case EventType::EvOnCollisionExit:
		{
			EntCollisionExitEvent* collisionExit = dynamic_cast<EntCollisionExitEvent*>(ev);
			return;
		}

		case EventType::EvOnCollisionStay:
		{
			EntCollisionStayEvent* collisionStay = dynamic_cast<EntCollisionStayEvent*>(ev);

			Entity player;
			Entity ground;
			Vec2 playerTrans, groundTrans;
			Vec2 playerScale, groundScale;

			HierarchyInfo infoA = g_engine.m_coordinator.GetHierarchyInfo(collisionStay->GetEntityID());
			HierarchyInfo infoB = g_engine.m_coordinator.GetHierarchyInfo(collisionStay->GetOtherEntity());

			if (infoA.m_tag == "Player")
			{
				if (infoB.m_tag == "Ground")
				{
					player = collisionStay->GetEntityID();
					playerTrans = collisionStay->GetAPos();
					playerScale = collisionStay->GetScaleA();

					ground = collisionStay->GetOtherEntity();
					groundTrans = collisionStay->GetBPos();
					groundScale = collisionStay->GetScaleB();
				}
			}
			else if (infoB.m_tag == "Player")
			{
				if (infoA.m_tag == "Ground")
				{
					ground = collisionStay->GetEntityID();
					groundTrans = collisionStay->GetAPos();
					groundScale = collisionStay->GetScaleA();

					player = collisionStay->GetOtherEntity();
					playerTrans = collisionStay->GetBPos();
					playerScale = collisionStay->GetScaleB();
				}
				else
					return;
			}
			else
				return;

			if (playerTrans.y - playerScale.y < groundTrans.y + groundScale.y)
				m_grounded = true;
			else
				m_grounded = false;

			return;
		}
		case EventType::EvMouseMoved:
		{
			MouseMoveEvent* mouseMove = dynamic_cast<MouseMoveEvent*>(ev);
			KeyPress keycode = mouseMove->GetKeyCode();

			return;
		}

		case EventType::EvKeyTriggered:
		{
			KeyTriggeredEvent* keytriggeredevent = dynamic_cast<KeyTriggeredEvent*>(ev);
			KeyPress keycode = keytriggeredevent->GetKeyCode();

			if (keycode == KeyPress::KeyF5 && g_engine.GetIsFocused())
				g_engine.m_coordinator.ToggleEditorIsRunning();

			else if (keycode == KeyPress::KeyF6 && g_engine.GetIsFocused())
				g_engine.ToggleVSync();

			else if (keycode == KeyPress::MB1 && g_engine.GetIsFocused())
			{
				for (Entity entity : m_entities)
				{
					auto& PlayerControllable = g_engine.m_coordinator.GetComponent<PlayerControllerComponent>(entity);
					g_engine.SetTimeScale(PlayerControllable.GetSlowTime());
				}
			}

			else if (keycode == KeyPress::MB2 && g_engine.GetIsFocused())
			{
				if (m_entities.size() && m_timedEntities.size() && m_isInLight < 0.0f)
				{
					//By right correct way of doing this
					CreateTeleportEvent(g_engine.m_coordinator.GetComponent<TransformComponent>(m_timedEntities.begin()->m_entity).GetPosition());
					/*if (keycode == KeyPress::MB2)
						g_engine.m_coordinator.GetComponent<TransformComponent>(*m_entities.begin()).setPosition(
							g_engine.m_coordinator.GetComponent<TransformComponent>(m_timedEntities.begin()->m_entity).getPosition());*/

					ClearTimedEntities();
				}
			}
			else if (keycode == KeyPress::MB3 && g_engine.GetIsFocused())
			{
				ClearTimedEntities();
			}

			else if (keycode == KeyPress::KeySpace && g_engine.GetIsFocused())
			{
  				if (!m_grounded)
					return;

				for (std::set<Entity>::iterator iEntity = m_entities.begin(); iEntity != m_entities.end(); ++iEntity)
				{
					//For 1st entity
					if (iEntity == m_entities.begin() && g_engine.m_coordinator.ComponentExists<RigidbodyComponent>(*iEntity))
					{
						ForceManager::instance().RegisterForce(*iEntity, Vec2(0.0f, 35000.0f));
						
						// Reset boolean for grounded
					}
				}
				m_grounded = false;
			}

			else if (keycode == KeyPress::KeyEsc && g_engine.GetIsFocused())
			{
				g_engine.m_coordinator.SetPauseState(true);
				g_engine.m_coordinator.GetSystem<MenuControllerSystem>()->ToggleUIMenuObjs();
			}

			//if (keycode == KeyPress::Numpad9)
			//{
			//	RE_ASSERT(false, "CRASH ON PURPOSE");
			//}
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
					if (keycode == KeyPress::KeyA && g_engine.GetIsFocused())
					{
						auto& ctrler = g_engine.m_coordinator.GetComponent<PlayerControllerComponent>(*iEntity);
						ctrler.SetMoveState(MoveState::e_left);

						Event* ev = new EntMoveEvent{ *iEntity, true, -1.0f, 0.0f };
						ev->SetSystemReceivers((int)SystemID::id_PHYSICSSYSTEM);
						ev->SetSystemReceivers((int)SystemID::id_GRAPHICSSYSTEM);
						EventDispatcher::instance().AddEvent(ev);
					}
					else if (keycode == KeyPress::KeyD && g_engine.GetIsFocused())
					{
						auto& ctrler = g_engine.m_coordinator.GetComponent<PlayerControllerComponent>(*iEntity);
						ctrler.SetMoveState(MoveState::e_right);

						Event* ev = new EntMoveEvent{ *iEntity, true, 1.0f, 0.0f };
						ev->SetSystemReceivers((int)SystemID::id_PHYSICSSYSTEM);
						ev->SetSystemReceivers((int)SystemID::id_GRAPHICSSYSTEM);
						EventDispatcher::instance().AddEvent(ev);
					}
					else if (keycode == KeyPress::KeyW && g_engine.GetIsFocused())
					{
						//ForceManager::instance().RegisterForce(*iEntity, Vec2::s_unitY * playerX, g_fixedDeltaTime);
					}
					else if (keycode == KeyPress::KeyS && g_engine.GetIsFocused())
					{
						//ForceManager::instance().RegisterForce(*iEntity, -Vec2::s_unitY * playerX, g_fixedDeltaTime);
					}

					auto& transform = g_engine.m_coordinator.GetComponent<TransformComponent>(*iEntity);
					/*if (keycode == KeyPress::Numpad4)
					{
						transform.offSetScale(Vec2(100.0f, 100.0f) * g_deltaTime * 60.0f);
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
					}*/
				}
				//For 2nd Entity
				//To be deleted in future (Since by right only one entityt should exist here for player
				else if (iEntity != m_entities.begin())
				{
					auto& rigidbody = g_engine.m_coordinator.GetComponent<RigidbodyComponent>(*iEntity);
					if (keycode == KeyPress::KeyArrowLeft && g_engine.GetIsFocused())
					{
						rigidbody.addForce(Vec2(-100.0f, 0.0f) * g_deltaTime * 1000.0f);
						//RE_INFO("Move B Left!");

					}
					else if (keycode == KeyPress::KeyArrowRight && g_engine.GetIsFocused())
					{
						rigidbody.addForce(Vec2(100.0f, 0.0f) * g_deltaTime * 1000.0f);
						//RE_INFO("Move B Right!");
					}
					else if (keycode == KeyPress::KeyArrowUp && g_engine.GetIsFocused())
					{
						rigidbody.addForce(Vec2(0.0f, 100.0f) * g_deltaTime * 1000.0f);
						//RE_INFO("Move B Up!");

					}
					else if (keycode == KeyPress::KeyArrowDown && g_engine.GetIsFocused())
					{
						rigidbody.addForce(Vec2(0.0f, -100.0f) * g_deltaTime * 1000.0f);
						//RE_INFO("Move B Down!");
					}

					auto& transform = g_engine.m_coordinator.GetComponent<TransformComponent>(*iEntity);
					/*if (keycode == KeyPress::Numpad8)
					{
						transform.setPosition(Vec2(200.0f, 0.0f));
					}*/
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

			if (keycode == KeyPress::MB1 && g_engine.GetIsFocused())
			{
				if (!m_timedEntities.size() /*&& m_ballCooldown < 0.0f*/)
				{
					CreateBallAttack();
					//m_ballTimer = 1.0f;
					//m_ballCooldown = 1.0f;
					//RE_INFO("CLICKCLICK");

					if (g_engine.m_coordinator.ComponentExists<AnimationComponent>(*m_entities.begin()))
						g_engine.m_coordinator.GetComponent<AnimationComponent>(*m_entities.begin()).setIsAnimating(true);

					AudioManager::instance().loadSound("Resources/Sounds/[Shoot Projectile]SCI-FI-WHOOSH_GEN-HDF-20864.ogg", 0.86f, false).Play();
					AudioManager::instance().loadSound("Resources/Sounds/[Ela Appear]SCI-FI-WHOOSH_GEN-HDF-20870.ogg", 0.3f, false).Play(0.3f);
				}
				g_engine.SetTimeScale(1.0f);
			}
			if ((keycode == KeyPress::KeyA && g_engine.GetIsFocused() )|| (keycode == KeyPress::KeyD && g_engine.GetIsFocused()))
			{
				auto& ctrler = g_engine.m_coordinator.GetComponent<PlayerControllerComponent>(*m_entities.begin());

				ctrler.SetMoveState(MoveState::e_stop);
			}
			return;
		} // switch (ev->GetEventType())
		} // Receive
	}

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
		//Check if timed entities exist first. If it doesn't, this call is redundant
		if (!m_timedEntities.size())
			return;

		//Deleting all local timedEntities
		for (TimedEntity& entity : m_timedEntities)
		{
			g_engine.m_coordinator.DestroyEntity(entity.m_entity);
		}

		auto& activeObjects = g_engine.m_coordinator.GetActiveObjects();
		for (auto iterator = activeObjects.begin(); iterator != activeObjects.end(); ++iterator)
		{
			if (*iterator == m_timedEntities.begin()->m_entity)
			{
				activeObjects.erase(iterator);
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

	void PlayerControllerSystem::setInLight(float duration)
	{
		m_isInLight = duration;
	}

	void PlayerControllerSystem::CreateBallAttack()
	{
		for (Entity entity : m_entities)
		{
			std::ostringstream strstream;
			Entity ball = g_engine.m_coordinator.CreateEntity();
			auto& trans = g_engine.m_coordinator.GetComponent<TransformComponent>(entity);
			Vec2 playerPos = trans.GetPosition();
			Vec2 cursorPos = PickingManager::instance().GetWorldCursor();

			Vec2 ballDir{ cursorPos.x - playerPos.x, cursorPos.y - playerPos.y };
			Vec2Normalize(ballDir, ballDir);

			strstream << playerPos.x + ballDir.x * POSITION_RELATIVITY << ";"
				<< playerPos.y + ballDir.y * POSITION_RELATIVITY << ";"
				<< "20;20;0";

			TransformComponent& ballTransform = g_engine.m_coordinator.CreateComponent<TransformComponent>(ball);

			ballTransform.Deserialize(strstream.str());

			SpriteComponent& sprite = g_engine.m_coordinator.CreateComponent<SpriteComponent>(ball);
			sprite.Deserialize("Resources/Assets/Projectile.png;1;1;1;1;1");

			RigidbodyComponent& rigidbody = g_engine.m_coordinator.CreateComponent<RigidbodyComponent>(ball);
			rigidbody.Deserialize("0;0;0;0;1;1;0;0.5;0.8;0.01");

			ForceManager::instance().RegisterForce(ball, Vec2(ballDir.x * FORCE_FACTOR, ballDir.y * FORCE_FACTOR), g_fixedDeltaTime);

			BoxCollider2DComponent& boxCollider = g_engine.m_coordinator.CreateComponent<BoxCollider2DComponent>(ball);
			boxCollider.Deserialize("0;0;0;0;0");

			HierarchyInfo newInfo(ball, "Ball");
			g_engine.m_coordinator.GetActiveObjects().push_back(ball);
			g_engine.m_coordinator.GetHierarchyInfo(ball) = newInfo;

			AddToTimedEntities(ball, 0.8f);
			break;
		}
	}


}