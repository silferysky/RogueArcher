/* Start Header ************************************************************************/
/*!
\file           TriggerZoomExit.h
\project        Exale
\author         Lim Jia Hao, l.jiahao, 390006518  (100%)
\par            c.terence\@digipen.edu
\date           3 April,2020
\brief          This file contains the functions definitions for TriggerZoom

All content (C) 2020 DigiPen (SINGAPORE) Corporation, all rights
reserved.

Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header **************************************************************************/
#pragma once
#include "ScriptComponent.h"

namespace Rogue
{
	class TriggerZoomExit
		: public ScriptComponent
	{
	public:
		TriggerZoomExit(Entity entity, LogicComponent& logicComponent, StatsComponent& statsComponent);

		void AIIdleUpdate() override;

		void OnTriggerEnter(Entity otherEnt) override;
		void OnTriggerExit(Entity otherEnt) override;

	private:
		bool m_isZooming;			//Whether zooming is in effect or not
		bool m_returning;			//Whether zooming is returning or not
		unsigned m_doCount;			//Number of times to do this zoom in and zoom out
		float m_zoomValueFinal;		//Final Zoom value
		float m_zoomValueInit;		//Initial Zoom value -> Taken from CameraManager on first trigger
		float m_zoomDuration;		//How long it takes to zoom
		float m_zoomTimer;			//Self timer
		float m_zoomDelay;			//Delay between Zooming in and out
		float m_zoomFactor;			//Zoom value per frame
	};
}