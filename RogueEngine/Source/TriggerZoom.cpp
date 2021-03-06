/* Start Header ************************************************************************/
/*!
\file           TriggerZoom.cpp
\project        Exale
\author         Chan Wai Kit Terence, c.terence, 440005918 (100%)
\par            c.terence\@digipen.edu
\date           3 April,2020
\brief          This file contains the functions for TriggerZoom

All content (C) 2020 DigiPen (SINGAPORE) Corporation, all rights
reserved.

Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header **************************************************************************/
#include "Precompiled.h"
#include "TriggerZoom.h"
#include "CameraManager.h"
//#include "Main.h"

namespace Rogue
{
	TriggerZoom::TriggerZoom(Entity entity, LogicComponent& logicComponent, StatsComponent& statsComponent)
		: ScriptComponent(entity, logicComponent, statsComponent), 
			m_isZooming {false},
			m_returning {false},
			m_doCount { statsComponent.GetZoomCount()},
			m_zoomValueFinal{ statsComponent.GetZoomValue() }, 
			m_zoomValueInit {0.0f},
			m_zoomDuration{ statsComponent.GetZoomDuration() },
			m_zoomTimer{ statsComponent.GetZoomDelay()},
			m_zoomDelay { statsComponent.GetZoomDelay() },
			m_zoomFactor {0.001f}
	{}
			

	void TriggerZoom::AIIdleUpdate()
	{
		if (!g_engine.m_coordinator.GameIsActive())
			return;

		if (!m_isZooming || m_doCount == 0)
			return;

		float cameraZoom = CameraManager::instance().GetCameraZoom();
		//std::cout << "Zoom Timer: " << m_zoomDelay - m_zoomTimer << std::endl;
		//std::cout << "Zoom Delay: " << m_zoomDelay << std::endl;

		//If waiting for delay
		if (m_zoomTimer < m_zoomDelay)
		{
			m_zoomTimer += g_deltaTime;
		}
		else//if (m_zoomTimer >= m_zoomDelay)
		{
			//If the zoom is at the end
			if ((m_zoomFactor < 0.0f && cameraZoom < m_zoomValueFinal) || (m_zoomFactor > 0.0f && cameraZoom > m_zoomValueFinal))
			{
				CameraManager::instance().SetCameraZoom(m_zoomValueFinal);
				
				if(m_returning)
					--m_doCount;

				if (m_doCount <= 0)
				{
					m_isZooming = false;
					return; //No need to do the rest since not coming back here
				}

				m_returning = !m_returning;
				m_zoomTimer = 0.0f;

				float tempZoom = m_zoomValueFinal;
				m_zoomValueFinal = m_zoomValueInit;
				m_zoomValueInit = tempZoom;
				m_zoomFactor = (m_zoomValueFinal - m_zoomValueInit) / m_zoomDuration * g_deltaTime;
			}
			else
			{
				CameraManager::instance().SetCameraZoom(cameraZoom + m_zoomFactor);
			}
		}
	}

	void TriggerZoom::OnTriggerEnter(Entity otherEnt)
	{
		if (!g_engine.m_coordinator.GameIsActive())
			return;

		if (otherEnt != PLAYER_STATUS.GetPlayerEntity())
			return;
		
		m_isZooming = true;
		m_zoomValueInit = CameraManager::instance().GetCameraZoom();
		m_zoomFactor = (m_zoomValueFinal - m_zoomValueInit) / m_zoomDuration * g_deltaTime;
	}
}
