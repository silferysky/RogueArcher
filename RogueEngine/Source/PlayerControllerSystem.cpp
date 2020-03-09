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
		//In case no player character leexdee
		if (!m_entities.size())
			return;

		//For PlayerControllerSystem Timer
		m_ignoreFrameEvent = false;

		if (PLAYER_STATUS.GetPlayerEntity() == MAX_ENTITIES && m_entities.size())
		{
			PLAYER_STATUS.SetPlayerEntity(*m_entities.begin());
		}

		//std::cout << "Freeze Timer: " << PLAYER_STATUS.GetFreezeControlTimer() << std::endl;
		if (PLAYER_STATUS.GetFreezeControlTimer() > 0.0f)
		{
			PLAYER_STATUS.SetFreezeControlTimer(PLAYER_STATUS.GetFreezeControlTimer() - g_deltaTime * g_engine.GetTimeScale());
			if (PLAYER_STATUS.GetFreezeControlTimer() <= 0.0f)
			{
				UnfreezeControlComponentUpdates();
			}
		}

		//PlayerControllerComponent& player = g_engine.m_coordinator.GetComponent<PlayerControllerComponent>(PLAYER_STATUS.GetPlayerEntity());

		//if (player.m_grounded)
		//{
		//	RE_INFO("Grounded!");
		//	player;
		//}
		//else
		//{
		//	RE_INFO("Not grounded!");
		//	player;
		//}

		if (!g_engine.m_coordinator.GameIsActive())
		{
			if (m_timedEntities.size())
				ClearTimedEntities();
			if (m_teleports.size())
				ClearTeleportEntities();
			return;
		}

		if (PLAYER_STATUS.ShowIndicator())
		{
			if (PLAYER_STATUS.GetIndicator() == MAX_ENTITIES && PLAYER_STATUS.GetPlayerEntity() != MAX_ENTITIES)
			{
				PLAYER_STATUS.SetIndicator(g_engine.m_coordinator.CloneArchetypes("Indicator", true));
				PLAYER_STATUS.SetHitchhikeIndicator(g_engine.m_coordinator.CloneArchetypes("IndicatorHitchhike", true));

				if (m_entities.size())
				{
					ParentSetEvent parent(*m_entities.begin(), PLAYER_STATUS.GetIndicator());
					parent.SetSystemReceivers((int)SystemID::id_PARENTCHILDSYSTEM);
					EventDispatcher::instance().AddEvent(parent);

					//ParentSetEvent hitchhikeParent(*m_entities.begin(), PLAYER_STATUS.GetHitchhikeIndicator());
					//hitchhikeParent.SetSystemReceivers((int)SystemID::id_PARENTCHILDSYSTEM);
					//EventDispatcher::instance().AddEvent(hitchhikeParent);
				}

				//Resetting some values
				PLAYER_STATUS.SetInfiniteJumps(false);
			}
			else
			{
				//If it reaches here, that means indicator exists already
				if (PLAYER_STATUS.GetIndicator() != MAX_ENTITIES &&
					g_engine.m_coordinator.ComponentExists<ChildComponent>(PLAYER_STATUS.GetIndicator()) &&
					g_engine.m_coordinator.ComponentExists<TransformComponent>(PLAYER_STATUS.GetIndicator()))
				{
					Vec2 calculatedPos = GetTeleportRaycast();
					TransformComponent& transform = g_engine.m_coordinator.GetComponent<TransformComponent>(PLAYER_STATUS.GetIndicator());
					Vec2 vecOfChange = Vec2(g_engine.m_coordinator.GetComponent<TransformComponent>(*m_entities.begin()).GetPosition() - calculatedPos);

					//For Position
					if (Vec2Length(vecOfChange) > 200.0f)
					{
						transform.setPosition(calculatedPos + vecOfChange / 2);
					}
					else
					{
						Vec2 length;
						Vec2Normalize(length, vecOfChange);
						length *= 200;
						transform.setPosition(calculatedPos + length / 2);
					}

					int dir = 1;
					if (vecOfChange.x < 0)
						dir *= -1;
					if (transform.GetScale().x > 0)
						dir *= -1;

					transform.setScale(Vec2(dir * transform.GetScale().x, transform.GetScale().y));

					//For Scale
					//if (/*vecOfChange.x < 200.0f &&*/ vecOfChange.x > 0.0f)
					//{
					//	transform.setScale(Vec2(-1 * transform.GetScale().x, transform.GetScale().y));
					//}
					//else// if (/*vecOfChange.x > -200.0f && */vecOfChange.x < 0.0f)
					//{
					//	//transform.setScale(Vec2(transform.GetScale().x, transform.GetScale().y));
					//}

					//For Rotation
					transform.setRotation(atan(vecOfChange.y / vecOfChange.x));
					//if (vecOfChange.x < 0.0f)
					//{
					//	transform.setScale(transform.GetScale() * -1);
					//}
				}

				//For Hitchhiking Indicator
				if (PLAYER_STATUS.GetHitchhikeIndicator() != MAX_ENTITIES &&
					g_engine.m_coordinator.ComponentExists<TransformComponent>(PLAYER_STATUS.GetHitchhikeIndicator()))
				{
					Entity toDrawAtEntity = GetEntityRaycasted();
					//std::cout << "Entity is " << toDrawAtEntity << std::endl;
				
					if (toDrawAtEntity != MAX_ENTITIES)
					{
						if (auto entTrans = g_engine.m_coordinator.TryGetComponent<TransformComponent>(toDrawAtEntity))
						{
							HierarchyInfo& entInfo = g_engine.m_coordinator.GetHierarchyInfo(toDrawAtEntity);
							TransformComponent& hitchhikeeTrans = entTrans->get();

							if (entInfo.m_tag == "Hitchhike" || entInfo.m_tag == "hitchhike")
							{
								TransformComponent& indicatorTrans = g_engine.m_coordinator.GetComponent<TransformComponent>(PLAYER_STATUS.GetHitchhikeIndicator());
								
								indicatorTrans.setPosition(hitchhikeeTrans.GetPosition());
								indicatorTrans.setScale(Vec2(75.0f, 75.0f));
								indicatorTrans.setZ(hitchhikeeTrans.GetZ());
								PLAYER_STATUS.SetHitchhikableEntity(toDrawAtEntity); // Save the hitchhikee's entity
							}
						}
					}
					else
					{
						//std::cout << "Entity is " << toDrawAtEntity << std::endl;
						//std::cout << "Entity Transform " << g_engine.m_coordinator.GetComponent<TransformComponent>(PLAYER_STATUS.GetHitchhikedEntity()).GetPosition().x << "," << g_engine.m_coordinator.GetComponent<TransformComponent>(PLAYER_STATUS.GetHitchhikedEntity()).GetPosition().y << std::endl;
						g_engine.m_coordinator.GetComponent<TransformComponent>(PLAYER_STATUS.GetHitchhikeIndicator()).setPosition(Vec2(10000.0f, 10000.0f));
						PLAYER_STATUS.SetHitchhikableEntity(MAX_ENTITIES);
						//std::cout << "Entity Transform " << entTrans->get().GetPosition().x << "," << entTrans->get().GetPosition().y << std::endl;
					}
				}
			}
		}

		//std::cout << PLAYER_STATUS.GetHitchhikableEntity() << std::endl;

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
				
			if (player.m_grounded)
				PLAYER_STATUS.SetTeleportCharge(3.0f);

			player.m_jumpTimer -= g_deltaTime * g_engine.GetTimeScale();
			//player.m_grounded = false;
		}
	}

	void PlayerControllerSystem::Receive(Event& ev)
	{
		//Statement here to make sure all commands only apply if game is not running
		//if (!g_engine.m_coordinator.GameIsActive())
		//{
		//	g_engine.SetTimeScale(1.0f);
		//	return;
		//}

		if (m_entities.begin() == m_entities.end())
			return;

		switch (ev.GetEventType())
		{
		case EventType::EvResetGame:
		{
			ResetGameEvent& reset = dynamic_cast<ResetGameEvent&>(ev);

			PLAYER_STATUS.SetPlayerEntity(MAX_ENTITIES);
			PLAYER_STATUS.SetHitchhikeEntity(MAX_ENTITIES);

			//Deleting teleport entities
			ClearTeleportEntities();

			//Deleting Indicator entity
			if (PLAYER_STATUS.GetIndicator() != MAX_ENTITIES)
			{
				g_engine.m_coordinator.AddToDeleteQueue(PLAYER_STATUS.GetIndicator());
				PLAYER_STATUS.SetIndicator(MAX_ENTITIES);
			}
			//Deleting IndicatorHitchhike entity
			if (PLAYER_STATUS.GetHitchhikeIndicator() != MAX_ENTITIES)
			{
				g_engine.m_coordinator.AddToDeleteQueue(PLAYER_STATUS.GetHitchhikeIndicator());
				PLAYER_STATUS.SetHitchhikeIndicator(MAX_ENTITIES);
			}

			PLAYER_STATUS.SetIndicatorStatus();

			////Safety check to make sure level exists
			//if (!PLAYER_STATUS.GetRunCount())
			//{
			//	//PLAYER_STATUS.SetRunCount(1);
			//	PLAYER_STATUS.Reset();
			//	return;
			//}

			//Deleting entity
			//for (auto entity : m_entities)
			//	g_engine.m_coordinator.AddToDeleteQueue(entity);

			//If more than 1 player entity
			//if (m_entities.size() > 1)
			//	g_engine.m_coordinator.AddToDeleteQueue(*m_entities.begin());

			//PLAYER_STATUS.Reset();
			break;
		}

		case EventType::EvFreezeControls:
		{
			//No need to set timer, since is being called by PlayerStatusManager
			FreezeControlComponentUpdates();
			break;
		}
		
		case EventType::EvUnfreezeControls:
		{
			UnfreezeControlComponentUpdates();
			break;
		}

		case EventType::EvEntityHitchhike:
		{
			if (PLAYER_STATUS.GetPlayerEntity() == MAX_ENTITIES)
				return;

			EntHitchhikeEvent& event = dynamic_cast<EntHitchhikeEvent&>(ev);

			Hitchhike(event.GetEntityID());
			break;
		}

		case EventType::EvEntityChangeRGBA:
		{
			EntChangeRGBAEvent& event = dynamic_cast<EntChangeRGBAEvent&>(ev);
			if (g_engine.m_coordinator.ComponentExists<SpriteComponent>(event.GetEntityID()))
			{
				auto& sprite = g_engine.m_coordinator.GetComponent<SpriteComponent>(event.GetEntityID());
				sprite.setFilter(glm::vec4(event.R(), event.G(), event.B(), event.A()));
			}

			return;
		}

		case EventType::EvEntityChangeSprite:
		{
			EntChangeSpriteEvent& event = dynamic_cast<EntChangeSpriteEvent&>(ev);
			if (g_engine.m_coordinator.ComponentExists<SpriteComponent>(event.GetEntityID()))
			{
				SpriteComponent& sprite = g_engine.m_coordinator.GetComponent<SpriteComponent>(event.GetEntityID());
				//sprite.setTexture(event.GetFile().c_str());
				sprite.setTexturePath(event.GetPath().c_str());
			}

			return;
		}

		case EventType::EvKeyTriggered:
		{
			KeyTriggeredEvent& keytriggeredevent = dynamic_cast<KeyTriggeredEvent&>(ev);
			KeyPress keycode = keytriggeredevent.GetKeyCode();

			if (g_engine.GetIsFocused())
			{
				if (keycode == KeyPress::KeyF5)
					g_engine.m_coordinator.ToggleEditorIsRunning();

				else if (keycode == KeyPress::KeyF6)
					g_engine.ToggleVSync();

				//else if (PLAYER_STATUS.GetFreezeControlTimer() > 0.0f)
				//{
				//	return;
				//}
				
				else if (keycode == KeyPress::KeyZ)
				{
					PLAYER_STATUS.SetFreezeControlTimer(10.0f);
					return;
				}

				else if (keycode == KeyPress::Key0)
				{
					PLAYER_STATUS.SetInfiniteJumps(!PLAYER_STATUS.GetInfiniteJumps());
				}

				else if (keycode == KeyPress::MB1 && g_engine.GetIsFocused())
				{
					//For slow mo
					for (Entity entity : m_entities)
					{
						auto& PlayerControllable = g_engine.m_coordinator.GetComponent<PlayerControllerComponent>(entity);
						if (!PlayerControllable.m_grounded /*&& !PLAYER_STATUS.ShowIndicator()*/)
						{
							g_engine.SetTimeScale(PlayerControllable.GetSlowTime());
						}
						//PLAYER_STATUS.SetIndicatorStatus();
					}

					if (PLAYER_STATUS.GetIndicator() != MAX_ENTITIES && g_engine.m_coordinator.ComponentExists<ChildComponent>(PLAYER_STATUS.GetIndicator()))
					{
						ChildComponent& comp = g_engine.m_coordinator.GetComponent<ChildComponent>(PLAYER_STATUS.GetIndicator());
						//SpriteComponent& sprite = g_engine.m_coordinator.GetComponent<SpriteComponent>(PLAYER_STATUS.GetIndicator());
						comp.SetIsFollowing(true);
						//auto filter = sprite.getFilter();
						//sprite.setFilter(glm::vec4(filter.r, filter.g, filter.b, 1));
					}
				}

				else if (keycode == KeyPress::MB2 || keycode == KeyPress::KeyShift || keycode == KeyPress::KeyE)
				{
					ToggleMode();
				}

				else if (keycode == KeyPress::MB3)
				{
					ClearTimedEntities();
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

						// Reset boolean for grounded
						if (!PLAYER_STATUS.HasJumped())
						{
							AudioManager::instance().loadSound("Resources/Sounds/jump.ogg", 0.3f, false).Play(1.0f);
							Hitchhike(MAX_ENTITIES);
							player.m_grounded = false;
							PLAYER_STATUS.SetHasJumped(true);
							player.m_jumpTimer = PLAYER_STATUS.GetJumpMaxTimer();
						}

						ResetPlayerParent();
					}
				}

				else if (keycode == KeyPress::KeyEsc)
				{
					g_engine.m_coordinator.SetPauseState(true);
					g_engine.m_coordinator.GetSystem<MenuControllerSystem>()->ToggleUIMenuObjs();
					PLAYER_STATUS.SetIndicatorStatus(PLAYER_STATUS.ShowIndicator());
					//If Indicator is no longer hidden
					if (!PLAYER_STATUS.ShowIndicator())
					{
						g_engine.m_coordinator.AddToDeleteQueue(PLAYER_STATUS.GetIndicator());
						g_engine.m_coordinator.AddToDeleteQueue(PLAYER_STATUS.GetHitchhikedEntity());
					}
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
			if (PLAYER_STATUS.GetFreezeControlTimer() > 0.0f)
				return;

			KeyPressEvent& EvPressKey = dynamic_cast<KeyPressEvent&>(ev);
			KeyPress keycode = EvPressKey.GetKeyCode();
			for (std::set<Entity>::iterator iEntity = m_entities.begin(); iEntity != m_entities.end(); ++iEntity)
			{
				//For 1st entity
				if (iEntity == m_entities.begin())
				{
					if (g_engine.GetIsFocused())
					{
						if (keycode == KeyPress::KeyA && PLAYER_STATUS.IsPlayerActive() && PLAYER_STATUS.GetHitchhikedEntity() == MAX_ENTITIES)
						{
							auto& ctrler = g_engine.m_coordinator.GetComponent<PlayerControllerComponent>(*iEntity);
							ctrler.SetMoveState(MoveState::e_left);

							EntMoveEvent ev{ *iEntity, true, -1.0f, 0.0f };
							ev.SetSystemReceivers((int)SystemID::id_PHYSICSSYSTEM);
							ev.SetSystemReceivers((int)SystemID::id_GRAPHICSSYSTEM);
							EventDispatcher::instance().AddEvent(ev);
							PLAYER_STATUS.SetMoveLeft(true);
							MovingPlayer();
						}
						else if (keycode == KeyPress::KeyD && PLAYER_STATUS.IsPlayerActive() && PLAYER_STATUS.GetHitchhikedEntity() == MAX_ENTITIES)
						{
							auto& ctrler = g_engine.m_coordinator.GetComponent<PlayerControllerComponent>(*iEntity);
							ctrler.SetMoveState(MoveState::e_right);

							EntMoveEvent ev{ *iEntity, true, 1.0f, 0.0f };
							ev.SetSystemReceivers((int)SystemID::id_PHYSICSSYSTEM);
							ev.SetSystemReceivers((int)SystemID::id_GRAPHICSSYSTEM);
							EventDispatcher::instance().AddEvent(ev);
							PLAYER_STATUS.SetMoveLeft(false);
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

									trans.setPosition(Vec2(2300.0f, 350.0f));
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

									trans.setPosition(Vec2(-2813.04f, 1103.46f));
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
			KeyReleaseEvent& KeyReleaseEv = dynamic_cast<KeyReleaseEvent&>(ev);
			KeyPress keycode = KeyReleaseEv.GetKeyCode();

			if (!g_engine.m_coordinator.GameIsActive())
				return;

			if (PLAYER_STATUS.GetFreezeControlTimer() > 0.0f)
				return;

			if (g_engine.GetIsFocused())
			{
				if (keycode == KeyPress::MB1)
				{
					if (PLAYER_STATUS.GetHitchhikableEntity() != MAX_ENTITIES && PLAYER_STATUS.GetHitchhikedEntity() == MAX_ENTITIES/*&& g_engine.m_coordinator.GetHierarchyInfo(pickedEntity).m_tag == "Hitchhike"*/)
						Hitchhike(PLAYER_STATUS.GetHitchhikableEntity());
					else if (!m_timedEntities.size() && PLAYER_STATUS.GetInLightDur() < 0.0f && 
						PLAYER_STATUS.GetTeleportDelay() < 0.0f && 
						(PLAYER_STATUS.GetTeleportCharge() >= 1.0f || PLAYER_STATUS.GetInfiniteJumps()))
					{
						//CreateBallAttack();
						//auto& player = g_engine.m_coordinator.GetComponent<PlayerControllerComponent>(*m_entities.begin());
						//if (player.m_grounded)
						//	PLAYER_STATUS.SetTeleportCharge(PLAYER_STATUS.GetMaxTeleportCharge());
						if (PLAYER_STATUS.IsPlayerActive())
						{
							Hitchhike(MAX_ENTITIES);
							Teleport();
						}
						m_ignoreFrameEvent = true;
					}
					//if (PLAYER_STATUS.ShowIndicator())
					g_engine.SetTimeScale(1.0f);
					//PLAYER_STATUS.SetIndicatorStatus(false);

					//To reduce calculations
					if (PLAYER_STATUS.GetIndicator() != MAX_ENTITIES && g_engine.m_coordinator.ComponentExists<ChildComponent>(PLAYER_STATUS.GetIndicator()))
					{
						ChildComponent& comp = g_engine.m_coordinator.GetComponent<ChildComponent>(PLAYER_STATUS.GetIndicator());
						//SpriteComponent& sprite = g_engine.m_coordinator.GetComponent<SpriteComponent>(PLAYER_STATUS.GetIndicator());
						comp.SetIsFollowing(false);
						//auto filter = sprite.getFilter();
						//sprite.setFilter(glm::vec4(filter.r, filter.g, filter.b, 0));
					}
				}
				if ((keycode == KeyPress::KeyA ) || (keycode == KeyPress::KeyD))
				{
					auto& ctrler = g_engine.m_coordinator.GetComponent<PlayerControllerComponent>(*m_entities.begin());

					ctrler.SetMoveState(MoveState::e_stop);
				}
			}
			
			return;
		}
		case EventType::EvOnCollisionExit:
		{
			auto& collisionExit = dynamic_cast<AABBCollisionExitEvent&>(ev);
			return;
		}

		case EventType::EvOnCollisionStay:
		{
			auto& collisionStay = dynamic_cast<AABBCollisionStayEvent&>(ev);

			if (m_ignoreFrameEvent)
			{
				m_ignoreFrameEvent = !m_ignoreFrameEvent;
				return;
			}

			Entity player;
			Entity ground;
			Vec2 playerTrans, groundTrans;
			Vec2 playerScale, groundScale; 

			const BoxCollider2DComponent* groundCollider = nullptr;

			HierarchyInfo infoA = g_engine.m_coordinator.GetHierarchyInfo(collisionStay.GetThis().m_entity);
			HierarchyInfo infoB = g_engine.m_coordinator.GetHierarchyInfo(collisionStay.GetOther().m_entity);
			
			if (infoA.m_tag == "Player")
			{
				if (infoB.m_tag == "Ground" || infoB.m_tag == "Platform")
				{
					player = collisionStay.GetThis().m_entity;
					playerTrans = CollisionManager::instance().GetColliderPosition(collisionStay.GetThis().m_collider.m_aabb, collisionStay.GetThis().m_transform);
					playerScale = CollisionManager::instance().GetColliderScale(collisionStay.GetThis().m_collider.m_aabb, collisionStay.GetThis().m_transform);

					ground = collisionStay.GetOther().m_entity;
					groundTrans = CollisionManager::instance().GetColliderPosition(collisionStay.GetOther().m_collider.m_aabb, collisionStay.GetOther().m_transform);
					groundScale = CollisionManager::instance().GetColliderScale(collisionStay.GetOther().m_collider.m_aabb, collisionStay.GetOther().m_transform); 
					groundCollider = &collisionStay.GetOther().m_collider;
				}
				else
					return;
			}
			else if (infoB.m_tag == "Player")
			{
				if (infoA.m_tag == "Ground" || infoB.m_tag == "Platform")
				{
					ground = collisionStay.GetThis().m_entity;
					groundTrans = CollisionManager::instance().GetColliderPosition(collisionStay.GetThis().m_collider.m_aabb, collisionStay.GetThis().m_transform);
					groundScale = CollisionManager::instance().GetColliderScale(collisionStay.GetThis().m_collider.m_aabb, collisionStay.GetThis().m_transform);
					groundCollider = &collisionStay.GetThis().m_collider;

					player = collisionStay.GetOther().m_entity;
					playerTrans = CollisionManager::instance().GetColliderPosition(collisionStay.GetOther().m_collider.m_aabb, collisionStay.GetOther().m_transform);
					playerScale = CollisionManager::instance().GetColliderScale(collisionStay.GetOther().m_collider.m_aabb, collisionStay.GetOther().m_transform);
					
				}
				else
					return;
			}
			else
				return;

			for (Entity entity : m_entities)
			{
				auto& player = g_engine.m_coordinator.GetComponent<PlayerControllerComponent>(entity);

				const float buffer = 10.0f;

				LineSegment finiteRay(playerTrans, Vec2(playerTrans.x, playerTrans.y - playerScale.y / 2.0f - buffer));

				LineSegment colliderEdge(Vec2(groundCollider->m_aabb.getMin().x, groundCollider->m_aabb.getMax().y), groundCollider->m_aabb.getMax());

				if (CollisionManager::instance().DiscreteLineVsLine(finiteRay, colliderEdge))
				{
					player.m_grounded = true;
					PLAYER_STATUS.SetHasJumped(false);	
				}
				//else if (infoA.m_tag == "Platform" || infoB.m_tag == "Platform")
				//{
				//	player.m_grounded = true;
				//	PLAYER_STATUS.SetHasJumped(false);
				//	if (infoA.m_tag == "Platform")
				//	{
				//		//ParentSetEvent& parent = new ParentSetEvent(infoA.m_Entity, entity);
				//		//parent->SetSystemReceivers((int)SystemID::id_PARENTCHILDSYSTEM);
				//		//EventDispatcher::instance().AddEvent(parent);
				//		//PLAYER_STATUS.SetHitchhikeEntity(infoA.m_Entity);
				//		//m_ignoreFrameEvent = true;
				//	}
				//	else //if (infoB.m_tag == "Platform")
				//	{
				//		//ParentSetEvent& parent = new ParentSetEvent(infoB.m_Entity, entity);
				//		//parent->SetSystemReceivers((int)SystemID::id_PARENTCHILDSYSTEM);
				//		//EventDispatcher::instance().AddEvent(parent);
				//		//PLAYER_STATUS.SetHitchhikeEntity(infoB.m_Entity);
				//		//m_ignoreFrameEvent = true;
				//	}
				//}
			}
			return;

		} // switch (ev.GetEventType())
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

	void PlayerControllerSystem::ClearTeleportEntities(Entity ent)
	{
		for (auto it = m_teleports.begin(); it != m_teleports.end(); ++it)
		{
			if (it->m_entity == ent)
			{
				g_engine.m_coordinator.AddToDeleteQueue(it->m_entity);
				m_teleports.erase(it);
				return;
			}
		}
	}

	void PlayerControllerSystem::CreateTeleportEvent(Vec2 newPosition)
	{
		EntTeleportEvent event(*m_entities.begin(), newPosition);
		event.SetSystemReceivers((int)SystemID::id_PHYSICSSYSTEM);
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
		g_engine.m_coordinator.GetComponent<PlayerControllerComponent>(*m_entities.begin()).m_grounded = false;

		Vec2 calculatedPos = GetTeleportRaycast();

		CreateTeleportEvent(calculatedPos);
		ResetPlayerParent();

		//ParentTransformEvent& parentTransform = new ParentTransformEvent(*m_entities.begin(), true);
		//parentTransform->SetSystemReceivers((int)SystemID::id_PARENTCHILDSYSTEM);
		//EventDispatcher::instance().AddEvent(parentTransform);

		//For teleport VFX
		TimedEntity ent(g_engine.m_coordinator.CloneArchetypes("TeleportSprite", false), 0.5f);
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

		if (PLAYER_STATUS.GetTeleportCharge() == 3)
			AudioManager::instance().loadSound("Resources/Sounds/Teleport3.ogg", 0.3f, false).Play();
		else if (PLAYER_STATUS.GetTeleportCharge() == 2)
			AudioManager::instance().loadSound("Resources/Sounds/Teleport2.ogg", 0.3f, false).Play();
		else
			AudioManager::instance().loadSound("Resources/Sounds/teleport1.ogg", 0.3f, false).Play();		
		//AudioManager::instance().loadSound("Resources/Sounds/[Shoot Projectile]SCI-FI-WHOOSH_GEN-HDF-20864.ogg", 0.86f, false).Play();
		//AudioManager::instance().loadSound("Resources/Sounds/[Ela Appear]SCI-FI-WHOOSH_GEN-HDF-20870.ogg", 0.3f, false).Play();

		//AudioManager::instance().loadSound("Resources/Sounds/[Shoot Projectile]SCI-FI-WHOOSH_GEN-HDF-20864.ogg", 0.86f, false).Play();
		//AudioManager::instance().loadSound("Resources/Sounds/[Ela Appear]SCI-FI-WHOOSH_GEN-HDF-20870.ogg", 0.3f, false).Play();
	}

	void PlayerControllerSystem::Hitchhike(Entity ent)
	{
		//std::cout << "Ent: " << ent << std::endl;
		if (ent != MAX_ENTITIES && g_engine.m_coordinator.GetHierarchyInfo(ent).m_tag == "Hitchhike")
		{
			//std::cout << "Object is " << g_engine.m_coordinator.GetHierarchyInfo(ent).m_objectName << std::endl;

			if (auto trans = g_engine.m_coordinator.TryGetComponent<TransformComponent>(PLAYER_STATUS.GetPlayerEntity()))
			{
				Vec2 initialPos = trans->get().GetPosition();
				Vec2 endPos = PickingManager::instance().GetWorldCursor();
				//std::cout << "Actual Distance" << Vec2SqDistance(initialPos, endPos) << std::endl;
				//std::cout << "Calc Distance" << trans->get().GetScale().x * trans->get().GetScale().x * 9 << std::endl;

				//If distance to hitchhike is > Hitchhike range * Slight bonus to "extend" range
				if (Vec2SqDistance(initialPos, endPos) > (trans->get().GetScale().x * 3)* (trans->get().GetScale().x * 3))
				{
					//std::cout << "Too Far" << std::endl;
					return;
				}
			}
			else //Cannot check transform properly, so this doesn't work
			{
				//std::cout << "No Transform" << std::endl;
				return;
			}

			SetPlayerParent(ent);
			PLAYER_STATUS.SetTeleportCharge(3.0f);
			if (g_engine.m_coordinator.ComponentExists<ChildComponent>(PLAYER_STATUS.GetPlayerEntity()))
			{
				auto& player = g_engine.m_coordinator.GetComponent<ChildComponent>(PLAYER_STATUS.GetPlayerEntity());
				player.SetLocalDirty();
				//player.SetGlobalDirty();
			}
			if (g_engine.m_coordinator.ComponentExists<BoxCollider2DComponent>(PLAYER_STATUS.GetPlayerEntity()))
			{
				auto& player = g_engine.m_coordinator.GetComponent<BoxCollider2DComponent>(PLAYER_STATUS.GetPlayerEntity());
				player.SetCollisionMode(CollisionMode::e_trigger);
				//player.SetGlobalDirty();
			}
		}
		else
		{
			ResetPlayerParent();
			if (g_engine.m_coordinator.ComponentExists<BoxCollider2DComponent>(PLAYER_STATUS.GetPlayerEntity()))
			{
				auto& player = g_engine.m_coordinator.GetComponent<BoxCollider2DComponent>(PLAYER_STATUS.GetPlayerEntity());
				player.SetCollisionMode(CollisionMode::e_awake);
				//player.SetGlobalDirty();
			}
		}

		PLAYER_STATUS.ChangePlayerSprite();
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

	// SOLELY tailored for hitchhiking.
	Entity PlayerControllerSystem::GetEntityRaycasted()
	{
		TransformComponent& playerTransform = g_engine.m_coordinator.GetComponent<TransformComponent>(*m_entities.begin());
		Vec2 initialPos = playerTransform.GetPosition();
		Vec2 cursor = PickingManager::instance().GetWorldCursor();
		Vec2 endPos = cursor;
		Vec2 calculatedPos = initialPos;
		Entity calculatedEntity = MAX_ENTITIES;

		//Calculating max cursor distance value
		endPos -= initialPos;
		Vec2Normalize(endPos, endPos);
		endPos *= playerTransform.GetScale().x * 3.0f;
		endPos += initialPos;
		
		// Take the smaller of the cursor and max range.
		if (Vec2SqDistance(cursor, initialPos) < Vec2SqDistance(endPos, initialPos))
			endPos = cursor;

		for (size_t checkCount = 0; checkCount < 3; ++checkCount)
		{
			calculatedPos += ((endPos - initialPos) / 3);
		}

		const std::set<Entity>& boxEntities = g_engine.m_coordinator.GetSystem<BoxCollisionSystem>()->GetEntitySet();
		BoxCollider2DComponent& playerCollider = g_engine.m_coordinator.GetComponent<BoxCollider2DComponent>(*m_entities.begin());
		ColliderComponent& playerGCollider = g_engine.m_coordinator.GetComponent<ColliderComponent>(*m_entities.begin());
		LineSegment teleportLine = LineSegment(playerTransform.GetPosition(), calculatedPos);
		Vec2 teleportVec = calculatedPos - playerTransform.GetPosition();

		for (Entity entity : boxEntities)
		{
			// Skip itself
			if (entity == *m_entities.begin())
				continue;

			BoxCollider2DComponent& boxCollider = g_engine.m_coordinator.GetComponent<BoxCollider2DComponent>(entity);
			ColliderComponent& boxGCollider = g_engine.m_coordinator.GetComponent<ColliderComponent>(entity);

			// Skip asleep colliders
			if (boxCollider.GetCollisionMode() == CollisionMode::e_asleep)
				continue;

			// Filter layers
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

					// Only allow hitchhike triggers and colliders (Avoid regular triggers)
					if (t > 0.0f && t < smallestT)
					{
						// Kena collider, no hitchhike available yet
						if (boxCollider.GetCollisionMode() == CollisionMode::e_awake)
						{
							smallestT = t;
							calculatedEntity = MAX_ENTITIES;
						}
						// Kena hitchhike trigger, hitchhike available.
						if(g_engine.m_coordinator.GetHierarchyInfo(entity).m_tag == "Hitchhike")
						{
							smallestT = t;
							calculatedEntity = entity;
						}
					}
				}

				calculatedPos = playerTransform.GetPosition() + smallestT * teleportVec;

				teleportLine = LineSegment(playerTransform.GetPosition(), calculatedPos);
				teleportVec = calculatedPos - playerTransform.GetPosition();
			}
		}
		return calculatedEntity;
	}

	void PlayerControllerSystem::ToggleMode()
	{
		PLAYER_STATUS.ToggleLightStatus();

		if (PLAYER_STATUS.GetLightStatus())
			AudioManager::instance().loadSound("Resources/Sounds/LightChange.ogg", 0.3f, false).Play();
		else
			AudioManager::instance().loadSound("Resources/Sounds/DarkChange.ogg", 0.3f, false).Play();

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

	void PlayerControllerSystem::FreezeControlComponentUpdates()
	{
		if (auto boxCollider = g_engine.m_coordinator.TryGetComponent<BoxCollider2DComponent>(PLAYER_STATUS.GetPlayerEntity()))
		{
			boxCollider->get().SetCollisionMode(CollisionMode::e_asleep);
		}
		if (auto rigidbody = g_engine.m_coordinator.TryGetComponent<RigidbodyComponent>(PLAYER_STATUS.GetPlayerEntity()))
		{
			rigidbody->get().setIsStatic(true);
		}
	}

	void PlayerControllerSystem::UnfreezeControlComponentUpdates()
	{
		if (auto boxCollider = g_engine.m_coordinator.TryGetComponent<BoxCollider2DComponent>(PLAYER_STATUS.GetPlayerEntity()))
		{
			boxCollider->get().SetCollisionMode(CollisionMode::e_awake);
		}
		if (auto rigidbody = g_engine.m_coordinator.TryGetComponent<RigidbodyComponent>(PLAYER_STATUS.GetPlayerEntity()))
		{
			rigidbody->get().setIsStatic(false);
		}
	}

	void PlayerControllerSystem::SetPlayerParent(Entity newParent)
	{
		if (!m_entities.size())
			return;

		if (g_engine.m_coordinator.ComponentExists<RigidbodyComponent>(PLAYER_STATUS.GetPlayerEntity()))
		{
			auto& player = g_engine.m_coordinator.GetComponent<RigidbodyComponent>(PLAYER_STATUS.GetPlayerEntity());
			player.setIsStatic(true);
		}

		ParentSetEvent parent(newParent, *m_entities.begin());
		parent.SetSystemReceivers((int)SystemID::id_PARENTCHILDSYSTEM);
		EventDispatcher::instance().AddEvent(parent);
		PLAYER_STATUS.SetHitchhikeEntity(newParent);
	}

	void PlayerControllerSystem::ResetPlayerParent()
	{
		if (PLAYER_STATUS.GetHitchhikedEntity() != MAX_ENTITIES)
		{
			if (g_engine.m_coordinator.ComponentExists<RigidbodyComponent>(PLAYER_STATUS.GetPlayerEntity()))
			{
				auto& player = g_engine.m_coordinator.GetComponent<RigidbodyComponent>(PLAYER_STATUS.GetPlayerEntity());
				player.setIsStatic(false);
			}

			ParentResetEvent parentReset(*m_entities.begin());
			parentReset.SetSystemReceivers((int)SystemID::id_PARENTCHILDSYSTEM);
			EventDispatcher::instance().AddEvent(parentReset);
			PLAYER_STATUS.SetHitchhikeEntity(MAX_ENTITIES);
		}
	}

	void PlayerControllerSystem::MovingPlayer()
	{
		if (PLAYER_STATUS.GetHitchhikedEntity() != MAX_ENTITIES)
		{
			ChildTransformEvent setParentEv(*m_entities.begin(), false);
			setParentEv.SetSystemReceivers((int)SystemID::id_PARENTCHILDSYSTEM);
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