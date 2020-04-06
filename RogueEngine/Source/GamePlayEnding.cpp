/* Start Header ************************************************************************/
/*!
\file           GamePlayEnding.cpp
\project        Exale
\author         Loh Kai Yi, kaiyi.loh, 390002918 (100%)
\par            kaiyi.loh\@digipen.edu
\date           3 April,2020
\brief          This file contains the functions definitions for GamePlayEnding

All content (C) 2020 DigiPen (SINGAPORE) Corporation, all rights
reserved.

Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header **************************************************************************/
#include "Precompiled.h"
#include "GamePlayEnding.h"
#include "PlayerStatusManager.h"
//#include "Main.h"
#include "GraphicsEvent.h"
#include "CameraManager.h"
#include "CameraSystem.h"
#include "InputManager.h"
#define propstransform 50.0f
#define filterchange 10.0f
#define backgroundfilterchange 0.6f
#define clonetransformx 100.0f
#define frame1 m_timer > 17.0f && m_timer < 18.0f
#define frame2 m_timer > 18.0f && m_timer < 19.0f
#define frame3 m_timer > 19.0f && m_timer < 20.0f
#define frame4 m_timer > 20.0f && m_timer < 21.0f
#define frame5 m_timer > 21.0f && m_timer < 22.0f
#define frame6 m_timer > 22.0f && m_timer < 23.0f
#define frame7 m_timer > 23.0f && m_timer < 24.0f
#define frame8 m_timer > 24.0f && m_timer < 25.0f
#define frame9 m_timer > 25.0f && m_timer < 26.0f
#define frame10 m_timer > 26.0f && m_timer < 27.0f
#define frame11 m_timer > 27.0f && m_timer < 28.0f
#define frame12 m_timer > 28.0f && m_timer < 29.0f
#define frame13 m_timer > 29.0f && m_timer < 30.0f
#define frame14 m_timer > 30.0f && m_timer < 31.0f
#define frame15 m_timer > 31.0f && m_timer < 31.5f
#define frame16 m_timer > 31.5f && m_timer < 32.0f
#define frame17 m_timer > 32.0f && m_timer < 32.5f

namespace Rogue
{
	GamePlayEnding::GamePlayEnding(Entity entity, LogicComponent& logicComponent, StatsComponent& statsComponent)
		: ScriptComponent(entity, logicComponent, statsComponent), m_souls{ 0 }, m_timer{ 0.0f }, m_activated{ false }
		, m_movement{ 15.0f }, m_moveleft{ false }, m_moveright{ false }, m_finalInput{ false }, m_endingAPressed{ false }, m_endingDPressed{ false },m_finalSpriteSet{false}
		, m_soundloaded{ false }, m_finalSprite{ MAX_ENTITIES }, m_finalSpriteBackground{ MAX_ENTITIES }
	{
		
	}

	void GamePlayEnding::AIActiveStateUpdate()
	{
		if (!g_engine.m_coordinator.GameIsActive())
			return;

		m_logicComponent->SetActiveStateBit(static_cast<size_t>(AIState::AIState_Idle));
	}

