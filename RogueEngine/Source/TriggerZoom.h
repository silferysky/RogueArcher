#pragma once
#include "ScriptComponent.h"

namespace Rogue
{
	class TriggerZoom
		: public ScriptComponent
	{
	public:
		TriggerZoom(Entity entity, LogicComponent& logicComponent, StatsComponent& statsComponent);

		void AIIdleUpdate() override;

		void OnTriggerEnter(Entity otherEnt) override;

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