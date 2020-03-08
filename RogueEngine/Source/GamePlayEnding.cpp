#include "Precompiled.h"
#include "GamePlayEnding.h"
#include "PlayerStatusManager.h"
//#include "Main.h"
#include "GraphicsEvent.h"
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
		m_timer += g_deltaTime * g_engine.GetTimeScale();
		//8. <Fade in/out 3 secs, display statement on top of the camera>
		if (m_timer < 3.0f)
		{

		}
		//9. < Fade in 3 secs, display choice input statement on top of the camera >
		else if (m_timer < 6.0f)
		{

		}
		//11.  <Inputted, Fade out choice input statement 3 secs and remove the input image>
		else if (m_timer < 9.0f)
		{

		}
		//12. < Fade in Black image to cover the camera, 3 secs >
		else if (m_timer < 12.0f)
		{

		}
		//13.<Fade in/out cutscenes according to ED1/2, 3 secs respectively for each cutscene duration>
		else if (m_timer < 15.0f)
		{
			
		}
		//14.<Fade in Black image to cover the camera, 3 secs>
		else if (m_timer < 18.0f)
		{
			m_timer = 0.0f;
		}
	}

	void GamePlayEnding::OnTriggerEnter(Entity other)
	{
		if (g_engine.m_coordinator.ComponentExists<PlayerControllerComponent>(other))
		{
			//PLAYER_STATUS
			//zoom
			m_souls = PLAYER_STATUS.GetSoulsCollected();
			if (m_souls < 7)
			{
				if (auto camera = g_engine.m_coordinator.TryGetComponent<CameraComponent>(m_entity))
				{
					//camera->get()
				}

				CameraShakeEvent shake(20.0f);
				shake.SetSystemReceivers(static_cast<int>(SystemID::id_CAMERASYSTEM));
				EventDispatcher::instance().AddEvent(shake);

				if (auto fade = g_engine.m_coordinator.TryGetComponent<FadeComponent>(m_entity))
				{
					fade->get().setIsActive(true);
					fade->get().setIsFadingIn(false);
				}
			}
			else
			{

			}
		}
	}
}