#pragma once
#include "BaseAI.h"

#define RANGE 200
#define SPEED 100

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