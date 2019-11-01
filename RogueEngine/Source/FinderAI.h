#pragma once
#include "BaseAI.h"

#define RANGE 5
#define SPEED 1

namespace Rogue
{
	class FinderAI
		: public BaseAI
	{
	public:
		FinderAI(Entity entity, LogicComponent& logicComponent);

		virtual void LogicInit() override;

		virtual void AIActiveStateUpdate() override;

		virtual void AIDetect() override;

		virtual void AIChaseUpdate() override;

	};

}