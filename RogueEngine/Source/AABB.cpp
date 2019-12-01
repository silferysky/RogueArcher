/* Start Header ************************************************************************/
/*!
\file           AABB.cpp
\project        Exale
\author         Lim Jia Hao, l.jiahao, 390006518 (100%)
\par            l.jiahao\@digipen.edu
\date           1 December,2019
\brief          This file contains the function definitions for AABB

All content (C) 2019 DigiPen (SINGAPORE) Corporation, all rights
reserved.

Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header **************************************************************************/
#include "Precompiled.h"
#include "AABB.h"

namespace Rogue
{
	AABB::AABB(const Vec2& min, const Vec2& max) :
		m_min{ min }, m_max{ max }
	{}

	AABB::AABB(const AABB& aabb) :
		BaseCollider(aabb), m_min{ aabb.m_min }, m_max{ aabb.m_max }
	{}

	AABB::AABB(AABB&& rhs) noexcept :
		BaseCollider{ rhs }, m_min{ Vec2{} }, m_max{ Vec2{} }
	{
		std::swap(m_min, rhs.m_min);
		std::swap(m_max, rhs.m_max);
	}

	AABB& AABB::operator=(const AABB& rhs)
	{
		if (this != &rhs)
		{
			BaseCollider::operator=(rhs);
			m_min = rhs.m_min;
			m_max = rhs.m_max;
		}

		return *this;

	}

	AABB& AABB::operator=(AABB&& rhs) noexcept
	{
		if (this != &rhs)
		{
			std::swap(m_min, rhs.m_min);
			std::swap(m_max, rhs.m_max);
		}

		return *this;
	}

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