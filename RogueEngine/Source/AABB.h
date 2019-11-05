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
		AABB() = default;
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