#pragma once
#include "Types.h"

namespace Rogue
{
	struct TimedEntity
	{
		Entity m_entity;
		float m_durationLeft;

		TimedEntity(Entity entity, float duration = 1.0f)
			: m_entity{ entity }, m_durationLeft{ duration }{}
	};
}