	void GamePlayEnding::AIIdleUpdate()
	{
		if (PlayerStatusManager::instance().GetEnding())
		{
			Sound sound;

			g_engine.m_coordinator.GetSystem<CameraSystem>()->setIsActive(true);

			m_timer += g_deltaTime * g_engine.GetTimeScale();

			//Zoom out slightly
			if (CameraManager::instance().GetCameraZoom() < 1.3f)
			{
				CameraManager::instance().SetCameraZoom(CameraManager::instance().GetCameraZoom() + 0.4f * g_deltaTime);
			}
			auto playerEnt = PlayerStatusManager::instance().GetPlayerEntity();
			auto& transform = g_engine.m_coordinator.GetComponent<TransformComponent>(playerEnt);
			if (m_moveright && (transform.GetPosition().x < 338.0f))
			{
				transform.offSetPosition(Vec2(130.0f * g_deltaTime, 0.0f));
			}

			if (m_moveleft && (transform.GetPosition().x > 338.0f))
			{
				transform.offSetPosition(Vec2(-130.0f * g_deltaTime, 0.0f));
			}

			if(m_timer > 2.5f && m_timer < 3.0f)
			{
				if (m_trueEnding)
				{
					sound = g_engine.m_coordinator.loadSound("Resources/Sounds/Exale_Ending.ogg");
					if (!m_soundloaded)
					{
						sound.Play();
						m_soundloaded = !m_soundloaded;
					}
				}
			}

			//8. <Fade in/out 3 secs, display statement on top of the camera>
			if (m_timer > 3.0f && m_timer < 6.0f)
			{
				for (HierarchyInfo& info : g_engine.m_coordinator.GetHierarchyInfoArray())
				{

					if (info.m_tag == "DoorUp")
					{
						auto& transform = g_engine.m_coordinator.GetComponent<TransformComponent>(info.m_Entity);
						transform.setPosition(Vec2{ transform.GetPosition().x,transform.GetPosition().y + propstransform * g_deltaTime });
					}
					if (info.m_tag == "DoorDown")
					{
						auto& transform = g_engine.m_coordinator.GetComponent<TransformComponent>(info.m_Entity);
						transform.setPosition(Vec2{ transform.GetPosition().x,transform.GetPosition().y - propstransform * g_deltaTime });
					}
					if (info.m_tag == "DoorLeft")
					{
						auto& transform = g_engine.m_coordinator.GetComponent<TransformComponent>(info.m_Entity);
						transform.setPosition(Vec2{ transform.GetPosition().x - propstransform * g_deltaTime,transform.GetPosition().y });
					}
					if (info.m_tag == "DoorRight")
					{
						auto& transform = g_engine.m_coordinator.GetComponent<TransformComponent>(info.m_Entity);
						transform.setPosition(Vec2{ transform.GetPosition().x + propstransform * g_deltaTime,transform.GetPosition().y });
					}
				}

				//FadeEvent ev = FadeEvent(MAX_ENTITIES, 0.5f, false);
				//ev.SetSystemReceivers(static_cast<int>(SystemID::id_GRAPHICSSYSTEM));
				//EventDispatcher::instance().AddEvent(ev);
			}
			//9. < Fade in 3 secs, display choice input statement on top of the camera >
			else if (m_timer > 6.0f && m_timer < 9.0f)
			{

				for (HierarchyInfo& info : g_engine.m_coordinator.GetHierarchyInfoArray())
				{
					if (info.m_tag == "ElaTitle")
					{
						auto& sprite = g_engine.m_coordinator.GetComponent<SpriteComponent>(info.m_Entity);
						sprite.setFilter(glm::vec4(sprite.getFilter().r, sprite.getFilter().g, sprite.getFilter().b, sprite.getFilter().a + filterchange * g_deltaTime));
					}
					if (info.m_tag == "ElaA")
					{
						auto& sprite = g_engine.m_coordinator.GetComponent<SpriteComponent>(info.m_Entity);
						auto& transform = g_engine.m_coordinator.GetComponent<TransformComponent>(info.m_Entity);
						sprite.setFilter(glm::vec4(sprite.getFilter().r, sprite.getFilter().g, sprite.getFilter().b, sprite.getFilter().a + filterchange * g_deltaTime));
						//transform.setZ(101);
					}
					if (info.m_tag == "ExaTitle")
					{
						auto& sprite = g_engine.m_coordinator.GetComponent<SpriteComponent>(info.m_Entity);
						sprite.setFilter(glm::vec4(sprite.getFilter().r, sprite.getFilter().g, sprite.getFilter().b, sprite.getFilter().a + filterchange * g_deltaTime));
					}
					//if (info.m_tag == "ExaA")
					//{
					//	auto& sprite = g_engine.m_coordinator.GetComponent<SpriteComponent>(info.m_Entity);
					//	auto& transform = g_engine.m_coordinator.GetComponent<TransformComponent>(info.m_Entity);
					//	sprite.setFilter(glm::vec4(sprite.getFilter().r, sprite.getFilter().g, sprite.getFilter().b, sprite.getFilter().a + filterchange * g_deltaTime));
					//	//transform.setZ(101);
					//}
				}
			}
			else if (m_timer > 9.0f && m_timer < 12.0f)
			{
				if (!m_trueEnding)
				{
					for (HierarchyInfo& info : g_engine.m_coordinator.GetHierarchyInfoArray())
					{
						if (info.m_tag == "Sacrifice")
						{
							auto& sprite = g_engine.m_coordinator.GetComponent<SpriteComponent>(info.m_Entity);
							sprite.setFilter(glm::vec4(sprite.getFilter().r, sprite.getFilter().g, sprite.getFilter().b, sprite.getFilter().a + filterchange * g_deltaTime));
						}

					}
				}
			}
			else if (m_timer > 12.0f && m_timer < 15.0f)
			{
				for (HierarchyInfo& info : g_engine.m_coordinator.GetHierarchyInfoArray())
				{
					if (info.m_tag == "Player")
					{
						auto& transform = g_engine.m_coordinator.GetComponent<TransformComponent>(info.m_Entity);
						//hide player
						transform.setZ(-100);
					}
					if (info.m_tag == "ElaTitle")
					{
						auto& transform = g_engine.m_coordinator.GetComponent<TransformComponent>(info.m_Entity);
						transform.setPosition(Vec2{ transform.GetPosition().x + propstransform * g_deltaTime,transform.GetPosition().y });
						if (!g_engine.m_coordinator.ComponentExists<UIComponent>(info.m_Entity))
						{
							g_engine.m_coordinator.AddComponent(info.m_Entity, UIComponent());
							auto& UI = g_engine.m_coordinator.GetComponent<UIComponent>(info.m_Entity);
							UI.setIsActive(true);
						}
					}
					//if (info.m_tag == "ElaA")
					//{
					//	auto& transform = g_engine.m_coordinator.GetComponent<TransformComponent>(info.m_Entity);
					//	transform.setPosition(Vec2{ transform.GetPosition().x + propstransform * g_deltaTime,transform.GetPosition().y });
					//	if (!g_engine.m_coordinator.ComponentExists<UIComponent>(info.m_Entity))
					//	{
					//		g_engine.m_coordinator.AddComponent(info.m_Entity, UIComponent());
					//		auto& UI = g_engine.m_coordinator.GetComponent<UIComponent>(info.m_Entity);
					//		UI.setIsActive(true);
					//	}
					//	transform.setZ(101);
					//}
					if (info.m_tag == "ExaTitle")
					{
						auto& transform = g_engine.m_coordinator.GetComponent<TransformComponent>(info.m_Entity);
						transform.setPosition(Vec2{ transform.GetPosition().x - propstransform * g_deltaTime,transform.GetPosition().y });
						if (!g_engine.m_coordinator.ComponentExists<UIComponent>(info.m_Entity))
						{
							g_engine.m_coordinator.AddComponent(info.m_Entity, UIComponent());
							auto& UI = g_engine.m_coordinator.GetComponent<UIComponent>(info.m_Entity);
							UI.setIsActive(true);
						}

					}
					//if (info.m_tag == "ExaA")
					//{
					//	auto& transform = g_engine.m_coordinator.GetComponent<TransformComponent>(info.m_Entity);
					//	transform.setPosition(Vec2{ transform.GetPosition().x - propstransform * g_deltaTime,transform.GetPosition().y });
					//	if (!g_engine.m_coordinator.ComponentExists<UIComponent>(info.m_Entity))
					//	{
					//		g_engine.m_coordinator.AddComponent(info.m_Entity, UIComponent());
					//		auto& UI = g_engine.m_coordinator.GetComponent<UIComponent>(info.m_Entity);
					//		UI.setIsActive(true);
					//	}
					//	transform.setZ(101);
					//}
					if (info.m_tag == "ExaClone")
					{
						auto& transform = g_engine.m_coordinator.GetComponent<TransformComponent>(info.m_Entity);
						transform.setZ(102);
						transform.setPosition(Vec2{ transform.GetPosition().x - clonetransformx * g_deltaTime,transform.GetPosition().y });
					}
					if (info.m_tag == "ElaClone")
					{
						auto& transform = g_engine.m_coordinator.GetComponent<TransformComponent>(info.m_Entity);
						transform.setZ(102);
						transform.setPosition(Vec2{ transform.GetPosition().x + clonetransformx * g_deltaTime,transform.GetPosition().y });
					}

				}
			}
			else if (m_timer > 15.0f && m_timer < 16.0f)
			{
				for (HierarchyInfo& info : g_engine.m_coordinator.GetHierarchyInfoArray())
				{
					if (info.m_tag == "ElaTitle")
					{
						auto& sprite = g_engine.m_coordinator.GetComponent<SpriteComponent>(info.m_Entity);
						sprite.setTexturePath("Resources/Assets/EndingEventEla.png");
						//sprite.setFilter(glm::vec4(255, 255, 255, sprite.getFilter().a + 0.1f));
					}

					if (info.m_tag == "ExaTitle")
					{
						auto& sprite = g_engine.m_coordinator.GetComponent<SpriteComponent>(info.m_Entity);
						sprite.setTexturePath("Resources/Assets/EndingEventExa.png");
						//sprite.setFilter(glm::vec4(255, 255, 255, sprite.getFilter().a + 0.1f));
					}

					//if (info.m_tag == "ElaA")
					//{
					//	//auto& sprite = g_engine.m_coordinator.GetComponent<SpriteComponent>(info.m_Entity);
					//	auto& transform = g_engine.m_coordinator.GetComponent<TransformComponent>(info.m_Entity);
					//	transform.setZ(-100);
					//	auto& UI = g_engine.m_coordinator.GetComponent<UIComponent>(info.m_Entity);
					//	UI.setIsActive(false);
					//}
					//
					//if (info.m_tag == "ExaA")
					//{
					//	//auto& sprite = g_engine.m_coordinator.GetComponent<SpriteComponent>(info.m_Entity);
					//
					//	//auto& transform = g_engine.m_coordinator.GetComponent<TransformComponent>(info.m_Entity);
					//	//transform.setZ(-100);
					//}
					if (info.m_tag == "ExaClone")
					{
						auto& sprite = g_engine.m_coordinator.GetComponent<SpriteComponent>(info.m_Entity);
						auto& animation = g_engine.m_coordinator.GetComponent<AnimationComponent>(info.m_Entity);
						sprite.setTexturePath("Resources/Assets/ExaResist.png");
						animation.setFrames(4);
					}
					if (info.m_tag == "ElaClone")
					{
						auto& sprite = g_engine.m_coordinator.GetComponent<SpriteComponent>(info.m_Entity);
						sprite.setTexturePath("Resources/Assets/ElaResist.png");
						auto& animation = g_engine.m_coordinator.GetComponent<AnimationComponent>(info.m_Entity);
						animation.setFrames(4);
					}

					if (!m_trueEnding)
					{
						if (info.m_tag == "AKey")
						{
							auto& transform = g_engine.m_coordinator.GetComponent<TransformComponent>(info.m_Entity);
							transform.setZ(102);
						}

						if (info.m_tag == "DKey")
						{
							auto& transform = g_engine.m_coordinator.GetComponent<TransformComponent>(info.m_Entity);
							transform.setZ(102);
						}
					}
				}
			}
			else if (m_timer > 16.0f)
			{
				if (!m_trueEnding)
				{
					if (!m_finalInput)
					{
						if (g_engine.m_coordinator.GetSystem<InputManager>()->KeyDown(KeyPress::KeyA))
						{
							if (!m_endingAPressed)
							{
								m_timer = 17.0f;
								m_endingAPressed = true;
								m_finalInput = true;
							}
						}
						else if (g_engine.m_coordinator.GetSystem<InputManager>()->KeyDown(KeyPress::KeyD))
						{
							if (!m_endingDPressed)
							{
								m_timer = 17.0f;
								m_endingDPressed = true;
								m_finalInput = true;
							}
						}
					}
				}

				if (m_trueEnding)
				{
					TrueEnding(sound);
				}

				else if (m_endingAPressed)
				{
					ExaEnding();
				}
				else if (m_endingDPressed)
				{
					ElaEnding();
				}
			}
		}
	}

