/* Start Header ************************************************************************/
/*!
\file           GamePlayEnding.h
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
#pragma once
#include "ScriptComponent.h"

namespace Rogue
{
	class GamePlayEnding : public ScriptComponent
	{
		int m_souls = 0;
		float m_timer = 0.0f;
		bool m_activated = false;
		float m_movement = 15.0f;
		bool m_moveleft;
		bool m_moveright;
		bool m_finalInput;
		bool m_endingAPressed;
		bool m_endingDPressed;
		bool m_trueEnding;
		bool m_finalSpriteSet;
		bool m_soundloaded;
		Entity m_finalSprite;
		Entity m_finalSpriteBackground;
	public:
		GamePlayEnding(Entity entity, LogicComponent& logicComponent, StatsComponent& statsComponent);
		virtual void AIActiveStateUpdate() override;
		virtual void AIIdleUpdate() override;
		virtual void OnTriggerEnter(Entity other) override;

		void TrueEnding();
		void ExaEnding();
		void ElaEnding();
		bool CrystalComplete;
		bool CoralComplete;
		bool VegetationComplete;
	};
}
