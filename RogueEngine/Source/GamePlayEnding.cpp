#include "Precompiled.h"
#include "GamePlayEnding.h"
#include "PlayerStatusManager.h"
//#include "Main.h"
#include "GraphicsEvent.h"
#include "CameraManager.h"
#include "CameraSystem.h"
#include "InputManager.h"

namespace Rogue
{
	GamePlayEnding::GamePlayEnding(Entity entity, LogicComponent& logicComponent, StatsComponent& statsComponent)
		: ScriptComponent(entity, logicComponent, statsComponent)
	{
	}

	void GamePlayEnding::AIActiveStateUpdate()
	{
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
			//Freeze Player Controls			
			//m_souls = PLAYER_STATUS.GetSoulsCollected();
			//if (m_souls < 7)
			//{
			//}
			//else
			//{
			//
			//}
			//8. <Fade in/out 3 secs, display statement on top of the camera>
			if (m_timer < 3.0f)
			{
				for (HierarchyInfo& info : g_engine.m_coordinator.GetHierarchyInfoArray())
				{
					if (info.m_tag == "DoorUp")
					{
						auto& transform = g_engine.m_coordinator.GetComponent<TransformComponent>(info.m_Entity);
						transform.setPosition(Vec2{ transform.GetPosition().x,transform.GetPosition().y + 0.2f });
					}
					if (info.m_tag == "DoorDown")
					{
						auto& transform = g_engine.m_coordinator.GetComponent<TransformComponent>(info.m_Entity);
						transform.setPosition(Vec2{ transform.GetPosition().x,transform.GetPosition().y - 0.2f });
					}
					if (info.m_tag == "DoorLeft")
					{
						auto& transform = g_engine.m_coordinator.GetComponent<TransformComponent>(info.m_Entity);
						transform.setPosition(Vec2{ transform.GetPosition().x - 0.2f,transform.GetPosition().y });
					}
					if (info.m_tag == "DoorRight")
					{
						auto& transform = g_engine.m_coordinator.GetComponent<TransformComponent>(info.m_Entity);
						transform.setPosition(Vec2{ transform.GetPosition().x + 0.2f,transform.GetPosition().y });
					}
				}

				FadeEvent ev = FadeEvent(MAX_ENTITIES, 0.5f, false);
				ev.SetSystemReceivers(static_cast<int>(SystemID::id_GRAPHICSSYSTEM));
				EventDispatcher::instance().AddEvent(ev);
			}
			//9. < Fade in 3 secs, display choice input statement on top of the camera >
			//else if (m_timer < 6.0f)
			//{
			//
			//}
			////11.  <Inputted, Fade out choice input statement 3 secs and remove the input image>
			//else if (m_timer < 9.0f)
			//{
			//
			//}
			////12. < Fade in Black image to cover the camera, 3 secs >
			//else if (m_timer < 12.0f)
			//{
			//
			//}
			////13.<Fade in/out cutscenes according to ED1/2, 3 secs respectively for each cutscene duration>
			//else if (m_timer < 15.0f)
			//{
			//
			//}
			//14.<Fade in Black image to cover the camera, 3 secs>
			else
			{
				m_timer = 0.0f;
				PLAYER_STATUS.UnfreezeControls();
				for (HierarchyInfo& info : g_engine.m_coordinator.GetHierarchyInfoArray())
				{
					if (info.m_tag == "slots")
					{
						auto& transform = g_engine.m_coordinator.GetComponent<TransformComponent>(info.m_Entity);
						transform.setZ(0);
					}
				}
				CameraManager::instance().SetCameraZoom(CameraManager::instance().GetCameraZoom() - 0.2f);
				PlayerStatusManager::instance().SetEnding(false);
			}
			
		}
	}

	void GamePlayEnding::OnTriggerEnter(Entity other)
	{
		if (g_engine.m_coordinator.ComponentExists<PlayerControllerComponent>(other))
		{
			PLAYER_STATUS.FreezeControls();
			g_engine.m_coordinator.GetSystem<CameraSystem>()->setIsActive(false);
			//Zoom out slightly
			CameraManager::instance().SetCameraZoom(CameraManager::instance().GetCameraZoom() + 0.2f);
			//Camera shake 2 seconds
			CameraShakeEvent shake(20.0f);
			shake.SetSystemReceivers(static_cast<int>(SystemID::id_CAMERASYSTEM));
			EventDispatcher::instance().AddEvent(shake);
			for (HierarchyInfo& info : g_engine.m_coordinator.GetHierarchyInfoArray())
			{			
				if (info.m_tag == "slots")
				{
					auto& transform = g_engine.m_coordinator.GetComponent<TransformComponent>(info.m_Entity);
					transform.setZ(100);
				}
			}
			PlayerStatusManager::instance().SetEnding(true);
		}
	}
}