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
#include "ParentEvent.h"
#include "PickingManager.h"
#include "MenuControllerSystem.h"
#include "PlayerStatusManager.h"
#include "BoxCollisionSystem.h"
#include "CollisionManager.h"

namespace Rogue
{
	PlayerControllerSystem::PlayerControllerSystem()
		:System(SystemID::id_PLAYERCONTROLLERSYSTEM), m_ignoreFrameEvent{ false }
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
		m_ignoreFrameEvent = false;

		if (PLAYER_STATUS.GetPlayerEntity() == MAX_ENTITIES && m_entities.size())
		{
			PLAYER_STATUS.SetPlayerEntity(*m_entities.begin());
		}

		if (!g_engine.m_coordinator.GameIsActive())
		{
			if (m_timedEntities.size())
				ClearTimedEntities();
			if (m_teleports.size())
				ClearTeleportEntities();
			return;
		}

		if (PLAYER_STATUS.InSlowMo())
		{
			if (PLAYER_STATUS.GetIndicator() == MAX_ENTITIES && PLAYER_STATUS.GetPlayerEntity() != MAX_ENTITIES)
			{
				PLAYER_STATUS.SetIndicator(g_engine.m_coordinator.cloneArchetypes("Indicator", false));

				ParentSetEvent* parent = new ParentSetEvent(*m_entities.begin(), PLAYER_STATUS.GetIndicator());
				parent->SetSystemReceivers((int)SystemID::id_PARENTCHILDSYSTEM);
				EventDispatcher::instance().AddEvent(parent);

				return;
			}

			//If it reaches here, that means indicator exists already
			if (g_engine.m_coordinator.ComponentExists<ChildComponent>(PLAYER_STATUS.GetIndicator()))
			{
				ChildComponent& comp = g_engine.m_coordinator.GetComponent <ChildComponent>(PLAYER_STATUS.GetIndicator());
				comp.SetIsFollowing(true);

				Vec2 calculatedPos = GetTeleportRaycast();

				if (g_engine.m_coordinator.ComponentExists<TransformComponent>(PLAYER_STATUS.GetIndicator()))
				{
					TransformComponent& transform = g_engine.m_coordinator.GetComponent<TransformComponent>(PLAYER_STATUS.GetIndicator());
					Vec2 vecOfChange = Vec2(g_engine.m_coordinator.GetComponent<TransformComponent>(*m_entities.begin()).GetPosition() - calculatedPos);
					transform.setPosition(calculatedPos + vecOfChange / 2);
					transform.setRotation(atan(vecOfChange.y / vecOfChange.x));
					//No need to set scale
					//transform.setScale(Vec2(vecOfChange.x, vecOfChange.y));
				}
			}
		}

		if (m_teleports.size())
		{
			for (TimedEntity& ent : m_teleports)
			{
				ent.m_durationLeft -= g_deltaTime * g_engine.GetTimeScale();

				if (ent.m_durationLeft < 0.0f)
					ClearTeleportEntities(ent.m_entity);
			}
		}

		//Timers 
		//if (PLAYER_STATUS.GetTeleportCharge() < PLAYER_STATUS.GetMaxTeleportCharge())
			//PLAYER_STATUS.IncrementTeleportCharge(g_deltaTime * g_engine.GetTimeScale());

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
				
			if (PLAYER_STATUS.GetHitchhikedEntity() != MAX_ENTITIES)
			{
				auto& transform = g_engine.m_coordinator.GetComponent<TransformComponent>(entity);
				auto& parentTransform = g_engine.m_coordinator.GetComponent<TransformComponent>(PLAYER_STATUS.GetHitchhikedEntity());
				transform.setPosition(parentTransform.GetPosition());
				break;
			}

			if (player.GetMoveState() == MoveState::e_stop)
				ForceManager::instance().RegisterForce(entity, Vec2(rigidbody.getVelocity().x * -c_stopFactor, 0.0f));
				//rigidbody.addForce(Vec2(rigidbody.getVelocity().x * -c_stopFactor, 0.0f));