	void GamePlayEnding::OnTriggerEnter(Entity other)
	{
		if (!g_engine.m_coordinator.GameIsActive())
			return;

		if (m_activated)
			return;

		if (g_engine.m_coordinator.ComponentExists<PlayerControllerComponent>(other))
		{
			//Freeze Player Controls			
			m_timer = 0.0f;

			if ((PLAYER_STATUS.GetCollectedSoulsInLevel(LEVEL::CRYSTAL) > PLAYER_STATUS.GetTotalSoulsInLevel(LEVEL::CRYSTAL) * 0.8 &&
				PLAYER_STATUS.GetCollectedSoulsInLevel(LEVEL::CORAL) > PLAYER_STATUS.GetTotalSoulsInLevel(LEVEL::CORAL) * 0.8 &&
				PLAYER_STATUS.GetCollectedSoulsInLevel(LEVEL::VEGETATION) > PLAYER_STATUS.GetTotalSoulsInLevel(LEVEL::VEGETATION) * 0.8) 
				|| PLAYER_STATUS.GetEndTrigger() || PLAYER_STATUS.GetTrueEndTrigger())
			{
				PLAYER_STATUS.FreezeControls();
				auto playerEnt = PlayerStatusManager::instance().GetPlayerEntity();
				auto& transform = g_engine.m_coordinator.GetComponent<TransformComponent>(playerEnt);
				if (transform.GetPosition().x - 330.0f > 0)
				{
					m_moveleft = true;
				}
				else
				{
					m_moveright = true;
				}
				g_engine.m_coordinator.GetSystem<CameraSystem>()->setIsActive(false);

				//Camera shake 2 seconds
				//CameraShakeEvent shake(20.0f);
				//shake.SetSystemReceivers(static_cast<int>(SystemID::id_CAMERASYSTEM));
				//EventDispatcher::instance().AddEvent(shake);
				for (HierarchyInfo& info : g_engine.m_coordinator.GetHierarchyInfoArray())
				{
					if (info.m_tag == "ObjectiveTxt")
					{
						auto& transform = g_engine.m_coordinator.GetComponent<TransformComponent>(info.m_Entity);
						transform.setZ(0);
					}
					if (info.m_tag == "slots1")
					{
						auto& transform = g_engine.m_coordinator.GetComponent<TransformComponent>(info.m_Entity);
						//show slots
						transform.setZ(100);
					}
					if (info.m_tag == "slots2")
					{
						auto& transform = g_engine.m_coordinator.GetComponent<TransformComponent>(info.m_Entity);
						//show slots
						transform.setZ(100);
					}
					if (info.m_tag == "slots3")
					{
						auto& transform = g_engine.m_coordinator.GetComponent<TransformComponent>(info.m_Entity);
						//show slots
						transform.setZ(100);
					}

					if (info.m_tag == "Player")
					{
						//auto& transform = g_engine.m_coordinator.GetComponent<TransformComponent>(info.m_Entity);
						////hide player
						//transform.setZ(-100);
						auto& camera = g_engine.m_coordinator.GetComponent<CameraComponent>(info.m_Entity);
						camera.setIsActive(false);
					}

					if (info.m_tag == "indicator")
					{
						auto& sprite = g_engine.m_coordinator.GetComponent<SpriteComponent>(info.m_Entity);
						//hide indicator
						auto& filter = sprite.getFilter();
						sprite.setFilter({ filter.r, filter.g, filter.b, 0 });
					}

					//if (info.m_tag == "ExaClone")
					//{
					//	auto& transform = g_engine.m_coordinator.GetComponent<TransformComponent>(info.m_Entity);
					//	//show clone
					//	transform.setZ(102);
					//}

					if (info.m_tag == "Camera")
					{
						auto& camera = g_engine.m_coordinator.GetComponent<CameraComponent>(info.m_Entity);
						camera.setIsActive(true);
					}



					//if (info.m_tag == "ElaClone")
					//{
					//	auto& transform = g_engine.m_coordinator.GetComponent<TransformComponent>(info.m_Entity);
					//	//show clone
					//	transform.setZ(102);
					//}
				}

				PlayerStatusManager::instance().SetEnding(true);

				if ((PLAYER_STATUS.GetCollectedSoulsInLevel(LEVEL::CORAL) == PLAYER_STATUS.GetTotalSoulsInLevel(LEVEL::CORAL) &&
					PLAYER_STATUS.GetCollectedSoulsInLevel(LEVEL::CRYSTAL) == PLAYER_STATUS.GetTotalSoulsInLevel(LEVEL::CRYSTAL) &&
					PLAYER_STATUS.GetCollectedSoulsInLevel(LEVEL::VEGETATION) == PLAYER_STATUS.GetTotalSoulsInLevel(LEVEL::VEGETATION) &&
					PLAYER_STATUS.GetCollectedSoulsInLevel(LEVEL::CORAL) != 0 &&
					PLAYER_STATUS.GetCollectedSoulsInLevel(LEVEL::CRYSTAL) != 0 &&
					PLAYER_STATUS.GetCollectedSoulsInLevel(LEVEL::VEGETATION) != 0) || 
					PLAYER_STATUS.GetTrueEndTrigger())
				{
					m_trueEnding = true;
				}

				m_activated = true;
			}
			
		}
	}

