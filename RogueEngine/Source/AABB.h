/* Start Header ************************************************************************/
/*!
\file           AABB.h
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
#pragma once
#include "Vector2D.h"
#include "BaseCollider.h"

namespace Rogue
{
	class AABB : public BaseCollider
	{
		Vec2 m_min;
		Vec2 m_max;

	public:
		AABB(const Vec2& min = Vec2{}, const Vec2& max = Vec2{});
		~AABB() = default;

		
		AABB(const AABB& aabb);
		AABB(AABB&& rhs) noexcept;
		AABB& operator=(const AABB& rhs);
		AABB& operator=(AABB&& rhs) noexcept;

		Vec2 getMin() const;
		Vec2 getMax() const;
		void setMin(const Vec2& min);
		void setMax(const Vec2& max);
	};
}