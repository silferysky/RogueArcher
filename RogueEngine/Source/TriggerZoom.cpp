#include "Precompiled.h"
#include "TriggerZoom.h"
#include "CameraManager.h"
#include "Main.h"

namespace Rogue
{
	TriggerZoom::TriggerZoom(Entity entity, LogicComponent& logicComponent, StatsComponent& statsComponent, float zoomValue, float zoomDuration, float zoomDelay, unsigned doCount)
		: ScriptComponent(entity, logicComponent, statsComponent), 
			m_isZooming {false},
			m_returning {false},
			m_doCount {doCount},
			m_zoomValueFinal{ zoomValue }, 
			m_zoomValueInit {0.0f},
			m_zoomDuration{ zoomDuration },
			m_zoomTimer{m_zoomDelay},
			m_zoomDelay {zoomDelay},
			m_zoomFactor {0.001f}{}

	void TriggerZoom::AIIdleUpdate()
	{
		if (!m_isZooming || m_doCount == 0)
			return;

		float cameraZoom = CameraManager::instance().GetCameraZoom();

		//If waiting for delay
		if (m_zoomTimer < m_zoomDelay)
		{
			m_zoomTimer += g_deltaTime; //* g_engine.GetTimeScale();
		}
		else if (m_zoomTimer >= m_zoomDelay)
		{
			CameraManager::instance().SetCameraZoom(cameraZoom + m_zoomFactor);

			//If the zoom is at the end
			if (std::abs(cameraZoom - m_zoomValueFinal) < 0.001f)
			{
				//Counter check
				if (m_returning)
					--m_doCount;

				if (m_doCount == 0)
				{
					m_isZooming = false;
					return; //No need to do the rest since not coming back here
				}

				m_returning = !m_returning;
				m_zoomTimer = 0.0f;

				float tempZoom = m_zoomValueFinal;
				m_zoomValueFinal = m_zoomValueInit;
				m_zoomValueInit = tempZoom;
				m_zoomFactor = (m_zoomValueFinal - m_zoomValueInit) / m_zoomDuration;
			}
		}
	}

	void TriggerZoom::OnTriggerEnter(Entity otherEnt)
	{
		if (otherEnt != PLAYER_STATUS.GetPlayerEntity())
			return;

		m_isZooming = true;
		m_zoomValueInit = CameraManager::instance().GetCameraZoom();
		m_zoomFactor = (m_zoomValueFinal - m_zoomValueInit) / m_zoomDuration;
	}
}