			if (player.m_grounded)
				PLAYER_STATUS.SetTeleportCharge(3.0f);

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
			if (g_engine.m_coordinator.ComponentExists<SpriteComponent>(event->GetEntityID()))
			{
				SpriteComponent& sprite = g_engine.m_coordinator.GetComponent<SpriteComponent>(event->GetEntityID());
				sprite.setTexture(event->GetFilePath().c_str());
				sprite.setTexturePath(event->GetFilePath().c_str());
			}

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
						{
							g_engine.SetTimeScale(PlayerControllable.GetSlowTime());
							PLAYER_STATUS.SetSlowMo();
						}
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

				else if (keycode == KeyPress::KeyE)
				{
					ToggleMode();
				}

				else if (keycode == KeyPress::KeySpace)
				{
					for (std::set<Entity>::iterator iEntity = m_entities.begin(); iEntity != m_entities.end(); ++iEntity)
					{
						auto& player = g_engine.m_coordinator.GetComponent<PlayerControllerComponent>(*iEntity);
						auto& rigidbody = g_engine.m_coordinator.GetComponent<RigidbodyComponent>(*iEntity);

						if (!player.m_grounded || player.m_jumpTimer > 0.0f || PLAYER_STATUS.HasJumped())
							return;

						ForceManager::instance().RegisterForce(*iEntity, Vec2(0.0f, 35000.0f));
						//rigidbody.addForce(Vec2(0.0f, 35000.0f));

						// Reset boolean for grounded
						player.m_grounded = false;
						PLAYER_STATUS.SetHasJumped(true);
						player.m_jumpTimer = PLAYER_STATUS.GetJumpMaxTimer();

						ResetPlayerParent();
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

							MovingPlayer();
						}
						else if (keycode == KeyPress::KeyD)
						{
							auto& ctrler = g_engine.m_coordinator.GetComponent<PlayerControllerComponent>(*iEntity);
							ctrler.SetMoveState(MoveState::e_right);

							Event* ev = new EntMoveEvent{ *iEntity, true, 1.0f, 0.0f };
							ev->SetSystemReceivers((int)SystemID::id_PHYSICSSYSTEM);
							ev->SetSystemReceivers((int)SystemID::id_GRAPHICSSYSTEM);
							EventDispatcher::instance().AddEvent(ev);

							MovingPlayer();
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

									trans.setPosition(Vec2(155.0f, 55.0f));
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
						m_ignoreFrameEvent = true;
					}
					g_engine.SetTimeScale(1.0f);
					PLAYER_STATUS.SetSlowMo(false);

					//To reduce calculations
					if (PLAYER_STATUS.GetIndicator() != MAX_ENTITIES && g_engine.m_coordinator.ComponentExists<ChildComponent>(PLAYER_STATUS.GetIndicator()))
					{
						ChildComponent& comp = g_engine.m_coordinator.GetComponent <ChildComponent>(PLAYER_STATUS.GetIndicator());
						comp.SetIsFollowing(false);
					}
				}
				if ((keycode == KeyPress::KeyA ) || (keycode == KeyPress::KeyD))
				{
					auto& ctrler = g_engine.m_coordinator.GetComponent<PlayerControllerComponent>(*m_entities.begin());

					ctrler.SetMoveState(MoveState::e_stop);
				}

