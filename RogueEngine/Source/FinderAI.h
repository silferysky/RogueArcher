/* Start Header ************************************************************************/
/*!
\file           FinderAI.h
\project        Exale
\author         Chan Wai Kit Terence, c.terence, 440005918 (100%)
\par            c.terence\@digipen.edu
\date           1 December,2019
\brief          This file contains the function definitions for BaseAi

All content (C) 2019 DigiPen (SINGAPORE) Corporation, all rights
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
	class FinderAI
		: public ScriptComponent
	{
	public:
		FinderAI(Entity entity, LogicComponent& logicComponent, StatsComponent& statsComponent);

		virtual void AIActiveStateUpdate() override;

		virtual void AIDetect() override;

		virtual void AIChaseUpdate() override;

		virtual void AIIdleUpdate() override;

	};

}