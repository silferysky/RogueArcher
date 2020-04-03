/* Start Header ************************************************************************/
/*!
\file           DeathBox.h
\project        Exale
\author         Javier Foo, javier.foo, 440002318 (100%)
\par            javier.foo\@digipen.edu
\date           3 April,2020
\brief          This file contains the functions definitions for DeathBox

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
	class DeathBox
		: public ScriptComponent
	{
	public:
		DeathBox(Entity entity, LogicComponent& logicComponent, StatsComponent& statsComponent);

		virtual void AIActiveStateUpdate() override;
		virtual void AIIdleUpdate() override;

		virtual void OnTriggerEnter(Entity other) override;
	private:
		float m_age;
		float m_lifetime;
		bool m_triggered;
		int m_playerZ;
	};
}