				else if (keycode == KeyPress::KeySpace)
				{
					//for (Entity entity : m_entities)
					//{
					//	PlayerControllerComponent& player = g_engine.m_coordinator.GetComponent<PlayerControllerComponent>(entity);

					//	player.m_grounded = false;
					//}
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
				if (infoB.m_tag == "Ground" || infoB.m_tag == "Platform")
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
				if (infoA.m_tag == "Ground" || infoB.m_tag == "Platform")
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

				//Bottom of player is lower than top of ground (Standing on top)
				//Bottom of player is above bottom of ground (Player is above ground)
				if (playerTrans.y - playerScale.y < groundTrans.y + groundScale.y /*&& playerTrans.y - playerScale.y / 2 > groundTrans.y - groundScale.y / 2*/ && !m_ignoreFrameEvent)
				{
					player.m_grounded = true;
					PLAYER_STATUS.SetHasJumped(false);
				}
				else if (infoA.m_tag == "Platform" || infoB.m_tag == "Platform")
				{
					player.m_grounded = true;
					PLAYER_STATUS.SetHasJumped(false);
					if (infoA.m_tag == "Platform")
					{
						ParentSetEvent* parent = new ParentSetEvent(infoA.m_Entity, entity);
						parent->SetSystemReceivers((int)SystemID::id_PARENTCHILDSYSTEM);
						EventDispatcher::instance().AddEvent(parent);
						PLAYER_STATUS.SetHitchhikeEntity(infoA.m_Entity);
						m_ignoreFrameEvent = true;
					}
					else //if (infoB.m_tag == "Platform")
					{
					}
				}
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
			if (entity.m_durationLeft < 0.0f)
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

	void PlayerControllerSystem::ClearTeleportEntities(Entity ent)
	{
		for (auto it = m_teleports.begin(); it != m_teleports.end(); ++it)
		{
			if (it->m_entity == ent)
			{
				m_teleports.erase(it);
				return;
			}
		}
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
		// In case player doesn't even exist lmao
		if (m_entities.begin() == m_entities.end())
			return;
    
		// In case player doesn't have a box collider
		if (!g_engine.m_coordinator.ComponentExists<BoxCollider2DComponent>(*m_entities.begin()) ||
			!g_engine.m_coordinator.ComponentExists<ColliderComponent>(*m_entities.begin()))
			return;

		PLAYER_STATUS.IncrementTeleportCharge(-1.0f);
		if (g_engine.m_coordinator.ComponentExists<PlayerControllerComponent>(*m_entities.begin()))
			g_engine.m_coordinator.GetComponent<PlayerControllerComponent>(*m_entities.begin()).m_grounded = false;

		Vec2 calculatedPos = GetTeleportRaycast();

		CreateTeleportEvent(calculatedPos);

		if (PLAYER_STATUS.GetHitchhikedEntity() != MAX_ENTITIES)
		{
			ParentResetEvent* parentReset = new ParentResetEvent(*m_entities.begin());
			parentReset->SetSystemReceivers((int)SystemID::id_PARENTCHILDSYSTEM);
			EventDispatcher::instance().AddEvent(parentReset);
			PLAYER_STATUS.SetHitchhikeEntity(MAX_ENTITIES);
		}

		//For teleport VFX
		TimedEntity ent(g_engine.m_coordinator.cloneArchetypes("TeleportSprite", false), 1.0f);
		m_teleports.push_back(ent);
		if (g_engine.m_coordinator.ComponentExists<TransformComponent>(m_teleports.back().m_entity))
		{
			TransformComponent& transform = g_engine.m_coordinator.GetComponent<TransformComponent>(m_teleports.back().m_entity);
			Vec2 vecOfChange = Vec2(g_engine.m_coordinator.GetComponent<TransformComponent>(*m_entities.begin()).GetPosition() - calculatedPos);
			transform.setPosition(calculatedPos + vecOfChange/ 2);
			transform.setRotation(atan(vecOfChange.y / vecOfChange.x));
			//No need to set scale
			//transform.setScale(Vec2(vecOfChange.x, vecOfChange.y));
		}

		//For teleport SFX
		if (g_engine.m_coordinator.ComponentExists<AnimationComponent>(*m_entities.begin()))
			g_engine.m_coordinator.GetComponent<AnimationComponent>(*m_entities.begin()).setIsAnimating(true);

		AudioManager::instance().loadSound("Resources/Sounds/[Shoot Projectile]SCI-FI-WHOOSH_GEN-HDF-20864.ogg", 0.86f, false).Play();
		AudioManager::instance().loadSound("Resources/Sounds/[Ela Appear]SCI-FI-WHOOSH_GEN-HDF-20870.ogg", 0.3f, false).Play();
	}

	Vec2 PlayerControllerSystem::GetTeleportRaycast()
	{
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
		//if (Vec2SqDistance(initialPos, endPos) < Vec2SqDistance(initialPos, cursor))
		//	cursor = endPos;

		for (size_t checkCount = 0; checkCount < 3; ++checkCount)
		{
			calculatedPos += ((cursor - initialPos) / 3);
		}

		const std::set<Entity>& boxEntities = g_engine.m_coordinator.GetSystem<BoxCollisionSystem>()->GetEntitySet();
		BoxCollider2DComponent& playerCollider = g_engine.m_coordinator.GetComponent<BoxCollider2DComponent>(*m_entities.begin());
		ColliderComponent& playerGCollider = g_engine.m_coordinator.GetComponent<ColliderComponent>(*m_entities.begin());
		LineSegment teleportLine = LineSegment(playerTransform.GetPosition(), calculatedPos);
		Vec2 teleportVec = calculatedPos - playerTransform.GetPosition();

		for (Entity entity : boxEntities)
		{
			if (entity == *m_entities.begin())
				continue;

			BoxCollider2DComponent& boxCollider = g_engine.m_coordinator.GetComponent<BoxCollider2DComponent>(entity);
			if (boxCollider.GetCollisionMode() != CollisionMode::e_awake)
				continue;

			ColliderComponent& boxGCollider = g_engine.m_coordinator.GetComponent<ColliderComponent>(entity);

			if (!CollisionManager::instance().FilterColliders(playerGCollider.GetCollisionMask(), boxGCollider.GetCollisionCat()) ||
				!CollisionManager::instance().FilterColliders(boxGCollider.GetCollisionMask(), playerGCollider.GetCollisionCat()))
				continue;

			TransformComponent& boxTrans = g_engine.m_coordinator.GetComponent<TransformComponent>(entity);

			//DebugDrawBall(boxCollider.m_aabb, boxTrans);
			//DebugDrawArrow(teleportLine);

			if (CollisionManager::instance().DiscreteLineVsAABB(teleportLine, boxCollider.m_aabb))
			{
				Vec2 boxPos = CollisionManager::instance().GetColliderPosition(boxCollider.m_aabb, boxTrans);

				std::array<LineSegment, 4> edges = CollisionManager::instance().GenerateEdges(boxCollider.m_aabb);

				float smallestT = 1.0f;
				float t = smallestT;

				for (LineSegment edge : edges)
				{
					t = Vec2DotProduct(Vec2(edge.m_pt0 - playerTransform.GetPosition()), edge.m_normal) /
						Vec2DotProduct(teleportVec, edge.m_normal);

					if (t > 0.0f && t < smallestT)
						smallestT = t;
				}

				calculatedPos = playerTransform.GetPosition() + smallestT * teleportVec;

				teleportLine = LineSegment(playerTransform.GetPosition(), calculatedPos);
				teleportVec = calculatedPos - playerTransform.GetPosition();
			}
		}
		return calculatedPos;
	}

	void PlayerControllerSystem::ToggleMode()
	{
		PLAYER_STATUS.ToggleLightStatus();
		for (Entity player : m_entities)
		{
			if (g_engine.m_coordinator.ComponentExists<ColliderComponent>(player))
			{
				ColliderComponent& playerCollider = g_engine.m_coordinator.GetComponent<ColliderComponent>(player);
				int lightPos = -1;
				int darkPos = -1;

				lightPos = LayerManager::instance().GetLayerCategory("Light");
				darkPos = LayerManager::instance().GetLayerCategory("Dark");

				// True means in light mode.
				if (PlayerStatusManager::instance().GetLightStatus())
				{
					playerCollider.ChangeLayer(lightPos);
					playerCollider.SetMask(darkPos);
					playerCollider.SetMask(lightPos, false);
				}
				else
				{
					playerCollider.ChangeLayer(darkPos);
					playerCollider.SetMask(lightPos);
					playerCollider.SetMask(darkPos, false);
				}
			}
		}
	}

	void PlayerControllerSystem::SetPlayerParent(Entity newParent)
	{
		if (!m_entities.size())
			return;
		ParentSetEvent* parent = new ParentSetEvent(newParent, *m_entities.begin());
		parent->SetSystemReceivers((int)SystemID::id_PARENTCHILDSYSTEM);
		EventDispatcher::instance().AddEvent(parent);
		PLAYER_STATUS.SetHitchhikeEntity(newParent);
	}

	void PlayerControllerSystem::ResetPlayerParent()
	{
		if (PLAYER_STATUS.GetHitchhikedEntity() != MAX_ENTITIES)
		{
			ParentResetEvent* parentReset = new ParentResetEvent(*m_entities.begin());
			parentReset->SetSystemReceivers((int)SystemID::id_PARENTCHILDSYSTEM);
			EventDispatcher::instance().AddEvent(parentReset);
			PLAYER_STATUS.SetHitchhikeEntity(MAX_ENTITIES);
		}
	}

	void PlayerControllerSystem::MovingPlayer()
	{
		if (PLAYER_STATUS.GetHitchhikedEntity() != MAX_ENTITIES)
		{
			ChildTransformEvent* setParentEv = new ChildTransformEvent(*m_entities.begin(), false);
			setParentEv->SetSystemReceivers((int)SystemID::id_PARENTCHILDSYSTEM);
			EventDispatcher::instance().AddEvent(setParentEv);
		}
	}

	void PlayerControllerSystem::DebugDrawBall(const BaseCollider& box, const TransformComponent& trans) const
	{
		std::ostringstream strstream;
		Entity ball = g_engine.m_coordinator.CreateEntity();
		strstream
			<< CollisionManager::instance().GetColliderPosition(box, trans).x << ";"
			<< CollisionManager::instance().GetColliderPosition(box, trans).y << ";"
			<< CollisionManager::instance().GetColliderScale(box, trans).x << ";"
			<< CollisionManager::instance().GetColliderScale(box, trans).y << ";"
			<< "0;"
			<< "2";

		TransformComponent& ballTransform = g_engine.m_coordinator.CreateComponent<TransformComponent>(ball);

		ballTransform.Deserialize(strstream.str());

		SpriteComponent& sprite = g_engine.m_coordinator.CreateComponent<SpriteComponent>(ball);
		sprite.Deserialize("Resources/Assets/Projectile.png;1;1;1;1;0.1");

		HierarchyInfo newInfo(ball, "Ball", "ball");
		g_engine.m_coordinator.GetActiveObjects().push_back(ball);
		g_engine.m_coordinator.GetHierarchyInfo(ball) = newInfo;

	}

	void PlayerControllerSystem::DebugDrawArrow(const LineSegment& teleportLine) const
	{
		Entity line = g_engine.m_coordinator.CreateEntity();
		TransformComponent& linetrans = g_engine.m_coordinator.CreateComponent<TransformComponent>(line);

		linetrans.setZ(2);
		linetrans.setPosition(teleportLine.m_pt0 + (teleportLine.m_pt1 - teleportLine.m_pt0) / 2);
		linetrans.setScale(Vec2(Vec2Length(teleportLine.m_pt1 - teleportLine.m_pt0), 50.0f));
		linetrans.setRotation(Vec2Rotation(teleportLine.m_pt1 - teleportLine.m_pt0));

		SpriteComponent& lsprite = g_engine.m_coordinator.CreateComponent<SpriteComponent>(line);
		lsprite.Deserialize("Resources/Assets/Arrow.png;1;1;1;1;1");

		HierarchyInfo nnewInfo(line, "Line", "line");
		g_engine.m_coordinator.GetActiveObjects().push_back(line);
		g_engine.m_coordinator.GetHierarchyInfo(line) = nnewInfo;
	}


}