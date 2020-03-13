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
#define clonetransformx 100.0f
namespace Rogue
{
	GamePlayEnding::GamePlayEnding(Entity entity, LogicComponent& logicComponent, StatsComponent& statsComponent)
		: ScriptComponent(entity, logicComponent, statsComponent), m_souls{ 0 }, m_timer{ 0.0f }, m_activated{ false },m_movement{15.0f},m_moveleft{false},m_moveright{false}
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
			g_engine.m_coordinator.GetSystem<CameraSystem>()->setIsActive(true);
			if(g_engine.m_coordinator.GetSystem<InputManager>()->KeyDown(KeyPress::KeyA))
			{
				
			}
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
					if (info.m_tag == "ExaA")
					{
						auto& sprite = g_engine.m_coordinator.GetComponent<SpriteComponent>(info.m_Entity);
						auto& transform = g_engine.m_coordinator.GetComponent<TransformComponent>(info.m_Entity);
						sprite.setFilter(glm::vec4(sprite.getFilter().r, sprite.getFilter().g, sprite.getFilter().b, sprite.getFilter().a + filterchange * g_deltaTime));
						//transform.setZ(101);
					}
				}
			}
			else if (m_timer > 9.0f && m_timer < 12.0f)
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
					if (info.m_tag == "ElaA")
					{
						auto& transform = g_engine.m_coordinator.GetComponent<TransformComponent>(info.m_Entity);
						transform.setPosition(Vec2{ transform.GetPosition().x + propstransform * g_deltaTime,transform.GetPosition().y });
						if (!g_engine.m_coordinator.ComponentExists<UIComponent>(info.m_Entity))
						{
							g_engine.m_coordinator.AddComponent(info.m_Entity, UIComponent());
							auto& UI = g_engine.m_coordinator.GetComponent<UIComponent>(info.m_Entity);
							UI.setIsActive(true);
						}
						transform.setZ(101);
					}
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
					if (info.m_tag == "ExaA")
					{
						auto& transform = g_engine.m_coordinator.GetComponent<TransformComponent>(info.m_Entity);
						transform.setPosition(Vec2{ transform.GetPosition().x - propstransform * g_deltaTime,transform.GetPosition().y });
						if (!g_engine.m_coordinator.ComponentExists<UIComponent>(info.m_Entity))
						{
							g_engine.m_coordinator.AddComponent(info.m_Entity, UIComponent());
							auto& UI = g_engine.m_coordinator.GetComponent<UIComponent>(info.m_Entity);
							UI.setIsActive(true);
						}
						transform.setZ(101);
					}
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
			else if (m_timer > 15.0f && m_timer < 18.0f)
			{
				for (HierarchyInfo& info : g_engine.m_coordinator.GetHierarchyInfoArray())
				{
					if (info.m_tag == "ElaTitle")
					{
						auto& sprite = g_engine.m_coordinator.GetComponent<SpriteComponent>(info.m_Entity);
						sprite.setTexturePath("Resources/Assets/EndingEventEla.png");
						//sprite.setFilter(glm::vec4(255, 255, 255, sprite.getFilter().a + 0.1f));
					}
					if (info.m_tag == "ElaA")
					{
						//auto& sprite = g_engine.m_coordinator.GetComponent<SpriteComponent>(info.m_Entity);
						auto& transform = g_engine.m_coordinator.GetComponent<TransformComponent>(info.m_Entity);
						transform.setZ(-100);
						auto& UI = g_engine.m_coordinator.GetComponent<UIComponent>(info.m_Entity);
						UI.setIsActive(false);
					}

					if (info.m_tag == "ExaA")
					{
						//auto& sprite = g_engine.m_coordinator.GetComponent<SpriteComponent>(info.m_Entity);

						//auto& transform = g_engine.m_coordinator.GetComponent<TransformComponent>(info.m_Entity);
						//transform.setZ(-100);
					}
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
				}
			}
			else
			{
				//m_timer = 0.0f;
				//PLAYER_STATUS.UnfreezeControls();
				//for (HierarchyInfo& info : g_engine.m_coordinator.GetHierarchyInfoArray())
				//{
				//	if (info.m_tag == "slots")
				//	{
				//		auto& transform = g_engine.m_coordinator.GetComponent<TransformComponent>(info.m_Entity);
				//		transform.setZ(0);
				//	}
				//}
				//CameraManager::instance().SetCameraZoom(CameraManager::instance().GetCameraZoom() + 0.5f);
				//PlayerStatusManager::instance().SetEnding(false);
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
			m_souls = PLAYER_STATUS.GetSoulsCollected();
			if (m_souls < 1)
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
					if (info.m_tag == "slots")
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
				m_activated = true;
			}
			
		}
	}
}