/* Start Header ************************************************************************/
/*!
\file           PlayerControllerSystem.cpp
\project        Exale
\author         Chan Wai Kit Terence, c.terence, 440005918 (100%)
\par            c.terence\@digipen.edu
\date           1 December,2019
\brief          This file contains the function definitions for PlayerControllerSystem

All content (C) 2019 DigiPen (SINGAPORE) Corporation, all rights
reserved.

Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header **************************************************************************/
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
#include "PlayerStatusManager.h"

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
			if (m_teleports.size())
				ClearTeleportEntities();
			return;
		}

		if (m_teleports.size())
		{
			for (TimedEntity& ent : m_teleports)
			{
				ent.m_durationLeft -= g_deltaTime * g_engine.GetTimeScale();
			}

			if (m_teleports.back().m_durationLeft < 0.0f)
				ClearTeleportEntities();
		}

		//Timers 
		if (PLAYER_STATUS.GetTeleportCharge() < PLAYER_STATUS.GetMaxTeleportCharge())
			PLAYER_STATUS.IncrementTeleportCharge(g_deltaTime * g_engine.GetTimeScale());

		PLAYER_STATUS.SetInLightDur(PLAYER_STATUS.GetInLightDur() - g_deltaTime * g_engine.GetTimeScale());
		PLAYER_STATUS.DecrementTeleportDelay(g_deltaTime * g_engine.GetTimeScale());

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


		const float c_stopFactor = 10.0f;

		for (Entity entity : m_entities)
		{
			auto& player = g_engine.m_coordinator.GetComponent<PlayerControllerComponent>(entity);
			auto& rigidbody = g_engine.m_coordinator.GetComponent<RigidbodyComponent>(entity);
				
			if (PLAYER_STATUS.GetHitchhikedEntity() != -1)
			{
				auto& transform = g_engine.m_coordinator.GetComponent<TransformComponent>(entity);
				auto& parentTransform = g_engine.m_coordinator.GetComponent<TransformComponent>(PLAYER_STATUS.GetHitchhikedEntity());
				transform.setPosition(parentTransform.GetPosition());
				break;
			}

			if (player.GetMoveState() == MoveState::e_stop)
				ForceManager::instance().RegisterForce(entity, Vec2(rigidbody.getVelocity().x * -c_stopFactor, 0.0f));
				//rigidbody.addForce(Vec2(rigidbody.getVelocity().x * -c_stopFactor, 0.0f));

			player.m_jumpTimer -= g_deltaTime * g_engine.GetTimeScale();
		}

	}

	void PlayerControllerSystem::Receive(Event* ev)
	{
		//Statement here to make sure all commands only apply if game is not running
		if (!g_engine.m_coordinator.GameIsActive())
		{
			g_engine.SetTimeScale(1.0f);
			return;
		}

		if (m_entities.begin() == m_entities.end())
			return;

		switch (ev->GetEventType())
		{
		case EventType::EvMouseMoved:
		{
			MouseMoveEvent* mouseMove = dynamic_cast<MouseMoveEvent*>(ev);
			KeyPress keycode = mouseMove->GetKeyCode();

			return;
		}

		case EventType::EvEntityChangeSprite:
		{
			EntChangeSpriteEvent* event = dynamic_cast<EntChangeSpriteEvent*>(ev);
			SpriteComponent& sprite = g_engine.m_coordinator.GetComponent<SpriteComponent>(*m_entities.begin());
			sprite.setTexture(event->GetFilePath().c_str());

			return;
		}

		case EventType::EvKeyTriggered:
		{
			KeyTriggeredEvent* keytriggeredevent = dynamic_cast<KeyTriggeredEvent*>(ev);
			KeyPress keycode = keytriggeredevent->GetKeyCode();

			if (g_engine.GetIsFocused())
			{
				if (keycode == KeyPress::KeyF5)
					g_engine.m_coordinator.ToggleEditorIsRunning();

				else if (keycode == KeyPress::KeyF6)
					g_engine.ToggleVSync();

				else if (keycode == KeyPress::MB1 && g_engine.GetIsFocused())
				{
					//For slow mo
					for (Entity entity : m_entities)
					{
						auto& PlayerControllable = g_engine.m_coordinator.GetComponent<PlayerControllerComponent>(entity);
						if (!PlayerControllable.m_grounded)
							g_engine.SetTimeScale(PlayerControllable.GetSlowTime());
					}

					//For teleport
					if (m_entities.size() && m_timedEntities.size() && PLAYER_STATUS.GetInLightDur() < 0.0f && PLAYER_STATUS.GetTeleportCharge() > 1.0f)
					{
						//TimedEntity ent(g_engine.m_coordinator.cloneArchetypes("TeleportSprite", false), 0.5f);
						//m_teleports.push_back(ent);
						//if (g_engine.m_coordinator.ComponentExists<TransformComponent>(m_teleports.back().m_entity))
						//{
						//	TransformComponent& transform = g_engine.m_coordinator.GetComponent<TransformComponent>(m_teleports.back().m_entity);
						//	transform.setPosition(g_engine.m_coordinator.GetComponent<TransformComponent>(m_timedEntities.begin()->m_entity).GetPosition());
						//}
						////By right correct way of doing this
						////CreateTeleportEvent(g_engine.m_coordinator.GetComponent<TransformComponent>(m_timedEntities.begin()->m_entity).GetPosition());
						///*if (keycode == KeyPress::MB2)
						//	g_engine.m_coordinator.GetComponent<TransformComponent>(*m_entities.begin()).setPosition(
						//		g_engine.m_coordinator.GetComponent<TransformComponent>(m_timedEntities.begin()->m_entity).getPosition());*/

						//ClearTimedEntities();
						//PLAYER_STATUS.IncrementTeleportCharge(-1.0f);
						//PLAYER_STATUS.SetTeleportDelay(TELEPORT_DELAY);
					}
				}

				else if (keycode == KeyPress::MB2)
				{
					//For Hitchhiking
				}
				else if (keycode == KeyPress::MB3)
				{
					ClearTimedEntities();
				}

				else if (keycode == KeyPress::KeyQ)
				{
					PLAYER_STATUS.ToggleLightStatus();
				}

				else if (keycode == KeyPress::KeySpace)
				{
					for (std::set<Entity>::iterator iEntity = m_entities.begin(); iEntity != m_entities.end(); ++iEntity)
					{
						auto& player = g_engine.m_coordinator.GetComponent<PlayerControllerComponent>(*iEntity);
						auto& rigidbody = g_engine.m_coordinator.GetComponent<RigidbodyComponent>(*iEntity);

						if (!player.m_grounded || player.m_jumpTimer > 0.0f)
							return;

						ForceManager::instance().RegisterForce(*iEntity, Vec2(0.0f, 35000.0f));
						//rigidbody.addForce(Vec2(0.0f, 35000.0f));

						// Reset boolean for grounded
						player.m_grounded = false;
						player.m_jumpTimer = PLAYER_STATUS.GetJumpMaxTimer();
					}
				}

				else if (keycode == KeyPress::KeyEsc)
				{
					g_engine.m_coordinator.SetPauseState(true);
					g_engine.m_coordinator.GetSystem<MenuControllerSystem>()->ToggleUIMenuObjs();
				}

				//if (keycode == KeyPress::Numpad9)
				//{
				//	RE_ASSERT(false, "CRASH ON PURPOSE");
				//}
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
					if (g_engine.GetIsFocused())
					{
						if (keycode == KeyPress::KeyA)
						{
							auto& ctrler = g_engine.m_coordinator.GetComponent<PlayerControllerComponent>(*iEntity);
							ctrler.SetMoveState(MoveState::e_left);

							Event* ev = new EntMoveEvent{ *iEntity, true, -1.0f, 0.0f };
							ev->SetSystemReceivers((int)SystemID::id_PHYSICSSYSTEM);
							ev->SetSystemReceivers((int)SystemID::id_GRAPHICSSYSTEM);
							EventDispatcher::instance().AddEvent(ev);
						}
						else if (keycode == KeyPress::KeyD)
						{
							auto& ctrler = g_engine.m_coordinator.GetComponent<PlayerControllerComponent>(*iEntity);
							ctrler.SetMoveState(MoveState::e_right);

							Event* ev = new EntMoveEvent{ *iEntity, true, 1.0f, 0.0f };
							ev->SetSystemReceivers((int)SystemID::id_PHYSICSSYSTEM);
							ev->SetSystemReceivers((int)SystemID::id_GRAPHICSSYSTEM);
							EventDispatcher::instance().AddEvent(ev);
						}

						// Skip level
						else if (keycode == KeyPress::KeyI)
						{
							if (SceneManager::instance().getCurrentFileName() == "Level 12.json")
							{
								if (g_engine.m_coordinator.ComponentExists<TransformComponent>(*iEntity))
								{
									auto& trans = g_engine.m_coordinator.GetComponent<TransformComponent>(*iEntity);

									trans.setPosition(Vec2(480.0f, -321.0f));
								}
							}

							if (SceneManager::instance().getCurrentFileName() == "Level 9.json")
							{
								if (g_engine.m_coordinator.ComponentExists<TransformComponent>(*iEntity))
								{
									auto& trans = g_engine.m_coordinator.GetComponent<TransformComponent>(*iEntity);

									trans.setPosition(Vec2(1150.0f, 578.0f));
								}
							}
							
						}
						else if (keycode == KeyPress::KeyS)
						{
							//ForceManager::instance().RegisterForce(*iEntity, -Vec2::s_unitY * playerX, g_fixedDeltaTime);
						}

						// Reset level
						else if (keycode == KeyPress::KeyO)
						{
							if (SceneManager::instance().getCurrentFileName() == "Level 12.json")
							{
								if (g_engine.m_coordinator.ComponentExists<TransformComponent>(*iEntity))
								{
									auto& trans = g_engine.m_coordinator.GetComponent<TransformComponent>(*iEntity);

									trans.setPosition(Vec2(-530.0f, 287.0f));
								}
							}
							else if (SceneManager::instance().getCurrentFileName() == "Level 9.json")
							{
								if (g_engine.m_coordinator.ComponentExists<TransformComponent>(*iEntity))
								{
									auto& trans = g_engine.m_coordinator.GetComponent<TransformComponent>(*iEntity);

									trans.setPosition(Vec2(-1557.0f, 258.0f));
								}
							}
						}
					}
					
				}
			} // End of Entity for-loop

			return;
		}
		case EventType::EvKeyReleased:
		{
			KeyReleaseEvent* KeyReleaseEv = dynamic_cast<KeyReleaseEvent*>(ev);
			KeyPress keycode = KeyReleaseEv->GetKeyCode();

			if (!g_engine.m_coordinator.GameIsActive())
				return;
			if (g_engine.GetIsFocused())
			{
				if (keycode == KeyPress::MB1)
				{
					if (!m_timedEntities.size() && PLAYER_STATUS.GetInLightDur() < 0.0f && PLAYER_STATUS.GetTeleportDelay() < 0.0f && PLAYER_STATUS.GetTeleportCharge() >= 1.0f)
					{
						//CreateBallAttack();
						//auto& player = g_engine.m_coordinator.GetComponent<PlayerControllerComponent>(*m_entities.begin());
						//if (player.m_grounded)
						//	PLAYER_STATUS.SetTeleportCharge(PLAYER_STATUS.GetMaxTeleportCharge());
						Teleport();

						if (g_engine.m_coordinator.ComponentExists<AnimationComponent>(*m_entities.begin()))
							g_engine.m_coordinator.GetComponent<AnimationComponent>(*m_entities.begin()).setIsAnimating(true);

						AudioManager::instance().loadSound("Resources/Sounds/[Shoot Projectile]SCI-FI-WHOOSH_GEN-HDF-20864.ogg", 0.86f, false).Play();
						AudioManager::instance().loadSound("Resources/Sounds/[Ela Appear]SCI-FI-WHOOSH_GEN-HDF-20870.ogg", 0.3f, false).Play();
					}
					g_engine.SetTimeScale(1.0f);
				}
				if ((keycode == KeyPress::KeyA ) || (keycode == KeyPress::KeyD))
				{
					auto& ctrler = g_engine.m_coordinator.GetComponent<PlayerControllerComponent>(*m_entities.begin());

					ctrler.SetMoveState(MoveState::e_stop);
				}

				else if (keycode == KeyPress::KeySpace)
				{
					for (Entity entity : m_entities)
					{
						PlayerControllerComponent& player = g_engine.m_coordinator.GetComponent<PlayerControllerComponent>(entity);

						player.m_grounded = false;
					}
				}
			}
			
			return;
		}
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

			for (Entity entity : m_entities)
			{
				auto& player = g_engine.m_coordinator.GetComponent<PlayerControllerComponent>(entity);

				if (playerTrans.y - playerScale.y < groundTrans.y + groundScale.y)
					player.m_grounded = true;
				else
					player.m_grounded = false;
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

	void PlayerControllerSystem::ClearTeleportEntities()
	{
		if (!m_teleports.size())
			return;

		auto& activeObjects = g_engine.m_coordinator.GetActiveObjects();
		for (TimedEntity& entity : m_teleports)
		{
			g_engine.m_coordinator.DestroyEntity(entity.m_entity);

			//for (auto iterator = activeObjects.begin(); iterator != activeObjects.end(); ++iterator)
			//{
			//	if (*iterator == m_teleports.begin()->m_entity)
			//	{
			//		activeObjects.erase(iterator);
			//		break;
			//	}
			//}
		}

		m_teleports.clear();
	}

	void PlayerControllerSystem::CreateTeleportEvent(Vec2 newPosition)
	{
		EntTeleportEvent* event = new EntTeleportEvent(*m_entities.begin(), newPosition);
		event->SetSystemReceivers((int)SystemID::id_PHYSICSSYSTEM);
		EventDispatcher::instance().AddEvent(event);
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
				<< "30;30;0";

			TransformComponent& ballTransform = g_engine.m_coordinator.CreateComponent<TransformComponent>(ball);

			ballTransform.Deserialize(strstream.str());

			SpriteComponent& sprite = g_engine.m_coordinator.CreateComponent<SpriteComponent>(ball);
			sprite.Deserialize("Resources/Assets/Projectile.png;1;1;1;1;1");

			RigidbodyComponent& rigidbody = g_engine.m_coordinator.CreateComponent<RigidbodyComponent>(ball);
			rigidbody.Deserialize("0;0;0;0;1;1;0;0.5;0.8;0.01");

			ForceManager::instance().RegisterForce(ball, ballDir * FORCE_FACTOR);
			//rigidbody.addForce(Vec2(ballDir.x * FORCE_FACTOR, ballDir.y * FORCE_FACTOR));

			BoxCollider2DComponent& boxCollider = g_engine.m_coordinator.CreateComponent<BoxCollider2DComponent>(ball);

			ColliderComponent collider = g_engine.m_coordinator.CreateComponent<ColliderComponent>(ball);
			collider.Deserialize("BOX");

			HierarchyInfo newInfo(ball, "Ball");
			g_engine.m_coordinator.GetActiveObjects().push_back(ball);
			g_engine.m_coordinator.GetHierarchyInfo(ball) = newInfo;

			AddToTimedEntities(ball, 0.8f);
			break;
		}
	}

	void PlayerControllerSystem::Teleport()
	{
		PLAYER_STATUS.IncrementTeleportCharge(-1.0f);

		TransformComponent& playerTransform = g_engine.m_coordinator.GetComponent<TransformComponent>(*m_entities.begin());
		Vec2 initialPos = playerTransform.GetPosition();
		Vec2 endPos = PickingManager::instance().GetWorldCursor();
		Vec2 cursor = endPos;
		Vec2 calculatedPos = initialPos;

		//Calculating max cursor distance value
		cursor -= initialPos;
		Vec2Normalize(cursor, cursor);
		cursor *= playerTransform.GetScale().x * 3;
		cursor += initialPos;

		//Prevent going past cursor when clicking close
		if (Vec2SqDistance(initialPos, endPos) < Vec2SqDistance(initialPos, cursor))
			cursor = endPos;

		for (size_t checkCount = 0; checkCount < 3; ++checkCount)
		{
			calculatedPos += ((cursor - initialPos) / 3);
		}

		CreateTeleportEvent(calculatedPos);
	}


}