/* Start Header ************************************************************************/
/*!
\file           FadeObject.h
\project        Exale
\author         Javier Foo, javier.foo, 440002318 (100%)
\par            javier.foo\@digipen.edu
\date           3 April,2020
\brief          This file contains the functions definitions for FadeObject

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
	class FadeObject
		: public ScriptComponent
	{
	public:
		FadeObject(Entity entity, LogicComponent& logicComponent, StatsComponent& statsComponent, bool isContinuous = false, bool active = true, float maxTimer = 3.0f);

		virtual void AIActiveStateUpdate() override;
		virtual void AIIdleUpdate() override;

		void ResetTimer();
		void SetMaxTimer(float max);
		void SetIsActive(bool active);
		void SetContinuous(bool isContinuous);

		float GetTimer() const;
		float GetMaxTimer() const;
		bool IsActive() const;
		bool IsContinuous() const;

	private:
		float m_timer;
		float m_maxTimer;
		bool m_active;
		bool m_continuous;
	};
}