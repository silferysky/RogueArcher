#include "AABB.h"

namespace Rogue
{
	Vec2 AABB::getMin() const
	{
		return m_min;
	}

	Vec2 AABB::getMax() const
	{
		return m_max;
	}

	void AABB::setMin(const Vec2& min)
	{
		m_min = min;
	}

	void AABB::setMax(const Vec2& max)
	{
		m_max = max;
	}
}
