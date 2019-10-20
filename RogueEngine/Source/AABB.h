#pragma once
#include "Vector2D.h"

class AABB // : public BaseComponent
{
	Vec2 m_min;
	Vec2 m_max;

public:
	AABB() = default;
	~AABB() = default;

	Vec2 getMin() const;
	Vec2 getMax() const;
	void setMin(const Vec2& min);
	void setMax(const Vec2& max);
};