	void GamePlayEnding::TrueEnding(Sound& sound)
	{
		auto playerEnt = PlayerStatusManager::instance().GetPlayerEntity();
		auto& transform = g_engine.m_coordinator.GetComponent<TransformComponent>(playerEnt);

		//transform.setPosition(Vec2(CameraManager::instance().GetCameraPos().x, CameraManager::instance().GetCameraPos().y));
		
		for (HierarchyInfo& info : g_engine.m_coordinator.GetHierarchyInfoArray())
		{
			if (info.m_tag == "ExaTitle")
			{
				if (g_engine.m_coordinator.ComponentExists<UIComponent>(info.m_Entity))
				{
					auto& UI = g_engine.m_coordinator.GetComponent<UIComponent>(info.m_Entity);
					UI.setIsActive(false);
				}
			}

			if (info.m_tag == "ElaTitle")
			{
				if (g_engine.m_coordinator.ComponentExists<UIComponent>(info.m_Entity))
				{
					auto& UI = g_engine.m_coordinator.GetComponent<UIComponent>(info.m_Entity);
					UI.setIsActive(false);
				}
			}

			if (info.m_tag == "ElaA")
			{
				if (g_engine.m_coordinator.ComponentExists<UIComponent>(info.m_Entity))
				{
					auto& UI = g_engine.m_coordinator.GetComponent<UIComponent>(info.m_Entity);
					UI.setIsActive(false);
				}
			}

			if (info.m_tag == "ExaA")
			{
				if (g_engine.m_coordinator.ComponentExists<UIComponent>(info.m_Entity))
				{
					auto& UI = g_engine.m_coordinator.GetComponent<UIComponent>(info.m_Entity);
					UI.setIsActive(false);
				}
			}

			if (!m_finalSpriteSet)
			{
				if (info.m_tag == "EndingSprite")
				{
					if (!g_engine.m_coordinator.ComponentExists<UIComponent>(info.m_Entity))
					{
						g_engine.m_coordinator.AddComponent(info.m_Entity, UIComponent());
						auto& UI = g_engine.m_coordinator.GetComponent<UIComponent>(info.m_Entity);
						UI.setIsActive(true);
					}
					auto& transform = g_engine.m_coordinator.GetComponent<TransformComponent>(info.m_Entity);
					auto& sprite = g_engine.m_coordinator.GetComponent<SpriteComponent>(info.m_Entity);
					sprite.setTexturePath("Resources/Assets/True1.png");
					m_finalSprite = info.m_Entity;
					transform.setZ(103);
					m_finalSpriteSet = true;
				}
			}

			if (info.m_tag == "EndingBackground")
			{
				m_finalSpriteBackground = info.m_Entity;
			}
		}

		if (frame1)
		{
			auto& sprite = g_engine.m_coordinator.GetComponent<SpriteComponent>(m_finalSprite);
			sprite.setTexturePath("Resources/Assets/True2.png");
		}

		if (frame2)
		{
			auto& sprite = g_engine.m_coordinator.GetComponent<SpriteComponent>(m_finalSprite);
			sprite.setTexturePath("Resources/Assets/True3.png");
		}

		if (frame3)
		{
			auto& sprite = g_engine.m_coordinator.GetComponent<SpriteComponent>(m_finalSprite);
			sprite.setTexturePath("Resources/Assets/True4.png");
		}

		if (frame4)
		{
			auto& sprite = g_engine.m_coordinator.GetComponent<SpriteComponent>(m_finalSprite);
			sprite.setTexturePath("Resources/Assets/True5.png");
		}

		if (frame5)
		{
			auto& sprite = g_engine.m_coordinator.GetComponent<SpriteComponent>(m_finalSpriteBackground);
			auto& transform = g_engine.m_coordinator.GetComponent<TransformComponent>(m_finalSpriteBackground);
			for (HierarchyInfo& info : g_engine.m_coordinator.GetHierarchyInfoArray())
			{
				if (info.m_tag == "EndingBackground")
				{
					if (!g_engine.m_coordinator.ComponentExists<UIComponent>(info.m_Entity))
					{
						g_engine.m_coordinator.AddComponent(info.m_Entity, UIComponent());
						auto& UI = g_engine.m_coordinator.GetComponent<UIComponent>(info.m_Entity);
						UI.setIsActive(true);
					}
					auto& transform = g_engine.m_coordinator.GetComponent<TransformComponent>(info.m_Entity);
					transform.setZ(1001);
				}
			}
			sprite.setFilter(glm::vec4(sprite.getFilter().r, sprite.getFilter().g, sprite.getFilter().b, sprite.getFilter().a + backgroundfilterchange * g_deltaTime));
		}

		if (frame6)
		{
			auto& spritebackground = g_engine.m_coordinator.GetComponent<SpriteComponent>(m_finalSpriteBackground);
			auto& sprite = g_engine.m_coordinator.GetComponent<SpriteComponent>(m_finalSprite);
			spritebackground.setFilter(glm::vec4(spritebackground.getFilter().r, spritebackground.getFilter().g, spritebackground.getFilter().b, spritebackground.getFilter().a - backgroundfilterchange * g_deltaTime));
			sprite.setTexturePath("Resources/Assets/True6.png");
		}

		if (frame7)
		{
			auto& sprite = g_engine.m_coordinator.GetComponent<SpriteComponent>(m_finalSprite);
			sprite.setTexturePath("Resources/Assets/True7.png");
		}

		if (frame8)
		{
			auto& sprite = g_engine.m_coordinator.GetComponent<SpriteComponent>(m_finalSprite);
			sprite.setTexturePath("Resources/Assets/True8.png");
		}

		if (frame9)
		{
			auto& sprite = g_engine.m_coordinator.GetComponent<SpriteComponent>(m_finalSprite);
			sprite.setTexturePath("Resources/Assets/True9.png");
		}

		if (frame10)
		{
			auto& sprite = g_engine.m_coordinator.GetComponent<SpriteComponent>(m_finalSprite);
			sprite.setTexturePath("Resources/Assets/True10.png");
		}

		if (frame11)
		{
			auto& sprite = g_engine.m_coordinator.GetComponent<SpriteComponent>(m_finalSprite);
			sprite.setTexturePath("Resources/Assets/True11.png");
		}

		if (frame12)
		{
			auto& sprite = g_engine.m_coordinator.GetComponent<SpriteComponent>(m_finalSprite);
			sprite.setTexturePath("Resources/Assets/True12.png");
		}

		if (frame13)
		{
			auto& sprite = g_engine.m_coordinator.GetComponent<SpriteComponent>(m_finalSprite);
			sprite.setTexturePath("Resources/Assets/True13.png");
		}

		if (m_timer > 30.0f && m_timer < 31.0f)
		{
			auto& sprite = g_engine.m_coordinator.GetComponent<SpriteComponent>(m_finalSprite);
			sprite.setTexturePath("Resources/Assets/True14.png");
		}

		if (m_timer > 31.0f && m_timer < 32.0f)
		{
			auto& sprite = g_engine.m_coordinator.GetComponent<SpriteComponent>(m_finalSprite);
			sprite.setTexturePath("Resources/Assets/True15.png");
		}

		if (m_timer > 32.0f && m_timer < 33.0f)
		{
			auto& sprite = g_engine.m_coordinator.GetComponent<SpriteComponent>(m_finalSprite);
			sprite.setTexturePath("Resources/Assets/True16.png");
		}

		if (m_timer > 33.0f && m_timer < 34.0f)
		{
			auto& sprite = g_engine.m_coordinator.GetComponent<SpriteComponent>(m_finalSprite);
			sprite.setTexturePath("Resources/Assets/True17.png");
		}

		if (m_timer > 34.0f && m_timer < 35.0f)
		{
			auto& sprite = g_engine.m_coordinator.GetComponent<SpriteComponent>(m_finalSprite);
			sprite.setTexturePath("Resources/Assets/True18.png");
		}

		if (m_timer > 35.0f && m_timer < 36.0f)
		{
			auto& sprite = g_engine.m_coordinator.GetComponent<SpriteComponent>(m_finalSprite);
			sprite.setTexturePath("Resources/Assets/True19.png");
		}

		if (m_timer > 36.0f && m_timer < 37.0f)
		{
			auto& sprite = g_engine.m_coordinator.GetComponent<SpriteComponent>(m_finalSprite);
			sprite.setTexturePath("Resources/Assets/True20.png");
		}

		if (m_timer > 37.0f && m_timer < 38.0f)
		{
			auto& sprite = g_engine.m_coordinator.GetComponent<SpriteComponent>(m_finalSprite);
			sprite.setTexturePath("Resources/Assets/True21.png");
		}

		if (m_timer > 41.0f)
		{
			sound.Pause(true);
			//sound.Unload();
			AudioManager::instance().getAudioMap().erase("Resources/Sounds/Exale_Ending.ogg");
			m_soundloaded = !m_soundloaded;
			PLAYER_STATUS.ResetEndGame();
			g_engine.m_coordinator.SetTransitionLevel("Level 19.json", 0.0f); //2nd value doesn't matter anymore probably
			g_engine.m_coordinator.SetTransition(true);
		}
	}

