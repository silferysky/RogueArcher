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
#include "TriggerZoomExit.h"
#include "CameraManager.h"

namespace Rogue
{
	TriggerZoomExit::TriggerZoomExit(Entity entity, LogicComponent& logicComponent, StatsComponent& statsComponent)
		: ScriptComponent(entity, logicComponent, statsComponent),
		m_isZooming{ false },
		m_returning{ false },
		m_doCount{ 0 },
		m_zoomValueFinal{ statsComponent.GetZoomValue() },
		m_zoomValueInit{ statsComponent.GetZoomValue() },
		m_zoomDuration{ statsComponent.GetZoomDuration() },
		m_zoomTimer{ 0.0f },
		m_zoomDelay{ statsComponent.GetZoomDelay() },
		m_zoomFactor{ 0.001f }
	{}


	void TriggerZoomExit::AIIdleUpdate()
	{
		if (!g_engine.m_coordinator.GameIsActive())
			return;

		if (!m_isZooming)
			return;

		float cameraZoom = CameraManager::instance().GetCameraZoom();

		//If the zoom is at the end
		if ((m_zoomFactor < 0.0f && cameraZoom < m_zoomValueFinal) || (m_zoomFactor > 0.0f && cameraZoom > m_zoomValueFinal))
		{
			CameraManager::instance().SetCameraZoom(m_zoomValueFinal);

			m_isZooming = false;
			m_zoomTimer = 0.0f;
			std::swap(m_zoomValueFinal, m_zoomValueInit);
		}
		else
		{
			CameraManager::instance().SetCameraZoom(cameraZoom + m_zoomFactor);
		}

	}

	void TriggerZoomExit::OnTriggerEnter(Entity otherEnt)
	{
		if (!g_engine.m_coordinator.GameIsActive())
			return;

		if (otherEnt != PLAYER_STATUS.GetPlayerEntity())
			return;

		m_zoomTimer = 0.0f;
		m_isZooming = true;
		m_zoomValueInit = CameraManager::instance().GetCameraZoom();
		m_zoomFactor = (m_zoomValueFinal - m_zoomValueInit) / m_zoomDuration * g_deltaTime;
	}

	void TriggerZoomExit::OnTriggerExit(Entity otherEnt)
	{
		if (!g_engine.m_coordinator.GameIsActive())
			return;

		if (otherEnt != PLAYER_STATUS.GetPlayerEntity())
			return;

		m_zoomTimer = 0.0f;
		m_isZooming = true;
		m_zoomValueInit = CameraManager::instance().GetCameraZoom();
		m_zoomFactor = (m_zoomValueFinal - m_zoomValueInit) / m_zoomDuration * g_deltaTime;
	}
}
