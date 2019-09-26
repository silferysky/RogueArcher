#pragma once

class AABB : public BaseCollider
{
	Vec2 m_min;
	Vec2 m_max;

public:
	AABB() : m_min{ Vec2() }, m_max{ Vec2() }
	{}
	~AABB()
	{}

	Vec2 getMin() const;
	Vec2 getMax() const;
	void setMin(float x, float y);
	void setMax(float x, float y);

	inline void update(const Vec2& pos, const float scale);
	bool collisionTest(const AABB &rhs, const Vec2 &vel1, const Vec2 &vel2);
	inline bool staticCollision(const AABB &rhs);
	bool movingCollision(const AABB &rhs, const Vec2 &vel1, const Vec2 &vel2);
};