	void GamePlayEnding::ExaEnding()
	{
		auto playerEnt = PlayerStatusManager::instance().GetPlayerEntity();
		auto& transform = g_engine.m_coordinator.GetComponent<TransformComponent>(playerEnt);

		//transform.setPosition(Vec2(CameraManager::instance().GetCameraPos().x, CameraManager::instance().GetCameraPos().y));

		auto sound = g_engine.m_coordinator.loadSound("Resources/Sounds/Exale_Normal_Ending.ogg");
		if (!m_soundloaded)
		{
			sound.Play();
			m_soundloaded = !m_soundloaded;
		}

		if (frame1)
		{
			for (HierarchyInfo& info : g_engine.m_coordinator.GetHierarchyInfoArray())
			{
				if (info.m_tag == "ExaTitle")
				{
					if (g_engine.m_coordinator.ComponentExists<UIComponent>(info.m_Entity))
					{
						auto& UI = g_engine.m_coordinator.GetComponent<UIComponent>(info.m_Entity);
						UI.setIsActive(false);
					}
				}

				if (info.m_tag == "ElaTitle")
				{
					if (g_engine.m_coordinator.ComponentExists<UIComponent>(info.m_Entity))
					{
						auto& UI = g_engine.m_coordinator.GetComponent<UIComponent>(info.m_Entity);
						UI.setIsActive(false);
					}
				}

				if (info.m_tag == "ElaA")
				{
					if (g_engine.m_coordinator.ComponentExists<UIComponent>(info.m_Entity))
					{
						auto& UI = g_engine.m_coordinator.GetComponent<UIComponent>(info.m_Entity);
						UI.setIsActive(false);
					}
				}

				if (info.m_tag == "ExaA")
				{
					if (g_engine.m_coordinator.ComponentExists<UIComponent>(info.m_Entity))
					{
						auto& UI = g_engine.m_coordinator.GetComponent<UIComponent>(info.m_Entity);
						UI.setIsActive(false);
					}
				}

				if (!m_finalSpriteSet)
				{
					if (info.m_tag == "EndingSprite")
					{
						if (!g_engine.m_coordinator.ComponentExists<UIComponent>(info.m_Entity))
						{
							g_engine.m_coordinator.AddComponent(info.m_Entity, UIComponent());
							auto& UI = g_engine.m_coordinator.GetComponent<UIComponent>(info.m_Entity);
							UI.setIsActive(true);
						}
						auto& transform = g_engine.m_coordinator.GetComponent<TransformComponent>(info.m_Entity);
						auto& sprite = g_engine.m_coordinator.GetComponent<SpriteComponent>(info.m_Entity);
						sprite.setTexturePath("Resources/Assets/ExaS1.png");
						m_finalSprite = info.m_Entity;
						transform.setZ(103);
						m_finalSpriteSet = true;
					}
				}

				if (info.m_tag == "EndingBackground")
				{
					m_finalSpriteBackground = info.m_Entity;
				}
			}
		}

		if (frame2)
		{
			auto& sprite = g_engine.m_coordinator.GetComponent<SpriteComponent>(m_finalSprite);
			sprite.setTexturePath("Resources/Assets/ExaS2.png");
		}

		if (frame3)
		{
			auto& sprite = g_engine.m_coordinator.GetComponent<SpriteComponent>(m_finalSprite);
			sprite.setTexturePath("Resources/Assets/ExaS3.png");
		}

		if (frame4)
		{
			auto& sprite = g_engine.m_coordinator.GetComponent<SpriteComponent>(m_finalSprite);
			sprite.setTexturePath("Resources/Assets/ExaS4.png");
		}

		if (frame5)
		{
			auto& sprite = g_engine.m_coordinator.GetComponent<SpriteComponent>(m_finalSprite);
			sprite.setTexturePath("Resources/Assets/ExaS5.png");
		}

		if (frame6)
		{
			auto& sprite = g_engine.m_coordinator.GetComponent<SpriteComponent>(m_finalSpriteBackground);
			auto& transform = g_engine.m_coordinator.GetComponent<TransformComponent>(m_finalSpriteBackground);
			for (HierarchyInfo& info : g_engine.m_coordinator.GetHierarchyInfoArray())
			{
				if (info.m_tag == "EndingBackground")
				{
					if (!g_engine.m_coordinator.ComponentExists<UIComponent>(info.m_Entity))
					{
						g_engine.m_coordinator.AddComponent(info.m_Entity, UIComponent());
						auto& UI = g_engine.m_coordinator.GetComponent<UIComponent>(info.m_Entity);
						UI.setIsActive(true);
					}
					auto& transform = g_engine.m_coordinator.GetComponent<TransformComponent>(info.m_Entity);
					transform.setZ(1001);
				}
			}
			sprite.setFilter(glm::vec4(sprite.getFilter().r, sprite.getFilter().g , sprite.getFilter().b , sprite.getFilter().a + backgroundfilterchange * g_deltaTime));
		}

		if (frame7)
		{
			auto& spritebackground = g_engine.m_coordinator.GetComponent<SpriteComponent>(m_finalSpriteBackground);
			auto& sprite = g_engine.m_coordinator.GetComponent<SpriteComponent>(m_finalSprite);
			spritebackground.setFilter(glm::vec4(spritebackground.getFilter().r, spritebackground.getFilter().g, spritebackground.getFilter().b, spritebackground.getFilter().a - backgroundfilterchange * g_deltaTime));
			sprite.setTexturePath("Resources/Assets/ExaS6.png");
		}

		if (frame8)
		{
			auto& sprite = g_engine.m_coordinator.GetComponent<SpriteComponent>(m_finalSprite);
			sprite.setTexturePath("Resources/Assets/ExaS7.png");
		}

		if (frame9)
		{
			auto& sprite = g_engine.m_coordinator.GetComponent<SpriteComponent>(m_finalSprite);
			sprite.setTexturePath("Resources/Assets/ExaS8.png");
		}

		if (frame10)
		{
			auto& sprite = g_engine.m_coordinator.GetComponent<SpriteComponent>(m_finalSprite);
			sprite.setTexturePath("Resources/Assets/ExaS9.png");
		}

		if (frame11)
		{
			auto& sprite = g_engine.m_coordinator.GetComponent<SpriteComponent>(m_finalSprite);
			sprite.setTexturePath("Resources/Assets/ExaS10.png");
		}

		if (frame12)
		{
			auto& sprite = g_engine.m_coordinator.GetComponent<SpriteComponent>(m_finalSprite);
			sprite.setTexturePath("Resources/Assets/ExaS11.png");
		}

		if (frame13)
		{
			auto& sprite = g_engine.m_coordinator.GetComponent<SpriteComponent>(m_finalSprite);
			sprite.setTexturePath("Resources/Assets/ExaS12.png");
		}

		if (frame14)
		{
			auto& sprite = g_engine.m_coordinator.GetComponent<SpriteComponent>(m_finalSprite);
			sprite.setTexturePath("Resources/Assets/ExaS13.png");
		}

		if (frame15)
		{
			auto& sprite = g_engine.m_coordinator.GetComponent<SpriteComponent>(m_finalSprite);
			sprite.setTexturePath("Resources/Assets/ExaS14.png");
		}

		if (frame16)
		{
			auto& sprite = g_engine.m_coordinator.GetComponent<SpriteComponent>(m_finalSprite);
			sprite.setTexturePath("Resources/Assets/ExaS15.png");
		}

		if (frame17)
		{
			auto& sprite = g_engine.m_coordinator.GetComponent<SpriteComponent>(m_finalSprite);
			sprite.setTexturePath("Resources/Assets/ExaS16.png");
		}

		if (m_timer > 35.0f)
		{
			sound.Pause(true);
			//sound.Unload();
			AudioManager::instance().getAudioMap().erase("Resources/Sounds/Exale_Normal_Ending.ogg");
			m_soundloaded = !m_soundloaded;
			PLAYER_STATUS.ResetEndGame();
			g_engine.m_coordinator.SetTransitionLevel("Level 28.json", 0.0f); //2nd value doesn't matter anymore probably
			g_engine.m_coordinator.SetTransition(true);
		}
	}

