class AABB
{
	Vec2 min;
	Vec2 max;

public:
	AABB() : min{ Vec2() }, max{ Vec2() }
	{}
	~AABB()
	{}

	Vec2 getMin() const;
	Vec2 getMax() const;
	void setMin(const float x, const float y);
	void setMax(const float x, const float y);

	bool CollisionTest(const AABB &rhs, const Vec2 &vel1, const Vec2 &vel2);
	inline bool staticCollision(const AABB &rhs);
	bool movingCollision(const AABB &rhs, const Vec2 &vel1, const Vec2 &vel2);
};
