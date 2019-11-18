#pragma once
#include "BaseAI.h"

namespace Rogue
{
	class TriggerAI
		: public BaseAI
	{
		TriggerAI() = default;
		virtual void OnTriggerEnter(Entity other) override;
	};
}