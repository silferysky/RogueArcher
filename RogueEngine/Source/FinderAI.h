#pragma once
#include "BaseAI.h"

namespace Rogue
{
	class FinderAI
		: public BaseAI
	{
	public:
		FinderAI(Entity entity, LogicComponent& logicComponent);

		virtual void AIActiveStateUpdate() override;

		virtual void AIDetect() override;

		virtual void AIChaseUpdate() override;

	};

}