	void GamePlayEnding::ElaEnding()
	{
		auto playerEnt = PlayerStatusManager::instance().GetPlayerEntity();
		auto& transform = g_engine.m_coordinator.GetComponent<TransformComponent>(playerEnt);

	//	transform.setPosition(Vec2(CameraManager::instance().GetCameraPos().x, CameraManager::instance().GetCameraPos().y));

		auto sound = g_engine.m_coordinator.loadSound("Resources/Sounds/Exale_Normal_Ending.ogg");
		if (!m_soundloaded)
		{
			sound.Play();
			m_soundloaded = !m_soundloaded;
		}

		if (frame1)
		{
			for (HierarchyInfo& info : g_engine.m_coordinator.GetHierarchyInfoArray())
			{
				if (info.m_tag == "ExaTitle")
				{
					if (g_engine.m_coordinator.ComponentExists<UIComponent>(info.m_Entity))
					{
						auto& UI = g_engine.m_coordinator.GetComponent<UIComponent>(info.m_Entity);
						UI.setIsActive(false);
					}
				}

				if (info.m_tag == "ElaTitle")
				{
					if (g_engine.m_coordinator.ComponentExists<UIComponent>(info.m_Entity))
					{
						auto& UI = g_engine.m_coordinator.GetComponent<UIComponent>(info.m_Entity);
						UI.setIsActive(false);
					}
				}

				if (info.m_tag == "ElaA")
				{
					if (g_engine.m_coordinator.ComponentExists<UIComponent>(info.m_Entity))
					{
						auto& UI = g_engine.m_coordinator.GetComponent<UIComponent>(info.m_Entity);
						UI.setIsActive(false);
					}
				}

				if (info.m_tag == "ExaA")
				{
					if (g_engine.m_coordinator.ComponentExists<UIComponent>(info.m_Entity))
					{
						auto& UI = g_engine.m_coordinator.GetComponent<UIComponent>(info.m_Entity);
						UI.setIsActive(false);
					}
				}

				if (!m_finalSpriteSet)
				{
					if (info.m_tag == "EndingSprite")
					{
						if (!g_engine.m_coordinator.ComponentExists<UIComponent>(info.m_Entity))
						{
							g_engine.m_coordinator.AddComponent(info.m_Entity, UIComponent());
							auto& UI = g_engine.m_coordinator.GetComponent<UIComponent>(info.m_Entity);
							UI.setIsActive(true);
						}
						auto& transform = g_engine.m_coordinator.GetComponent<TransformComponent>(info.m_Entity);
						auto& sprite = g_engine.m_coordinator.GetComponent<SpriteComponent>(info.m_Entity);
						sprite.setTexturePath("Resources/Assets/ElaS1.png");
						m_finalSprite = info.m_Entity;
						transform.setZ(103);
						m_finalSpriteSet = true;
					}
				}

				if (info.m_tag == "EndingBackground")
				{
					m_finalSpriteBackground = info.m_Entity;
				}
			}
		}



		if (frame2)
		{
			auto& sprite = g_engine.m_coordinator.GetComponent<SpriteComponent>(m_finalSprite);
			sprite.setTexturePath("Resources/Assets/ElaS2.png");
		}

		if (frame3)
		{
			auto& sprite = g_engine.m_coordinator.GetComponent<SpriteComponent>(m_finalSprite);
			sprite.setTexturePath("Resources/Assets/ElaS3.png");
		}

		if (frame4)
		{
			auto& sprite = g_engine.m_coordinator.GetComponent<SpriteComponent>(m_finalSprite);
			sprite.setTexturePath("Resources/Assets/ElaS4.png");
		}

		if (frame5)
		{
			auto& sprite = g_engine.m_coordinator.GetComponent<SpriteComponent>(m_finalSprite);
			sprite.setTexturePath("Resources/Assets/ElaS5.png");
		}

		if (frame6)
		{
			auto& sprite = g_engine.m_coordinator.GetComponent<SpriteComponent>(m_finalSpriteBackground);
			auto& transform = g_engine.m_coordinator.GetComponent<TransformComponent>(m_finalSpriteBackground);
			for (HierarchyInfo& info : g_engine.m_coordinator.GetHierarchyInfoArray())
			{
				if (info.m_tag == "EndingBackground")
				{
					if (!g_engine.m_coordinator.ComponentExists<UIComponent>(info.m_Entity))
					{
						g_engine.m_coordinator.AddComponent(info.m_Entity, UIComponent());
						auto& UI = g_engine.m_coordinator.GetComponent<UIComponent>(info.m_Entity);
						UI.setIsActive(true);
					}
					auto& transform = g_engine.m_coordinator.GetComponent<TransformComponent>(info.m_Entity);
					transform.setZ(1001);
				}
			}
			sprite.setFilter(glm::vec4(sprite.getFilter().r, sprite.getFilter().g, sprite.getFilter().b, sprite.getFilter().a + backgroundfilterchange * g_deltaTime));
		}

		if (frame7)
		{
			auto& spritebackground = g_engine.m_coordinator.GetComponent<SpriteComponent>(m_finalSpriteBackground);
			auto& sprite = g_engine.m_coordinator.GetComponent<SpriteComponent>(m_finalSprite);
			spritebackground.setFilter(glm::vec4(spritebackground.getFilter().r, spritebackground.getFilter().g, spritebackground.getFilter().b, spritebackground.getFilter().a - backgroundfilterchange * g_deltaTime));
			sprite.setTexturePath("Resources/Assets/ElaS6.png");
		}

		if (frame8)
		{
			auto& sprite = g_engine.m_coordinator.GetComponent<SpriteComponent>(m_finalSprite);
			sprite.setTexturePath("Resources/Assets/ElaS7.png");
		}

		if (frame9)
		{
			auto& sprite = g_engine.m_coordinator.GetComponent<SpriteComponent>(m_finalSprite);
			sprite.setTexturePath("Resources/Assets/ElaS8.png");
		}

		if (frame10)
		{
			auto& sprite = g_engine.m_coordinator.GetComponent<SpriteComponent>(m_finalSprite);
			sprite.setTexturePath("Resources/Assets/ElaS9.png");
		}

		if (frame11)
		{
			auto& sprite = g_engine.m_coordinator.GetComponent<SpriteComponent>(m_finalSprite);
			sprite.setTexturePath("Resources/Assets/ElaS10.png");
		}

		if (frame12)
		{
			auto& sprite = g_engine.m_coordinator.GetComponent<SpriteComponent>(m_finalSprite);
			sprite.setTexturePath("Resources/Assets/ElaS11.png");
		}

		if (frame13)
		{
			auto& sprite = g_engine.m_coordinator.GetComponent<SpriteComponent>(m_finalSprite);
			sprite.setTexturePath("Resources/Assets/ElaS12.png");
		}

		if (frame14)
		{
			auto& sprite = g_engine.m_coordinator.GetComponent<SpriteComponent>(m_finalSprite);
			sprite.setTexturePath("Resources/Assets/ElaS13.png");
		}

		if (frame15)
		{
			auto& sprite = g_engine.m_coordinator.GetComponent<SpriteComponent>(m_finalSprite);
			sprite.setTexturePath("Resources/Assets/ElaS14.png");
		}

		if (frame16)
		{
			auto& sprite = g_engine.m_coordinator.GetComponent<SpriteComponent>(m_finalSprite);
			sprite.setTexturePath("Resources/Assets/ElaS15.png");
		}

		if (frame17)
		{
			auto& sprite = g_engine.m_coordinator.GetComponent<SpriteComponent>(m_finalSprite);
			sprite.setTexturePath("Resources/Assets/ElaS16.png");
		}


		if (m_timer > 35.0f)
		{
			sound.Pause(true);
			//sound.Unload();
			AudioManager::instance().getAudioMap().erase("Resources/Sounds/Exale_Normal_Ending.ogg");
			m_soundloaded = !m_soundloaded;

			PLAYER_STATUS.ResetEndGame();
			g_engine.m_coordinator.SetTransitionLevel("Level 27.json", 0.0f); //2nd value doesn't matter anymore probably
			g_engine.m_coordinator.SetTransition(true);
		}
	}
}