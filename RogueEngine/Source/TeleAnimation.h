/* Start Header ************************************************************************/
/*!
\file           TeleAnimation.h
\project        Exale
\author         Javier Foo, javier.foo, 440002318 (100%)
\par            javier.foo\@digipen.edu
\date           3 April,2020
\brief          This file contains the functions definitions for TeleAnimation

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
	class TeleAnimation
		: public ScriptComponent
	{
	public:
		TeleAnimation(Entity entity, LogicComponent& logicComponent, StatsComponent& statsComponent, float teleCharge = 0.0f);

		virtual void AIActiveStateUpdate() override;
		virtual void AIIdleUpdate() override;

		void SetTeleCharge(float max);
		float GetTeleCharge() const;
	private:
		float m_teleCharge = 3.0f;
		float m_timer = 0.0f;
		bool m_timerActive = false;
	};
}
